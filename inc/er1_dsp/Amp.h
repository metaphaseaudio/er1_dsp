//
// Created by Matt Zapp on 4/3/2018.
//

#pragma once
#include "Modulator.h"

namespace meta
{
    namespace ER1
    {
        class Amp
        {
        public:
            /// advances the envelope and returns its value multiplied by the input
            inline float processSample(float input) { return input * level * envelope.tick(); }

            /// silence the amplifier envelope
            void reset();

            /// trigger the amplifier envelope
            void start();

            /// updates the envelope
            void updateParams();

            float level = 1.0f;
            float lowBoost = 0.0f;

            meta::ER1::Modulator envelope;
        };
    }
}
