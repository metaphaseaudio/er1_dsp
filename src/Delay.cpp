//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Delay.h"
#include <meta/util/range.h>

meta::ER1::Delay::Delay(float sampleRate)
    : m_BPM(120)
    , m_Depth(0)
    , m_Time(0.5)
    , m_SampleRate(sampleRate)
    , m_Data{std::vector<float>(int(m_SampleRate * 2), 0.0f), std::vector<float>(int(m_SampleRate * 2), 0.0f)} // 2 seconds is the max delay time
{ recalculatePlayheadAdvance(); }

void meta::ER1::Delay::processBlock(float** data, size_t samps)
{
    for (int i = samps; --i >=0;)
    {
        
        m_Playhead += m_PlayheadAdvance;
        m_Playhead = fmod(m_Playhead, m_SampleRate * 2);

    }
}

void meta::ER1::Delay::recalculatePlayheadAdvance()
{
    if (m_Sync)
    {
        const auto i = meta::remap_range(juce::Range<float>(0, ER1::Delay::tempoFractions.size() - 1), juce::Range<float>(0.5, 2), m_Time);
        const auto beats = ER1::Delay::tempoFractions[int(std::round(i))];
        m_PlayheadAdvance = (60.0f / m_BPM) * m_SampleRate * beats;
    }
    else { m_PlayheadAdvance = m_SampleRate * m_Time; }
}

void meta::ER1::Delay::setTime(float time)
{
    m_Time = std::min(std::max(0.0f, time), 1.0f) * 1.5f + 0.5f;
    recalculatePlayheadAdvance();
};
