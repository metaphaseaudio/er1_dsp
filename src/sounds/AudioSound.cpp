//
// Created by Mattb on 6/1/2022.
//

#include "er1_dsp/sounds/AudioSound.h"


meta::ER1::AudioSound::AudioSound(float sampleRate, AudioChannel channel)
    : BaseSound(sampleRate)
    , m_Chan(channel)
{}

void meta::ER1::AudioSound::processBlockInternal(float* data, const float* ringData, int samps, int offset)
{
    for (int i = 0; i < samps; i++)
    {
        auto s = i + offset;
        data[s] *= m_Env.tick() * (ringData != nullptr ? ringData[s] : 1.0f);
    }
}

