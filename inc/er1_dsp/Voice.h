//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include <meta/dsp/BandlimitedOsc.h>
#include "Noise.h"
#include "Envelope.h"
#include "Oscillator.h"
#include "SampleAndHold.h"

namespace meta
{
    namespace ER1
    {
        class SimpleOscShapes
        {
            enum Type
            {
                SINE = 0
                , TRIANGLE
                , SQUARE
                , SAW
            };


        };


        /**
         * Comprises one oscillator and two modulators (amp and pitch) with the
         * associated handling code that allows them to be treated as a single
         * ER-1 voice.
         */
        class Voice
            : ER1::Oscillator
        {
        public:
            Voice(float sampleRate);

            /// fill an array of floats with data from the voice
            void processBlock(float** data, int samps, int offset);

            /// silence the voice
            void reset();

            /// trigger the voice
            void start();

            enum ModType
            {
                SAW = 0
                , SQUARE
                , TRIANGLE
                , SANDH
                , NOISE
                , DECAY
            };

            void setSampleRate(float sampleRate);
            void setWaveType(ER1::WaveShape waveType);
            void setPitch(float freq);
            void setModulationType(ModType type);
            void setModulationSpeed(float speed);
            void setModulationDepth(float depth);
            void setDecay(float time);
            bool hasEnded() noexcept { return m_Env.hasEnded(); }

            float level;
            float pan;

        private:
            void setOscFreq(float freq);

            meta::ER1::Envelope m_Env;

            float sampleRate;
            float pitch;
            float m_ModDepth;

            ModType m_ModType = ModType::DECAY;
            ER1::Oscillator m_ModOsc;
            meta::ER1::SampleAndHold m_SAH;
            meta::ER1::Envelope m_ModEnv;

            static meta::ER1::Noise m_Noise;
        };
    }
}

