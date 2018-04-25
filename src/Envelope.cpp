//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "er1_dsp/Envelope.h"

void meta::ER1::Envelope::reset()
{
    m_Ramp.setValue(0.0f);
}

void meta::ER1::Envelope::start()
{
    m_Ramp.setValue(1.0f);
	m_Ramp.setTarget(0.0f);
}

void meta::ER1::Envelope::setSpeed(float speed)
{
    m_Speed = speed;
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    m_Ramp.init(sampleRate);
    m_Ramp.setTime(1 / speed, sampleRate);
}
