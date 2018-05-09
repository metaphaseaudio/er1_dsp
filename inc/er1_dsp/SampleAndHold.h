//
// Created by Matt Zapp on 5/8/2018.
//

#pragma once

#include <JuceHeader.h>

//TODO: GET RID OF JUCE IN THIS FREAKIN' LIBRARY ALREADY!
#include "Types.h"
#include "Noise.h"

namespace meta
{
    namespace ER1
    {
        class SampleAndHold
        {
        public:
            void bypass(bool yn) { m_Bypass = yn; }
            void setCenterFrequency(fp1616_t freq) { m_CenterFreq = freq; };
            void setWidth(fp1616_t width) { m_Width = width; }

            void setResetCount(uint32_t count) { m_ResetCount = count; };

            /// Produce the next sample of the waveform
            inline fp1616_t tick(fp1616_t in)
            {
                const auto tmp = filter(in);

                if (!m_Bypass)
                {
                    m_Count++;
                    if (m_Count > m_ResetCount)
                    {
                        m_Value = tmp;
                        m_Count = 0;
                    }
                }

                return m_Value;
            }

        private:
            fp1616_t filter(fp1616_t);

            fp1616_t m_Value;

            bool m_Bypass;
            fp1616_t m_CenterFreq;
            fp1616_t m_Width;
            fp1616_t m_Resonance;

            uint32_t m_Count;
            uint32_t m_ResetCount;

            juce::dsp::IIR::Filter<float> m_FilterHi;
            juce::dsp::IIR::Filter<float> m_FilterLo;
        };
    }
};


