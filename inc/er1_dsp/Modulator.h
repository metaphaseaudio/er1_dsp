//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include <meta/audio/Asymp.h>

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

            struct Params
            {
                ModType modType = ModType::DECAY;
                float modAmp = 0.0f;     // -100 - 100
                float modSpeed = 0.1f;   // 0.1Hz - 5000Hz
            };


			void start();

            void reset();

            void updateParams();

            float tick();

            Params params;

        private:
            meta::Asymp m_Ramp; // For Decay modulations
        };
    }
}
