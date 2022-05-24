//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "er1_dsp/Envelope.h"

void meta::ER1::Envelope::reset(float sampleRate)
{
    m_Ramp.init(sampleRate);
    setSpeed(sampleRate, m_Speed);
}

void meta::ER1::Envelope::start()
{
    m_Ramp.setValue(1.0f);
	m_Ramp.setTarget(0.0f);
}

void meta::ER1::Envelope::setSpeed(float sampleRate, float speed)
{
    m_Speed = speed;
    m_Ramp.setTime(1 / speed, sampleRate);
}

bool meta::ER1::Envelope::hasEnded() const
{
    return m_Ramp.getState() == 0;
}

void meta::ER1::Envelope::processBlock(float* data, int n_samps)
{
    for (int i = 0; i < n_samps; i++)
        { data[i] += tick(); }
}

void meta::ER1::Envelope::setSampleRate(float sampleRate) {
    setSpeed(m_Speed, sampleRate);
}
