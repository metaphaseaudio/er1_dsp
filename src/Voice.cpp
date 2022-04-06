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
    : ER1::MainOscillator(long(sampleRate))
    , sampleRate(sampleRate)
    , pitch(250)
    , pan(0.5f)
    , level(1.0f)
    , m_ModDepth(0.0f)
    , m_ModOsc(sampleRate)
    , m_Delay(sampleRate)
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
    std::fill(noiseData.begin(), noiseData.end(), 0);
    std::fill(oscData.begin(), oscData.end(), 0);

    // The modulator isn't going to be changed during this block, we can just render it.
    switch (m_ModType)
    {

        case SINE:
        case TRIANGLE:
        case SQUARE:
        case SAW:
        case INVERSE_SAW:
        case NOISE:
            m_ModOsc.processBlock(modData.data(), samps);
            break;
        case SANDH:
            // Sample and Hold is handled in course of rendering the oscilator.
            break;
        case DECAY:
            m_ModEnv.processBlock(modData.data(), samps);
            break;
    }

    // The oscillator needs to be modified by the modulator (in most cases) as it ticks.
    // it also needs to keep track of how much room it has in the buffer
    size_t tmp_offset = 0;
    for (int i = 0; i < samps; i++)
    {
        if (sampleRate - m_BlipBuffs[0].samples_avail() == 0)
        {
            relocate_samples(oscData.data() + tmp_offset, m_BlipBuffs[0].samples_avail());
            tmp_offset = m_BlipBuffs[0].sample_rate() - m_BlipBuffs[0].samples_avail();
        }

        const auto modSample = modData[i];
        envData[i] = 1.0f; m_Env.tick();
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
    this->relocate_samples(oscData.data() + tmp_offset, m_BlipBuffs[0].samples_avail());

    for (int i = 0; i < samps; i++)
    {
        const auto sample = oscData[i] + noiseData[i];
        data[0][i + offset] += sample * level * pan;
        data[1][i + offset] += sample * level * (1.0f - pan);
    }

    m_Delay.processBlock(data, samps, offset);
}

void meta::ER1::Voice::reset()
{
	setOscFreq(pitch);
    sync(MainOscillator::Min);
    m_ModOsc.sync(Modulator::Min);
    m_Env.reset(sampleRate);
    m_ModEnv.reset(sampleRate);
}

void meta::ER1::Voice::start()
{
    m_Env.start();
    m_SAH.start(m_Noise.tick());
    m_ModEnv.start();
}

void meta::ER1::Voice::setModulationShape(meta::ER1::Voice::ModShape type)
{
    switch (type)
    {
        case ModShape::SINE: m_ModOsc.shape = ER1::WaveShape::COSINE; break;
        case ModShape::TRIANGLE: m_ModOsc.shape = ER1::WaveShape::TRIANGLE; break;
        case ModShape::SQUARE: m_ModOsc.shape = ER1::WaveShape::SQUARE; break;
        case ModShape::SAW: m_ModOsc.shape = ER1::WaveShape::SAW; break;
        case ModShape::INVERSE_SAW: m_ModOsc.shape = ER1::WaveShape::INVERSE_SAW; break;
        case ModShape::NOISE: m_ModOsc.shape = ER1::WaveShape::COSINE; break;
        default: break;
    }

    m_ModType = type;
}

void meta::ER1::Voice::setModulationSpeed(float speed)
{
	speed *= MOD_RATE_FACTOR;

    // TODO: this probably isn't just linear...
    m_ModEnv.setSpeed(sampleRate, speed);
    m_ModOsc.set_freq(speed);
	m_SAH.setResetCount(static_cast<uint32_t>(sampleRate / (speed * 2.0f)));
}

void meta::ER1::Voice::setPitch(float hz) { pitch = hz; }
void meta::ER1::Voice::setSampleRate(float newRate) { sampleRate = newRate; }
void meta::ER1::Voice::setModulationDepth(float depth) { m_ModDepth = depth; }

void meta::ER1::Voice::setOscFreq(float freq)
{
    const auto nyquist = sampleRate / 2.0f;
    set_freq(meta::limit(20.0f, nyquist, freq));
}

void meta::ER1::Voice::setWaveShape(meta::ER1::WaveShape waveType)
{
    this->shape = waveType;
}

void meta::ER1::Voice::setDecay(float time)
{
    m_Env.setSpeed(sampleRate, time / ER1::MainOscillator::OverSample);
}

float meta::ER1::Voice::wave_shape(float accumState, int chan)
{

    return ER1::MainOscillator::wave_shape(accumState, chan) * m_Env.tick();
}

void meta::ER1::Voice::setTempo(float bpm) { m_Delay.setBPM(bpm); }
void meta::ER1::Voice::setDelayTime(float time) { m_Delay.setTime(time); }
void meta::ER1::Voice::setDelayDepth(float depth) { m_Delay.setDepth(depth); }
void meta::ER1::Voice::setTempoSync(bool sync) { m_Delay.setTempoSync(sync); }
