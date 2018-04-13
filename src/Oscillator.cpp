//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include "../inc/er1_dsp/Oscillator.h"

using namespace meta::ER1;

std::array<float, 4800> Oscillator::m_SquareTable =
        meta::BandlimitedWavetable<float, 4800>::makeSquare(30, 0.2);

std::array<float, 4800> Oscillator::m_SawTable =
        meta::BandlimitedWavetable<float, 4800>::makeSaw(30, 0.2);

Oscillator::Oscillator()
	: m_TablePhase(0.0f)
	, m_PhaseDelta(0.0f)
{}

float Oscillator::tick()
{
    const auto tableSize = m_SquareTable.size();
	const auto i = static_cast<int>(m_TablePhase);

    const auto saw    = m_SawTable[i] * 0.5;
    const auto square = m_SquareTable[i];
    const auto tri    = m_Integrate.processSample(square) * 1.41254f;
    const auto sine   = m_SineFilter.processSample(tri);

    m_TablePhase += m_PhaseDelta;
	m_TablePhase = fmodf(m_TablePhase, tableSize);

    switch(params.waveType)
    {
        default:
        case WaveType::SINE:     return sine;
        case WaveType::TRIANGLE: return tri;
        case WaveType::SQUARE:   return square;
        case WaveType::SAW:      return saw;
    }
}

void Oscillator::reset()
{
    m_TablePhase = m_SquareTable.size()/2;

    m_Integrate.reset();
    m_Integrate.setLastSample(0.5f);
    m_SineFilter.reset();
    m_SineFilter.setLastSample(0.5f);
}

void Oscillator::updateParams()
{
    auto sampleRate = meta::SingletonSampleRate<float>::getValue();

    m_Integrate.setCutoff(sampleRate, params.freq / 2);
    m_SineFilter.setCutoff(sampleRate, params.freq / 2);
 
    m_PhaseDelta = static_cast<float>(m_SquareTable.size()) * params.freq / sampleRate;
}