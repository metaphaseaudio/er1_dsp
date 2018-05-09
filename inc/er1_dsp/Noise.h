//
// Created by mzapp on 4/14/18.
//
#pragma once

#include <cstdint>
#include <meta/dsp/FixedPointRandom.h>
#include "Types.h"


namespace meta
{
    namespace ER1
    {
        class Noise
        {
        public:
            Noise();

            void start();

            /// Produce the next sample of the waveform
            inline fp1616_t tick()
            {
                return fp1616_t(((static_cast<float>(m_Value)
                                  / static_cast<float>(UINT32_MAX))
                                 * 2.0f)
                                - 1.0f);
            }

        private:
            meta::FixedPointRandom<uint32_t> m_Random;
            uint32_t m_Value;
            int m_SAHCount;
        };
    }
}
