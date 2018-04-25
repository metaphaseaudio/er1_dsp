//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include <meta/audio/Asymp.h>
#include "Oscillator.h"
#include "Noise.h"

namespace meta
{
    namespace ER1
    {
        class Envelope
        {
        public:

			void start();

            void reset();

            void setSpeed(float speed);

            inline float tick() { return m_Ramp.tick(); }

        private:
            float m_Speed;
            meta::Asymp m_Ramp;
        };
    }
}
