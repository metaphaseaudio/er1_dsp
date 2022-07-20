//
// Created by Matt Zapp on 5/8/2018.
//
#include "er1_dsp/SampleAndHold.h"
#include "meta/audio/decibels.h"

meta::ER1::SampleAndHold::SampleAndHold()
    : m_Value(0)
    , m_ResetCount(0)
    , m_Count(0)
{}

float meta::ER1::SampleAndHold::tick(meta::ER1::fixed_t in)
{
    m_Count++;
    if (m_Count > m_ResetCount)
    {
        const auto max = (in.sign() > 0) ? fixed_t::maxSigned() : fixed_t::minSigned();
        const auto coeff = in / max;
        const auto invCoeff = fixed_t(1) - coeff;
        const auto value = (in * invCoeff) + (max * coeff);
//        const auto value = in;

        const auto floatValue = static_cast<float>(value / meta::ER1::fixed_t::maxSigned());
		m_Value = m_Filter.coefficients != nullptr ? m_Filter.processSample(floatValue) : floatValue;
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
