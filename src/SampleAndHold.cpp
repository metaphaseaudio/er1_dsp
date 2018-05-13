//
// Created by Matt Zapp on 5/8/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/math.h>
#include "er1_dsp/SampleAndHold.h"

meta::ER1::SampleAndHold::SampleAndHold()
    : m_Value(0)
{}

meta::ER1::fixed_t meta::ER1::SampleAndHold::tick(meta::ER1::fixed_t in)
{
    m_Count++;
    if (m_Count > m_ResetCount)
    {
        const auto max = (in.sign() > 0) ? fixed_t::maxSigned() : fixed_t::minSigned();
        const auto diff = max - in;
        m_Value = in + (diff / 4);
        m_Count = 0;
    }

    return m_Value;
}
