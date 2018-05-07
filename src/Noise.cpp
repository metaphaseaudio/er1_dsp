//
// Created by mzapp on 4/14/18.
//

#include <er1_dsp/Noise.h>
#include <meta/audio/SingletonSampleRate.h>

meta::ER1::Noise::Noise()
    : m_SAHCount(0)
    , m_ResetValue(0)
{}


void meta::ER1::Noise::setSAHFreq(float freq)
{
    m_ResetValue = static_cast<int>(meta::SingletonSampleRate<float>::getValue() / freq);
}

void meta::ER1::Noise::start() { m_Value = m_Random.next(); }
