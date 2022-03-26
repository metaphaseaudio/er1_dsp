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
    , m_Data{std::vector<float>(int(m_SampleRate * 2), 0.0f), std::vector<float>(int(m_SampleRate * 2), 0.0f)} // 2 seconds is the max delay time
    , m_Playhead(0, m_SampleRate * 2, sampleRate)
{
    recalculatePlayheadAdvance();
}

void meta::ER1::Delay::processBlock(float** data, size_t samps)
{
    for (int s = 0; s < samps; s++)
    {
        // save the data for later
        const auto ifj = meta::WavetableHelpers<float>::calculateIFJ(m_Data[0].size(), m_Playhead.getValue());
        const auto lsamp_out = meta::WavetableHelpers<float>::calculate_sample(m_Data[0].data(), ifj);
        const auto rsamp_out = meta::WavetableHelpers<float>::calculate_sample(m_Data[1].data(), ifj);

        // swap L&R and add to the output
        data[0][s] += rsamp_out * m_Depth;
        data[1][s] += lsamp_out * m_Depth;

        // Insert the incoming data, interpolated.
        const auto i = std::get<0>(ifj);
        const auto f = std::get<1>(ifj);
        const auto j = std::get<2>(ifj);
        m_Data[0][i] = data[0][s] * (1.0f - f);
        m_Data[0][j] = data[0][s] * f;
        m_Data[1][i] = data[1][s] * (1.0f - f);
        m_Data[1][j] = data[1][s] * f;

        // Advance the playhead
        m_Playhead.tick();
    }
}

void meta::ER1::Delay::recalculatePlayheadAdvance()
{
    if (m_Sync)
    {
        const auto i = meta::remap_range(juce::Range<float>(0, ER1::Delay::tempoFractions.size() - 1), juce::Range<float>(0.5, 2), m_Time);
        const auto frac = ER1::Delay::tempoFractions[int(std::round(i))];

        m_Playhead.set_freq((m_BPM / 60.0f) / frac);
    }
    else { m_Playhead.set_freq(m_Time / 60.0f); }
}

void meta::ER1::Delay::setTime(float time)
{
    m_Time = std::min(std::max(0.0f, time), 1.0f) * 1.5f + 0.5f;
    recalculatePlayheadAdvance();
}

void meta::ER1::Delay::setDepth(float depth)
{
    m_Depth = std::min(std::max(0.0f, depth), 1.0f);
}

void meta::ER1::Delay::setBPM(float bpm)
{
    m_BPM = std::max(0.0f, bpm);
    recalculatePlayheadAdvance();
}

void meta::ER1::Delay::setTempoSync(bool sync)
{
    m_Sync = sync;
    recalculatePlayheadAdvance();
}
