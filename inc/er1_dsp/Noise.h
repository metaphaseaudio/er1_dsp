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

            void start();

            /// Produce the next sample of the waveform
			inline float tick()
			{
				m_SAHCount++;
				if (m_SAHCount > m_ResetValue)
				{
					m_SAHCount = 0;
					m_Value = m_Random.next();
				}

				return static_cast<float>(m_Value) / static_cast<float>(UINT32_MAX);
			}


            void setSAHFreq(float freq);

        private:
            meta::FixedPointRandom<uint32_t> m_Random;
            uint32_t m_Value;
            int m_SAHCount;
            int m_ResetValue;
        };
    }
}
