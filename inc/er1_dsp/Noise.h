//
// Created by mzapp on 4/14/18.
//
#pragma once

#include <cstdint>
#include <meta/dsp/FixedPointRandom.h>

namespace meta
{
    namespace ER1
    {
        class Noise
        {
        public:
            Noise();

            /// Produce the next sample of the waveform
            float tick();

            float freqLFSR;
            float freqSAH;
        private:
            meta::FixedPointRandom<uint32_t> m_Random;
            uint16_t m_Value;
            int m_SAHCount;
        };
    }
}
