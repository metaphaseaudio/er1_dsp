//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include "Oscillator.h"
#include "Noise.h"
#include "Envelope.h"
#include "SampleAndHold.h"

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
            void setWaveType(ER1::Oscillator::WaveType waveType);
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

            meta::ER1::Oscillator m_Osc;
            meta::ER1::Envelope m_Env;

            float sampleRate;
            float pitch;
            float m_ModSpeed;
            float m_ModDepth;
			int m_ModCount;


            ModType m_ModType = ModType::DECAY;
            meta::ER1::Oscillator m_ModOsc;
            meta::ER1::SampleAndHold m_SAH;
            meta::ER1::Envelope m_ModEnv;

            static meta::ER1::Noise m_Noise;
        };
    }
}

