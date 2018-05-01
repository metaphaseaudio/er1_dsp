//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "../inc/er1_dsp/Oscillator.h"

using namespace meta::ER1;

std::array<float, 4800> Oscillator::m_SinTable =
        meta::BandlimitedWavetable<float, 4800>::makeSquare(1, 1, 0.0f);

std::array<float, 1024> Oscillator::m_SquareTable =
        meta::BandlimitedWavetable<float, 1024>::makeSquare(14, 1, 0.2f);

std::array<float, 1024> Oscillator::m_SawTable =
        meta::BandlimitedWavetable<float, 1024>::makeSaw(14, 1, 0.2f);

Oscillator::Oscillator()
	: m_TablePhase(0.0f)
	, m_PhaseDelta(0.0f)
    , m_TablePhases{0}
    , m_TableDeltas{0}
{
    for (int harmonic = 0; harmonic < PARTIAL_COUNT; harmonic++)
    {
        m_Coeffs[harmonic] = meta::BandlimitedWavetable<float, 4800>::getPartialGain(harmonic + 1, PARTIAL_COUNT, 0.2f) * 0.5f;
    }
}

float Oscillator::tick()
{
    const float tableSize = m_SinTable.size();
	const auto i = static_cast<int>(m_TablePhase);

//    const auto saw    = m_SawTable[i] * 0.5;
//    const auto square = m_SquareTable[i];
    const auto square = advanceAndSumOdds();
    const auto saw    = (advanceAndSumEvens() + square) * 0.5;
    const auto tri    = m_Integrate.processSample(square) * 1.41254f;
    const auto sine   = m_SineFilter.processSample(tri);

    m_TablePhase += m_PhaseDelta;

	while (m_TablePhase < 0.0f)
		{ m_TablePhase += static_cast<float>(tableSize);}

	m_TablePhase = fmodf(m_TablePhase, tableSize);

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
    m_TablePhase = float(m_SquareTable.size()) / 2.0f;
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
    m_PhaseDelta = static_cast<float>(m_SinTable.size()) * freq / sampleRate;
	m_MaxDelta = static_cast<float>(m_SinTable.size()) * (sampleRate / 2.0f) / sampleRate;
    m_TableDeltas[0] = m_PhaseDelta;

    for (int harm = 1; harm <= PARTIAL_COUNT; harm++)
        { m_TableDeltas[harm - 1] = m_TableDeltas[0] * harm; }

}

float Oscillator::advanceAndSumEvens()
{
    float retval = 0;

    for (int harm = 1; harm < PARTIAL_COUNT; harm += 2)
    {
        const auto tableSize = m_SinTable.size();
		const auto sinIndex = static_cast<int>(m_TablePhases[harm]);

		if (m_TableDeltas[harm] <= m_MaxDelta) { retval += m_SinTable[sinIndex] * m_Coeffs[harm]; }
        m_TablePhases[harm] += m_TableDeltas[harm];

        while (m_TablePhases[harm] < 0.0f) { m_TablePhases[harm] += static_cast<float>(tableSize);}
		m_TablePhases[harm] = fmodf(m_TablePhases[harm], tableSize);
    }

    return retval;
}

float Oscillator::advanceAndSumOdds()
{
    float retval = 0;

    for (int harm = 0; harm < PARTIAL_COUNT; harm += 2)
    {
        const auto tableSize = m_SinTable.size();
        const auto sinIndex = static_cast<int>(m_TablePhases[harm]);

		if (m_TableDeltas[harm] <= m_MaxDelta) { retval += m_SinTable[sinIndex] * m_Coeffs[harm]; }
        m_TablePhases[harm] += m_TableDeltas[harm];
        
        while (m_TablePhases[harm] < 0.0f) { m_TablePhases[harm] += static_cast<float>(tableSize);}
        m_TablePhases[harm] = fmodf(m_TablePhases[harm], m_SinTable.size());
    }

    return retval;
}
