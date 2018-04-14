//
// Created by mzapp on 4/14/18.
//
#pragma once

#include <cstdint>

namespace meta
{
    namespace ER1
    {
        class Noise
        {
            /// Produce the next sample of the waveform
            float tick();

            /// refreshes the internal values based on changes to the parameters
            void updateParams();

            float freqLFSR;
            float freqSAH;

        private:
            int64_t m_LFSR;
        };
    }
}
