//
// Created by Matt Zapp on 4/3/2018.
//

#include <cmath>
#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/fixed_point/Math.h>
#include "../inc/er1_dsp/Oscillator.h"

using namespace meta;
using namespace meta::ER1;

std::array<float, 256> Oscillator::m_WaveTable =
        meta::BandlimitedWavetable<float, 256>::makeSquare(1, 1, 0.0f);

Oscillator::Oscillator()
    : m_TablePhases{FixedPointValue<uint32_t, 16>(0)}
    , m_TableDeltas{FixedPointValue<uint32_t, 16>(0)}
{
    for (int harmonic = 0; harmonic < HARMONIC_COUNT; harmonic++)
    {
        m_Coeffs[harmonic] = meta::BandlimitedWavetable<float, 4800>::getPartialGain(harmonic + 1, HARMONIC_COUNT, 0.2f) * 0.5f;
    }
}

void Oscillator::sync()
{
    const auto rootPhase = float(m_WaveTable.size()) / 2.0f;

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
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    // Calculate base delta
    const FixedPointValue<uint32_t, 16> phaseDelta(float(m_WaveTable.size()) * freq / sampleRate);
	m_MaxDelta = static_cast<float>(m_WaveTable.size()) * (sampleRate / 2.0f) / sampleRate;
	m_TableDeltas[0] = phaseDelta;

	// update all partials
    for (int harm = 1; harm < HARMONIC_COUNT; harm++)
	    { m_TableDeltas[harm].rawValue = phaseDelta.rawValue + m_TableDeltas[harm - 1].rawValue; }

    // update filtering
    m_Integrate.setCutoff(sampleRate, abs(freq) / 2.0f);
    m_SineFilter.setCutoff(sampleRate, abs(freq) / 2.0f);
}

void Oscillator::advanceAllPartials()
{;
    for (int harm = 0; harm < HARMONIC_COUNT; harm++)
    {
		const size_t tableSize = m_WaveTable.size();
        m_TablePhases[harm].rawValue += m_TableDeltas[harm].rawValue;

		const uint32_t& inRaw = m_TablePhases[harm].rawValue;

		uint32_t absPos = (inRaw >> 16) & 0x7FFF;
		absPos = absPos % tableSize;
		if (inRaw & -0) { absPos = tableSize - absPos; }
		const uint32_t integralPart = absPos << 16;
		const uint32_t factionalPart = m_TablePhases[harm].raw() & 0xFFFF;

		m_TablePhases[harm].rawValue = integralPart | factionalPart;
    }
}

float Oscillator::sumPartials(Oscillator::Partials p)
{
	float retval = 0.0;

    // mute upper partials if above nyquist
    int maxHarm = HARMONIC_COUNT - 1;
    while (maxHarm >= 0 && m_TableDeltas[maxHarm].rawValue >= m_MaxDelta.rawValue) { maxHarm--; }

    // from either the first odd or even harmonic, iterate to include all allowed harmonic
    for (int harm = (p == Partials::evens) ? 1 : 0; harm <= maxHarm; harm += 2)
    {
        const auto i = m_TablePhases[harm].rawValue >> 16;
        retval += m_WaveTable[i] * m_Coeffs[harm];
    }

	return retval;
}
