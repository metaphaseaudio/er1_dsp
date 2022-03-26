//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Delay.h"
#include <meta/util/range.h>
#include <meta/dsp/WavetableReader.h>


meta::ER1::Delay::Delay(float sampleRate)
    : m_BPM(120)
    , m_Depth(0)
    , m_Time(0.5)
    , m_SampleRate(sampleRate)
    , m_Playhead(0)
    , m_Writehead(0)
    , m_DelaySampsTarget(0)
    , m_DelaySampsCurrent(m_SampleRate * 2)
    , m_Data{
        std::vector<float>(int(m_SampleRate * 2), 0.0f),
        std::vector<float>(int(m_SampleRate * 2), 0.0f)
    } // 2 seconds is the max delay time
{ recalculateDelaySamps(true); }

void meta::ER1::Delay::processBlock(float** data, size_t samps, int offset)
{
    for (int s = 0; s < samps; s++)
    {
        // inertia of tape transport, creates pitching effect when changing delay time
        // delay_time_current is set to reach delay_time_target over a period of time
        // delay_time_current speed towards delay_time_target is reduced as the
        // differential between them decreases
        m_DelaySampsCurrent = m_DelaySampsCurrent + 0.0001 * (m_DelaySampsTarget - m_DelaySampsCurrent);

        // save the data for later
        const auto playhead = m_Playhead;//.getValue();
        const auto ifj = meta::WavetableHelpers<float>::calculateIFJ(m_Data[0].size(), playhead);
        const auto lsamp_out = meta::WavetableHelpers<float>::calculate_sample(m_Data[0].data(), ifj);
        const auto rsamp_out = meta::WavetableHelpers<float>::calculate_sample(m_Data[1].data(), ifj);

        // swap L&R and add to the output
        data[0][s + offset] += rsamp_out * m_Depth;
        data[1][s + offset] += lsamp_out * m_Depth;

        // Insert the incoming data
        m_Data[0][m_Writehead] = data[0][s + offset];
        m_Data[1][m_Writehead] = data[1][s + offset];

        // Advance the play/writeheads
        m_Writehead = ++m_Writehead % int(m_Data->size());
        m_Playhead = m_Writehead - m_DelaySampsCurrent;

        if (m_Playhead < 0) { m_Playhead += m_Data->size(); }
    }
}

void meta::ER1::Delay::recalculateDelaySamps(bool hard)
{
    auto freq = 0.0f;

    if (m_Sync)
    {
        const auto i = meta::remap_range(juce::Range<float>(0, ER1::Delay::tempoFractions.size() - 1), juce::Range<float>(0.5, 2), m_Time);
        const auto frac = ER1::Delay::tempoFractions[int(std::round(i))];

        freq = (m_BPM / 60.0f) / frac;
    }
    else { freq = m_Time / 60.0f; }

    // At least two samples are required to represent nyquist. Higher than this shall not pass.
    m_DelaySampsTarget = std::max(2.0f, m_SampleRate / freq);

    if (hard) { m_DelaySampsCurrent = m_DelaySampsTarget; }
}

void meta::ER1::Delay::setTime(float time)
{
    m_Time = std::min(std::max(0.0f, time), 1.0f) * 1.5f + 0.5f;
    recalculateDelaySamps();
}

void meta::ER1::Delay::setDepth(float depth)
{
    m_Depth = std::min(std::max(0.0f, depth), 1.0f);
}

void meta::ER1::Delay::setBPM(float bpm)
{
    m_BPM = std::max(0.0f, bpm);
    recalculateDelaySamps();
}

void meta::ER1::Delay::setTempoSync(bool sync)
{
    m_Sync = sync;
    recalculateDelaySamps();
}
