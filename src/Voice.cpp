//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/math.h>
#include "../inc/er1_dsp/Voice.h"

#define MOD_RATE_FACTOR 1

meta::ER1::Noise meta::ER1::Voice::m_Noise = meta::ER1::Noise();

meta::ER1::Voice::Voice()
    : pitch(250)
    , pan(0.5f)
    , level(1.0f)
	, m_ModCount(MOD_RATE_FACTOR)
    , m_ModDepth(0.0f)
    , m_ModSpeed(0.01f)
{}

void meta::ER1::Voice::processBlock(float **data, int chans, int samps, int offset)
{
    for (int i = offset; i < samps + offset; i++)
    {
		switch (m_ModType)
		{
			case ModType::SAW:
			case ModType::SQUARE:
			case ModType::TRIANGLE: oscillator.setFrequency(pitch + m_ModDepth * m_ModOsc.tick()); break;
			case ModType::DECAY:    oscillator.setFrequency(pitch + m_ModDepth * m_ModEnv.tick()); break;
			case ModType::SANDH:    
			{
				const auto sah = m_SAH.tick(m_Noise.tick());
				const auto modValue = sah / fixed_t::maxSigned();
				oscillator.setFrequency(pitch + m_ModDepth * modValue.toFloat());
			}
			break;
			case ModType::NOISE:
			{
				const auto mod = m_ModDepth * static_cast<float>(m_Noise.tick());
				const auto invMod = -mod;
				break;
			}
		}

		auto o = oscillator.tick();
        auto sample = o * level * envelope.tick();

        // TODO: set things up for stereo pan
        for (int c = 0; c < chans; c++) { data[c][i] += sample; }
    }
}

void meta::ER1::Voice::reset()
{
    oscillator.sync();
    m_ModOsc.sync();
    envelope.reset();
    m_ModEnv.reset();
}

void meta::ER1::Voice::start()
{
    envelope.start();
    m_SAH.start(m_Noise.tick());
    m_ModEnv.start();
}

void meta::ER1::Voice::setModulationType(meta::ER1::Voice::ModType type)
{
    switch (type)
    {
        case ModType::SAW:      m_ModOsc.waveType = Oscillator::WaveType::SAW;      break;
        case ModType::SQUARE:   m_ModOsc.waveType = Oscillator::WaveType::SQUARE;   break;
        case ModType::TRIANGLE: m_ModOsc.waveType = Oscillator::WaveType::TRIANGLE; break;
        default: break;
    }

    m_ModType = type;
}

void meta::ER1::Voice::setModulationSpeed(float speed)
{
	speed *= MOD_RATE_FACTOR;
    m_ModSpeed = speed;
    m_ModEnv.setSpeed(speed / 500.0f);
    m_ModOsc.setFrequency(speed);
	m_SAH.setResetCount
            (static_cast<uint32_t>(meta::SingletonSampleRate<float>::getValue() / (speed * 2.0f)) - 1);
}

void meta::ER1::Voice::setModulationDepth(float depth)
{
    m_ModDepth = depth;
}

void meta::ER1::Voice::setPitch(float freq)
{
    pitch = freq;
}
