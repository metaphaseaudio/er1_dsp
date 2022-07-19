//
// Created by Matt Zapp on 5/8/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/math.h>
#include <meta/audio/decibels.h>
#include "er1_dsp/SampleAndHold.h"

meta::ER1::SampleAndHold::SampleAndHold()
    : m_Value(0)
    , m_ResetCount(0)
    , m_Count(0)
{}

float meta::ER1::SampleAndHold::tick(float in)
{
    m_Count++;
    if (m_Count > m_ResetCount)
    {
        const auto max = (in > 0) ? meta::NumInfo<float>::Max : meta::NumInfo<float>::Min;
        const auto coeff = in / max;
		const auto invCoeff = 1.0f - coeff;
		auto value = (in * invCoeff) + (max * coeff);

		m_Value = m_Filter.coefficients != nullptr ? m_Filter.processSample(value) : value;
		m_Filter.snapToZero();
        m_Count = 0;
    }

    return m_Value;
}

void meta::ER1::SampleAndHold::setSampleRate(float sr)
{
    m_Filter.prepare({sr, 1, 1});
    m_Filter.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, 20000, 0.21, meta::db_to_gain_coeff(10));
    m_Filter.reset();
}
