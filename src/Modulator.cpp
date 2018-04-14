//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "../inc/er1_dsp/Modulator.h"

void meta::ER1::Modulator::reset()
{
    m_Ramp.setValue(0.0f);
}

void meta::ER1::Modulator::start()
{
    m_Ramp.setValue(amp);
	m_Ramp.setTarget(0.0f);
}

void meta::ER1::Modulator::updateParams()
{
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    m_Ramp.init(sampleRate);
    m_Ramp.setTime(1 / speed, sampleRate);
}

