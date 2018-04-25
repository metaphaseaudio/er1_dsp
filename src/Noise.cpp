//
// Created by mzapp on 4/14/18.
//

#include <er1_dsp/Noise.h>
#include <meta/audio/SingletonSampleRate.h>

meta::ER1::Noise::Noise()
    : m_SAHCount(0)
    , m_ResetValue(0)
{}

float meta::ER1::Noise::tick()
{
    m_SAHCount++;
    if (m_SAHCount > m_ResetValue)
    {
        m_SAHCount = 0;
        m_Value = m_Random.next();
    }

    return static_cast<float>(m_Value) / static_cast<float>(UINT32_MAX);
}

void meta::ER1::Noise::setSAHFreq(float freq)
{
    m_ResetValue = static_cast<int>(freq /  meta::SingletonSampleRate<float>::getValue());
}

void meta::ER1::Noise::start() { m_Value = m_Random.next(); }
