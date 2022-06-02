//
// Created by Mattb on 6/1/2022.
//

#pragma once


#include "Envelope.h"

namespace meta::ER1
{
    class AudioVoice
    {
    public:
        explicit AudioVoice(float sampleRate);

        /// modify the incoming data by the envelope and ring modulation data
        void processBlock(float* data, const float* ringData, int samps, int offset);

        /// silence the voice
        void reset() { m_Env.reset(sampleRate); };

        /// trigger the voice
        void start() { m_Env.start(); };

        void setDecay(float time) { m_Env.setSpeed(sampleRate, time); };
    private:
        float sampleRate;
        meta::ER1::Envelope m_Env;
    };
}
