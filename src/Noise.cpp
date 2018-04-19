//
// Created by mzapp on 4/14/18.
//

#include <er1_dsp/Noise.h>

meta::ER1::Noise::Noise()
    : m_SAHCount(0)
    , freqSAH(0)
    , freqLFSR(0)
{}

float meta::ER1::Noise::tick()
{
    for (int i = freqLFSR; --i >= 0;) { m_Random.next(); }
    if (m_SAHCount <= 0)
    {
        m_SAHCount = freqSAH;
        m_Value = m_Random.next();
    }

    m_SAHCount--;
    return static_cast<float>(m_Value) / static_cast<float>(UINT16_MAX);
}
