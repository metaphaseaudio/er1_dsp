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

        virtual ~Voice() = default;

        virtual void processBlock(float* data, const float* ringData, int samps, int offset) = 0;

        void setDecay(float time) { m_Env.setSpeed(sampleRate, time); };

        virtual void setSampleRate(float newRate) { sampleRate = newRate; };

        /// silence the voice
        virtual void reset() { m_Env.reset(sampleRate); };

        /// trigger the voice
        virtual void start() { m_Env.start(); };

        /// Indicates whether or not the envelope has reached 0.
        [[nodiscard]] bool hasEnded() const noexcept { return m_Env.hasEnded(); }

        virtual void setWaveShape(Wave::Shape waveType) {};
        virtual void setPitch(float freq) {};
        virtual void setModulationShape(Mod::Shape type) {};
        virtual void setModulationSpeed(float speed) {};
        virtual void setModulationDepth(float depth) {};

    protected:
        float sampleRate;
        meta::ER1::Envelope m_Env;
    };
}