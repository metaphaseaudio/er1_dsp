//
// Created by Matt Zapp on 5/8/2018.
//

#pragma once

#include <meta/util/BitMask.h>
#include <meta/util/fixed_point/Math.h>
#include <juce_dsp/juce_dsp.h>
#include "Types.h"
#include "Noise.h"

namespace meta::ER1
{
    class SampleAndHold
    {
    public:
        SampleAndHold();

        void setSampleRate(float sr);

        void setResetCount(uint32_t count)
            { m_ResetCount = count; };

        void start(float value)
        {
            m_Value = value;
            m_Filter.reset();
            m_Count = 0;
        };

        /// Produce the next sample of the waveform
        float tick(float in);

    private:
        float sampleRate;
        float m_Value;
        uint32_t m_Count;
        uint32_t m_ResetCount;
        juce::dsp::IIR::Filter<float> m_Filter;
    };
};


