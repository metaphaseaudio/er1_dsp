//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include <meta/audio/Asymp.h>
#include "Constants.h"
#include "Noise.h"

namespace meta::ER1
{
    class Envelope
    {
    public:
        void start();

        void reset(float sampleRate);

        void setSampleRate(float sampleRate);
        void setSpeed(float sampleRate, float speed);

        void processBlock(float* data, int n_samps);
        inline float tick() { return m_Ramp.tick(); }

        [[nodiscard]] float getValue() const { return m_Ramp.getValue(); }
        [[nodiscard]] bool hasEnded() const;

    private:
        float m_Speed;
        meta::Asymp m_Ramp;
    };
}
