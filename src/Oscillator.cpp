//
// Created by Matt Zapp on 4/3/2018.
//

#include <cmath>
#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/fixed_point/Math.h>
#include <meta/util/math.h>
#include "../inc/er1_dsp/Oscillator.h"

using namespace meta;
using namespace meta::ER1;


std::array<float, Oscillator::TABLE_SIZE> Oscillator::m_WaveTable =
        meta::BandlimitedWavetable<float, Oscillator::TABLE_SIZE>::makeSin();


Oscillator::Oscillator(float sample_rate, float init_freq, WaveType wt)
    : m_Harmonics{}
{ setFrequency(sample_rate, init_freq); }


void Oscillator::sync()
{
    const auto targetPhase = float(m_WaveTable.size()) / 2.0f;
	const auto advDist = targetPhase / m_Harmonics[0].state.delta;

	for (int harm = HARMONIC_COUNT; --harm >= 0;)
	    { m_Harmonics[harm].state.phase = m_Harmonics[harm].state.delta * advDist; }

    m_Integrate.reset();
    m_Integrate.setLastSample(0.5f);

    m_SineFilter.reset();
    m_SineFilter.setLastSample(0.5f);
}


void Oscillator::setFrequency(float sampleRate, float freq)
{
    // Sanitize input
    auto nyquist = sampleRate / 2;
    freq = limit<float>(0.1f, nyquist, freq);
    if (m_Frequency == freq) { return; }
    m_Frequency = freq;

    // Calculate base delta
    const float phaseDelta = meta::WavetableHelpers<float>::calculate_delta(float(m_WaveTable.size()), sampleRate, freq);
    m_MaxDelta = static_cast<float>(m_WaveTable.size()) * nyquist / sampleRate;
    m_Harmonics.at(0).state.delta = phaseDelta;

    // update all partials
    for (int harm = 0; harm < HARMONIC_COUNT; harm++)
    {
    }

    // update filtering
    m_Integrate.setCutoff(sampleRate, fabsf(freq) / 2.0f);
    m_SineFilter.setCutoff(sampleRate, fabsf(freq) / 2.0f);
    sync();
}


void Oscillator::setWaveType(Oscillator::WaveType t)
{
    m_WaveType = t;
    setFrequency(m_Frequency);
    for (int i = HARMONIC_COUNT; --i >= 0;)
    {
        switch (t)
        {
            case WaveType::SQUARE:
                m_Harmonics.at(i).gain = meta::getSquarePartialGain<float>((i * 2) + 1);
                break;
            case WaveType::TRIANGLE:
                m_Harmonics.at(i).gain = meta::getTrianglePartialGain<float>((i * 2) + 1);
                break;
            case WaveType::SAW:
                m_Harmonics.at(i).gain = meta::getSawPartialGain<float>(i);
                break;
            case WaveType::SINE:
            default:
                m_Harmonics.at(i).gain = (i == 0) ? 1.0f : 0.0f;
                break;
        }
    }
}
