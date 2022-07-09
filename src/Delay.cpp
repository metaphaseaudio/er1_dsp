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
    , m_DelaySampsCurrent(0)
{
    setSampleRate(sampleRate);
}


void meta::ER1::Delay::recalculateDelaySamps(bool hard)
{
    auto newTarget = m_Time * m_SampleRate;
    if (m_Sync)
    {
        const auto i = meta::remap_range(0.0f, ER1::Delay::tempoFractions.size() - 1.0f, 0.005f, 2.0f, m_Time);
        const auto index = int(std::round(i));
        const auto frac = ER1::Delay::tempoFractions[index];

        auto freq = (m_BPM / 60.0f) / frac;
        newTarget = m_SampleRate / freq;
    }

    // At least two samples are required to represent nyquist. Higher than this shall not pass.
    m_DelaySampsTarget = std::max(2.0f, newTarget);

    if (hard)
        { m_DelaySampsCurrent = m_DelaySampsTarget; }
}

void meta::ER1::Delay::setTime(float time)
{
    time = meta::Interpolate<float>::parabolic(0.0f, 1.0f, time, 5);
    m_Time = std::min(std::max(0.0f, time), 1.0f) * 1.5f + 0.005f;
    recalculateDelaySamps();
}

void meta::ER1::Delay::setDepth(float depth)
{
    m_Depth = std::min(std::max(0.0f, depth), 0.9f);
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

void meta::ER1::Delay::setSampleRate(float sr)
{
    m_SampleRate = sr;
    m_Data[0] = std::vector<float>(int(m_SampleRate * 2), 0.0f);
    m_Data[1] = std::vector<float>(int(m_SampleRate * 2), 0.0f);
    recalculateDelaySamps(true);
}

std::array<float, 2> meta::ER1::Delay::tick(float left, float right)
{
    // inertia of tape transport, creates pitching effect when changing delay time
    // delay_time_current is set to reach delay_time_target over a period of time
    // delay_time_current speed towards delay_time_target is reduced as the
    // differential between them decreases
    m_DelaySampsCurrent = m_DelaySampsCurrent + 0.000015 * (m_DelaySampsTarget - m_DelaySampsCurrent);

    // save the data for later
    const auto table_size = m_Data[0].size();
    const auto phase = m_Playhead;
    auto i = static_cast<int>(phase) % table_size;
    auto f = phase - static_cast<int>(phase);
    auto j = (i == table_size - 1) ? 0 : i + 1;
    const auto lsamp_out = meta::Interpolate<float>::linear(m_Data[0][i], m_Data[0][j], f);
    const auto rsamp_out = meta::Interpolate<float>::linear(m_Data[1][i], m_Data[1][j], f);

    // swap L&R and add to the output
    const auto l = left + rsamp_out * m_Depth;
    const auto r = right + lsamp_out * m_Depth;

    // Insert the incoming data
    m_Data[0][m_Writehead] = l;
    m_Data[1][m_Writehead] = r;

    // Advance the play/writeheads
    m_Writehead = ++m_Writehead % int(m_Data[0].size());
    m_Playhead = m_Writehead - m_DelaySampsCurrent;

    if (m_Playhead < 0) { m_Playhead += m_Data[0].size(); }
    return {l, r};
}

