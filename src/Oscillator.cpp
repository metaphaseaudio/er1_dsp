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

std::array<float, 256> Oscillator::m_WaveTable =
        meta::BandlimitedWavetable<float, 256>::makeSquare(1, 1, 0.0f);

Oscillator::Oscillator()
    : m_TablePhases{fixed_t(0)}
    , m_TableDeltas{fixed_t(0)}
{
    for (int harmonic = 0; harmonic < HARMONIC_COUNT; harmonic++)
    {
        m_Coeffs[harmonic] =
                meta::BandlimitedWavetable<float, 4800>::getPartialGain
                        (harmonic + 1, HARMONIC_COUNT, 0.2f) * 0.5f;
    }
}

void Oscillator::sync()
{
    const auto rootPhase = float(m_WaveTable.size()) / 4.0f;

    for (int harm = HARMONIC_COUNT; --harm >= 0;)
        { m_TablePhases[harm] = rootPhase; }

    advanceAllPartials();

    m_Integrate.reset();
    m_Integrate.setLastSample(0.5f);

    m_SineFilter.reset();
    m_SineFilter.setLastSample(0.5f);
}

void Oscillator::setFrequency(float freq)
{
    if (csm_LimitFreq) { freq = limit<float>(0.50f, 22000.0f, freq); }

    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    // Calculate base delta
    const fixed_t phaseDelta(float(m_WaveTable.size()) * freq / sampleRate);
    m_MaxDelta = static_cast<float>(m_WaveTable.size()) * (sampleRate / 2.0f) / sampleRate;
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

float Oscillator::sumPartials(Oscillator::Partials p)
{
	float retval = 0.0;

    // mute upper partials if above nyquist
    int maxHarm = HARMONIC_COUNT - 1;

    while (maxHarm >= 0 && m_TableDeltas[maxHarm] >= m_MaxDelta) { maxHarm--; }

    // from either the first odd or even harmonic, iterate to include all allowed harmonic
    for (int harm = (p == Partials::evens) ? 1 : 0; harm <= maxHarm; harm += 2)
    {
		const auto i = m_TablePhases[harm].integral();
        retval += m_WaveTable[i] * m_Coeffs[harm];
    }

	return retval;
}
