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
        class Modulator
        {
        public:
            enum ModType
            {
                SAW = 0
                , SQUARE
                , TRIANGLE
                , SANDH
                , NOISE
                , DECAY
            };

			void start();

            void reset();

            void updateParams();

            inline float tick()
            {
                switch(modType)
                {
                    case meta::ER1::Modulator::ModType::DECAY: return m_Ramp.tick();
                }

                return 0.0;
            }

            ModType modType = ModType::DECAY;
            float amp = 0.0f;     // -100 - 100
            float speed = 0.1f;   // 0.1Hz - 5000Hz

        private:
            meta::ER1::Oscillator m_AnalogWave;
            meta::ER1::Noise m_Noise;
            meta::Asymp m_Ramp;
        };
    }
}
