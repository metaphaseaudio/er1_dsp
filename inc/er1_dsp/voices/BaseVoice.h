//
// Created by Mattb on 6/2/2022.
//

#pragma once

#include "../Envelope.h"


namespace meta::ER1
{
    class Voice
    {
    public:
        explicit Voice(float sampleRate, float decay=1.0f)
            : sampleRate(sampleRate)
        { setDecay(decay); }

        virtual void processBlock(float* data, const float* ringData, int samps, int offset) = 0;

        void setDecay(float time) { m_Env.setSpeed(sampleRate, time); };

        virtual void setSampleRate(float sampleRate) { this->sampleRate = sampleRate; };

        /// silence the voice
        virtual void reset() { m_Env.reset(sampleRate); };

        /// trigger the voice
        virtual void start() { m_Env.start(); };

        /// Indicates whether or not the envelope has reached 0.
        [[nodiscard]] bool hasEnded() const noexcept { return m_Env.hasEnded(); }

    protected:
        float sampleRate;
        meta::ER1::Envelope m_Env;
    };
}