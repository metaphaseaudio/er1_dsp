//
// Created by Matt Zapp on 5/8/2018.
//

#include "er1_dsp/SampleAndHold.h"

meta::ER1::fp1616_t meta::ER1::SampleAndHold::filter(meta::ER1::fp1616_t in)
{
    return fp1616_t(m_FilterLo.processSample(m_FilterHi.processSample(static_cast<float>(in))));
}
