//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/math.h>
#include "../inc/er1_dsp/Voice.h"

#define MOD_RATE_FACTOR 1

meta::ER1::Noise meta::ER1::Voice::m_Noise = meta::ER1::Noise();

meta::ER1::Voice::Voice(float sampleRate)
    : sampleRate(sampleRate)
    , pitch(250)
    , pan(0.5f)
    , level(1.0f)
	, m_ModCount(MOD_RATE_FACTOR)
    , m_ModDepth(0.0f)
    , m_ModSpeed(0.01f)
    , m_Osc(sampleRate, pitch)
    , m_ModOsc(sampleRate)
{}

void meta::ER1::Voice::processBlock(float **data, int samps, int offset)
{
    for (int i = offset; i < samps + offset; i++)
    {
		auto o = m_Osc.tick();

		switch (m_ModType)
		{
			case ModType::SAW:
			case ModType::SQUARE:
			case ModType::TRIANGLE: setOscFreq(pitch + m_ModDepth * m_ModOsc.tick()); break;
			case ModType::DECAY:    setOscFreq(pitch + m_ModDepth * m_ModEnv.tick()); break;
			case ModType::SANDH:    
			{
				const auto sah = m_SAH.tick(m_Noise.tick());
				const auto modValue = sah / fixed_t::maxSigned();
				setOscFreq(pitch + m_ModDepth * modValue.toFloat());
                break;
			}
			case ModType::NOISE:
			{
				const auto noise = static_cast<float>(m_Noise.tick() / fixed_t::maxSigned()) * 0.15f;
				const auto mix = ((m_ModOsc.tick() + 1.0f) * 0.5f) * (m_ModDepth / 11000.0f);
                const auto invMix = 1.0f - mix;
				o *= invMix;
				o += noise * mix;
                break;
			}
		}

	    auto sample = o * level * m_Env.tick();

        data[1][i] += sample * pan;
        data[0][i] += sample * (1.0f - pan);
    }
}

void meta::ER1::Voice::reset()
{
	setOscFreq(pitch);
    m_Osc.sync();
    m_ModOsc.sync();
    m_Env.reset();
    m_ModEnv.reset();
}

void meta::ER1::Voice::start()
{
    m_Env.start();
    m_SAH.start(m_Noise.tick());
    m_ModEnv.start();
}

void meta::ER1::Voice::setModulationType(meta::ER1::Voice::ModType type)
{
    switch (type)
    {
        case ModType::SAW:      m_ModOsc.setWaveType(Oscillator::WaveType::SAW);       break;
        case ModType::SQUARE:   m_ModOsc.setWaveType(Oscillator::WaveType::SQUARE);    break;
        case ModType::TRIANGLE: m_ModOsc.setWaveType(Oscillator::WaveType::TRIANGLE);  break;
        default: break;
    }

    m_ModType = type;
}

void meta::ER1::Voice::setModulationSpeed(float speed)
{
	speed *= MOD_RATE_FACTOR;
    m_ModSpeed = speed;
    // TODO: this probably isn't just linear...
    m_ModEnv.setSpeed(speed / 500.0f);
    m_ModOsc.setFrequency(sampleRate, speed);
	m_SAH.setResetCount(static_cast<uint32_t>(meta::SingletonSampleRate<float>::getValue() / (speed * 2.0f)) - 1);
}

void meta::ER1::Voice::setPitch(float hz) { pitch = hz; }
void meta::ER1::Voice::setSampleRate(float newRate) { sampleRate = newRate; }
void meta::ER1::Voice::setModulationDepth(float depth) { m_ModDepth = depth; }

void meta::ER1::Voice::setOscFreq(float freq)
{
    const auto nyquist = sampleRate / 2.0f;
    m_Osc.setFrequency(sampleRate, meta::limit(20.0f, nyquist, freq));
}

void meta::ER1::Voice::setWaveType(meta::ER1::Oscillator::WaveType waveType)
{
    m_Osc.setWaveType(waveType);

}

void meta::ER1::Voice::setDecay(float time)
{
    m_Env.setSpeed(time);
}

