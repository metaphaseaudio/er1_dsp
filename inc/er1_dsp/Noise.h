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

            /// Produce the next sample of the waveform
            inline fixed_t tick() { return fixed_t::fromRaw(m_Random.next()); }

        private:
            meta::FixedPointRandom<uint32_t> m_Random;
            int m_SAHCount;
        };
    }
}
