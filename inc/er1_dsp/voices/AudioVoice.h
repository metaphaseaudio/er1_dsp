//
// Created by Mattb on 6/1/2022.
//

#pragma once

#include "BaseVoice.h"

namespace meta::ER1
{
    class AudioVoice
        : public Voice
    {
    public:
        explicit AudioVoice(float sampleRate);
        /// modify the incoming data by the envelope and ring modulation data
        void processBlock(float* data, const float* ringData, int samps, int offset) override;
    };
}
