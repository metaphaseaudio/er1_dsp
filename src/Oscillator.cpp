//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "../inc/er1_dsp/Oscillator.h"

using namespace meta::ER1;

std::array<float, 4800> Oscillator::m_WaveTable =
        meta::BandlimitedWavetable<float, 4800>::makeSquare(1, 1, 0.0f);

Oscillator::Oscillator()
    : m_TablePhases{0}
    , m_TableDeltas{0}
{
    for (int harmonic = 0; harmonic < PARTIAL_COUNT; harmonic++)
    {
        m_Coeffs[harmonic] = meta::BandlimitedWavetable<float, 4800>::getPartialGain(harmonic + 1, PARTIAL_COUNT, 0.2f) * 0.5f;
    }
}

float Oscillator::tick()
{
    const auto square = sumPartials(odds);
    const auto saw    = (sumPartials(evens) + square) * 0.5;
    const auto tri    = m_Integrate.processSample(square) * 1.41254f;
    const auto sine   = m_SineFilter.processSample(tri);

	advanceAllPartials();

    switch(waveType)
    {
        default:
        case WaveType::SINE:     return sine;
        case WaveType::TRIANGLE: return tri;
        case WaveType::SQUARE:   return square;
        case WaveType::SAW:      return float(saw);
    }
}

void Oscillator::reset()
{
    for (int harm = PARTIAL_COUNT; --harm >=0;)
    {
        m_TablePhases[harm] = 0.0f;
        m_TableDeltas[harm] = 0.0f;
    }

    m_Integrate.reset();
    m_Integrate.setLastSample(0.5f);

    m_SineFilter.reset();
    m_SineFilter.setLastSample(0.5f);
}

void Oscillator::setFrequency(float freq)
{
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    m_Integrate.setCutoff(sampleRate, abs(freq) / 2.0f);
    m_SineFilter.setCutoff(sampleRate, abs(freq) / 2.0f);
    const auto phaseDelta = static_cast<float>(m_WaveTable.size()) * freq / sampleRate;
	m_MaxDelta = static_cast<float>(m_WaveTable.size()) * (sampleRate / 2.0f) / sampleRate;

    for (int harm = 0; harm < PARTIAL_COUNT; harm++)
	{
		m_TableDeltas[harm] = phaseDelta + ((harm > 0) ? m_TableDeltas[harm - 1] : 0);
	}
}

void Oscillator::advanceAllPartials()
{
    const auto tableSize = m_WaveTable.size();
    for (int harm = 0; harm < PARTIAL_COUNT; harm++)
    {
        m_TablePhases[harm] += m_TableDeltas[harm];
        while (m_TablePhases[harm] < 0.0f) { m_TablePhases[harm] += static_cast<float>(tableSize);}
        m_TablePhases[harm] = fmodf(m_TablePhases[harm], m_WaveTable.size());
    }
}

float Oscillator::sumPartials(Oscillator::Partials p)
{
	float retval = 0.0;

    for (int harm = (p == Partials::evens) ? 1 : 0; harm < PARTIAL_COUNT; harm += 2)
    {
        // resolve the wavetable index
        const auto i = static_cast<int>(m_TablePhases[harm]);

        // add the current partial to the sum if it's below nyquist. Otherwise, mute.
        retval += (m_TableDeltas[harm] <= m_MaxDelta)
                ? m_WaveTable[i] * m_Coeffs[harm]
                : 0;
    }

	return retval;
}
