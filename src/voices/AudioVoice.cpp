//
// Created by Mattb on 6/1/2022.
//

#include "er1_dsp/voices/AudioVoice.h"


meta::ER1::AudioVoice::AudioVoice(float sampleRate)
    : Voice(sampleRate)
{}

void meta::ER1::AudioVoice::processBlock(float* data, const float* ringData, int samps, int offset)
{
    for (int i = 0; i < samps; i++)
    {
        auto s = i + offset;
        data[s] *= m_Env.tick() * (ringData != nullptr ? ringData[s] : 1.0f);
    }
}

