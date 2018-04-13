//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include "Oscillator.h"
#include "Modulator.h"

namespace meta
{
    namespace ER1
    {
        /**
         * Comprises one oscillator and two modulators (amp and pitch) with the
         * associated handling code that allows them to be treated as a single
         * ER-1 voice.
         */
        class Voice
        {
        public:
            struct Params
            {
                float oscPitch; // The global pitch of the voice
            };

            /// fill an array of floats with data from the voice
            void processBlock(float** data, int chans, int samps, int offset);

            /// silence the voice
            void reset();

            /// trigger the voice
            void start();

            /// updates every parameter for all modulators and the oscillator
            void updateParams();

            Params params;

            meta::ER1::Oscillator oscillator;
            meta::ER1::Modulator ampModulator;
            meta::ER1::Modulator pitchModulator;
        };
    }
}

