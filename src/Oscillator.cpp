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
        meta::BandlimitedWavetable<float, Oscillator::TABLE_SIZE>::makeSquare(1, 1, 0.0f);

Oscillator::Oscillator(float init_freq)
    : m_TablePhases{fixed_t(0)}
    , m_TableDeltas{fixed_t(0)}
{
    for (int harmonic = 0; harmonic < HARMONIC_COUNT; harmonic++)
    {
        m_CoeffsLin[harmonic] = meta::getLinearPartialGain<float>(harmonic + 1, HARMONIC_COUNT, 0.2f) * 0.5f;
        m_CoeffsTri[harmonic] = meta::getTrianglePartialGain<float>(harmonic + 1, HARMONIC_COUNT, 0.2f) * 0.5f;
    }

    setFrequency(init_freq);
}

void Oscillator::sync()
{
    const auto targetPhase = float(m_WaveTable.size()) / 2.0f;
	const auto advDist = targetPhase / m_TableDeltas[0];

	for (int harm = HARMONIC_COUNT; --harm >= 0;)
	    { m_TablePhases[harm] = m_TableDeltas[harm] * advDist; }

    advanceAllPartials();

    m_Integrate.reset();
    m_Integrate.setLastSample(0.5f);

    m_SineFilter.reset();
    m_SineFilter.setLastSample(0.5f);
}

void Oscillator::setFrequency(float freq)
{
    // Sanitize input
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();
    auto nyquist = sampleRate / 2;
    freq = limit<float>(0.1f, nyquist, freq);
    if (m_Frequency == freq) { return; }
    m_Frequency = freq;

    // Calculate base delta
    const fixed_t phaseDelta(float(m_WaveTable.size()) * freq / sampleRate);
    m_MaxDelta = static_cast<float>(m_WaveTable.size()) * nyquist / sampleRate;
    m_TableDeltas[0] = phaseDelta;

    // update all partials
    for (int harm = 1; harm < HARMONIC_COUNT; harm++)
    { m_TableDeltas[harm] = phaseDelta + m_TableDeltas[harm - 1]; }

    // update filtering
    m_Integrate.setCutoff(sampleRate, fabsf(freq) / 2.0f);
    m_SineFilter.setCutoff(sampleRate, fabsf(freq) / 2.0f);
}

void Oscillator::advanceAllPartials()
{
    for (int harm = 0; harm < HARMONIC_COUNT; harm++)
    {
		const auto tableSize = m_WaveTable.size();
        m_TablePhases[harm] += m_TableDeltas[harm];

		uint32_t absPos = m_TablePhases[harm].integral() % tableSize;
		if (m_TablePhases[harm].raw() & -0) { absPos = tableSize - absPos; }

		m_TablePhases[harm] = fixed_t::fromRaw((absPos << m_TablePhases[harm].Radix)
		                    | m_TablePhases[harm].fractional());
    }
}

float Oscillator::sumPartials(Oscillator::Partials p, const float* gainCoeffs)
{
	float retval = 0.0;

    // mute upper partials if above nyquist
    int maxHarm = HARMONIC_COUNT - 1;

    while (maxHarm >= 0 && m_TableDeltas[maxHarm] >= m_MaxDelta) { maxHarm--; }

    // from either the first odd or even harmonic, iterate to include all allowed harmonic
    for (int harm = (p == Partials::evens) ? 1 : 0; harm <= maxHarm; harm += 2)
    {
		const auto i = m_TablePhases[harm].integral();
        const auto j = (i + 1) % TABLE_SIZE;
        const auto frac = m_TablePhases[harm] - i;
        const auto a = m_WaveTable[i] * (fixed_t(1) - frac);
        const auto b = m_WaveTable[j] * frac;

		retval += (a + b).toFloat() * gainCoeffs[harm];
    }

	return retval;
}
