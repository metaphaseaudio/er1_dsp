//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/audio/SingletonSampleRate.h>
#include <meta/util/math.h>
#include <vector>
#include "../inc/er1_dsp/Voice.h"

#define MOD_RATE_FACTOR 1

meta::ER1::Noise meta::ER1::Voice::m_Noise = meta::ER1::Noise();

meta::ER1::Voice::Voice(float sampleRate)
    : ER1::Oscillator(long(sampleRate))
    , sampleRate(sampleRate)
    , pitch(250)
    , pan(0.5f)
    , level(1.0f)
	, m_ModCount(MOD_RATE_FACTOR)
    , m_ModDepth(0.0f)
    , m_ModSpeed(0.01f)
    , m_ModOsc(sampleRate)
{
    set_freq(250);
}

void meta::ER1::Voice::processBlock(float **data, int samps, int offset)
{
    std::vector<float> modData(samps);
    std::vector<float> envData(samps);
    std::vector<float> noiseData(samps);
    std::vector<float> oscData(samps);
    std::fill(modData.begin(), modData.end(), 0);
    std::fill(envData.begin(), envData.end(), 0);
    std::fill(noiseData.begin(), noiseData.end(), 0);
    std::fill(oscData.begin(), oscData.end(), 0);

    // The modulator isn't going to be changed during this block, we can just render it.
    m_ModOsc.processBlock(modData.data(), samps);

    // The oscillator needs to be modified by the modulator (in most cases) as it ticks
    for (int i = 0; i < samps; i++)
    {
        const auto modSample = modData[i];
        envData[i] = m_Env.tick();
        this->tick();

        if (m_ModType == SANDH)
        {
            const auto sah = m_SAH.tick(m_Noise.tick());
            const auto modValue = sah / fixed_t::maxSigned();
            setOscFreq(pitch + m_ModDepth * modValue.toFloat());
        }
        else if (m_ModType == NOISE)
        {
            const auto noise = static_cast<float>(m_Noise.tick() / fixed_t::maxSigned()) * 0.15f;
            const auto mix = ((modSample + 1.0f) * 0.5f) * (m_ModDepth / 11000.0f); // How much of the noise
            const auto invMix = 1.0f - mix; // How much of the raw osc
            envData[i] *= invMix;
            noiseData[i] += noise * mix;
        }
        else
        {
            setOscFreq(pitch + m_ModDepth * modSample);
        }
    }

    // we can use this as temporary storage while we do the final mix
    this->relocate_samples(oscData.data(), samps);

    for (int i = 0; i < samps; i++)
    {
        const auto sample = oscData[i] * envData[i] + noiseData[i];
        data[0][i + offset] += sample * pan;
        data[1][i + offset] += sample * (1.0f - pan);
    }
}

void meta::ER1::Voice::reset()
{
	setOscFreq(pitch);
    sync(0.0);
    m_ModOsc.sync(0.0);
    m_Env.reset(sampleRate);
    m_ModEnv.reset(sampleRate);
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
//        case ModType::SAW:      m_ModOsc.setWaveType(Oscillator::WaveType::SAW);       break;
//        case ModType::SQUARE:   m_ModOsc.setWaveType(Oscillator::WaveType::SQUARE);    break;
//        case ModType::TRIANGLE: m_ModOsc.setWaveType(Oscillator::WaveType::TRIANGLE);  break;
        default: break;
    }

    m_ModType = type;
}

void meta::ER1::Voice::setModulationSpeed(float speed)
{
	speed *= MOD_RATE_FACTOR;
    m_ModSpeed = speed;
    // TODO: this probably isn't just linear...
    m_ModEnv.setSpeed(sampleRate, speed);
    m_ModOsc.set_freq(speed);
	m_SAH.setResetCount(static_cast<uint32_t>(meta::SingletonSampleRate<float>::getValue() / (speed * 2.0f)) - 1);
}

void meta::ER1::Voice::setPitch(float hz) { pitch = hz; }
void meta::ER1::Voice::setSampleRate(float newRate) { sampleRate = newRate; }
void meta::ER1::Voice::setModulationDepth(float depth) { m_ModDepth = depth; }

void meta::ER1::Voice::setOscFreq(float freq)
{
    const auto nyquist = sampleRate / 2.0f;
    set_freq(meta::limit(20.0f, nyquist, freq));
}

void meta::ER1::Voice::setWaveType(meta::ER1::WaveShape waveType)
{
    this->shape = waveType;
}

void meta::ER1::Voice::setDecay(float time)
{
    m_Env.setSpeed(sampleRate, time);
}
