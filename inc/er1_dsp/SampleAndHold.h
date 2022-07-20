//
// Created by Matt Zapp on 5/8/2018.
//

#pragma once

#include <meta/util/BitMask.h>
#include <meta/util/fixed_point/Math.h>
#include <juce_dsp/juce_dsp.h>
#include "Types.h"
#include "Noise.h"


namespace meta
{
    namespace ER1
    {
        class SampleAndHold
        {
        public:
            SampleAndHold();

            void setResetCount(uint32_t count)
            {
                m_ResetCount = count;
            };

            void setSampleRate(float sr);

            void start(fixed_t value)
            {
                m_Value = static_cast<float>(value / meta::ER1::fixed_t::maxSigned());
                m_Count = 0;
            };

            /// Produce the next sample of the waveform
            float tick(fixed_t in);

        private:
            juce::dsp::IIR::Filter<float> m_Filter;
            float m_Value;
            uint32_t m_Count;
            uint32_t m_ResetCount;
        };
    }
};


