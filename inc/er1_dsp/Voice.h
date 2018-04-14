//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include "Oscillator.h"
#include "Modulator.h"
#include "Amp.h"

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
            Voice();

            /// fill an array of floats with data from the voice
            void processBlock(float** data, int chans, int samps, int offset);

            /// silence the voice
            void reset();

            /// trigger the voice
            void start();

            /// updates every parameter for all modulators and the oscillator
            void updateParams();

            float oscPitch; // The global pitch of the voice
            float pan;

            meta::ER1::Oscillator oscillator;
            meta::ER1::Modulator pitchModulator;
            meta::ER1::Amp       amplifier;
        };
    }
}

