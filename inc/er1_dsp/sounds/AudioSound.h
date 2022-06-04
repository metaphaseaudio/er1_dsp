//
// Created by Mattb on 6/1/2022.
//

#pragma once

#include "BaseSound.h"

namespace meta::ER1
{
    class AudioSound
        : public BaseSound
    {
    public:
        AudioSound(float sampleRate, AudioChannel channel);

        /// modify the incoming data by the envelope and ring modulation data
        void processBlock(float* data, const float* ringData, int samps, int offset) override;

        AudioChannel wantsAudioChannel() const override { return m_Chan; }

    private:
        AudioChannel m_Chan;
    };
}
