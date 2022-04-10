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
    , m_ModOsc(ER1::MainOscillator::Min, ER1::MainOscillator::Max, sampleRate)
    , m_Delay(sampleRate * ER1::MainOscillator::OverSample)
{
    set_freq(250);
}

void meta::ER1::Voice::onSample(float)
{
    constexpr auto scale = MainOscillator::Min;
    switch (m_ModType)
    {
        case ModShape::SINE: setOscFreq(pitch + m_ModDepth * wave_shape(ER1::WaveShape::COSINE, m_ModOsc.tick()) / scale); break;
        case ModShape::TRIANGLE: setOscFreq(pitch + m_ModDepth * wave_shape(ER1::WaveShape::TRIANGLE, m_ModOsc.tick()) / scale); break;
        case ModShape::SQUARE: setOscFreq(pitch + m_ModDepth * wave_shape(ER1::WaveShape::SQUARE, m_ModOsc.tick()) / scale); break;
        case ModShape::SAW: setOscFreq(pitch + m_ModDepth * wave_shape(ER1::WaveShape::SAW, m_ModOsc.tick()) / scale); break;
        case ModShape::INVERSE_SAW: setOscFreq(pitch + m_ModDepth * wave_shape(ER1::WaveShape::INVERSE_SAW, m_ModOsc.tick()) / scale); break;
        case ModShape::SANDH: setOscFreq(pitch + m_ModDepth * (m_SAH.tick(m_Noise.tick()) / fixed_t::maxSigned()).toFloat()); break;
        case ModShape::DECAY: setOscFreq(pitch + m_ModDepth * m_ModEnv.tick()); break;
        case ModShape::NOISE:
            m_LastNoise = static_cast<float>(m_Noise.tick() / fixed_t::maxSigned()) * scale;
            m_LastMix = (1.0f + (wave_shape(ER1::WaveShape::COSINE, m_ModOsc.tick()) / scale) / 2) * m_ModDepth / 1100; break;
        default: break;
    }
}

std::array<float, 2> meta::ER1::Voice::onTick(float accumState)
{
    auto sample = wave_shape(m_Shape, accumState);
    const auto env = m_Env.tick();

    // Mix in the noise if appropriate
    const auto invMix = 1.0f - m_LastMix; // How much of the raw osc
    sample = ((sample * invMix) + (m_LastNoise * m_LastMix)) * env * level;

    const auto l = sample * pan;
    const auto r = sample * (1.0f - pan);

    return m_Delay.tick(l, r);
}

void meta::ER1::Voice::processBlock(float **data, int samps, int offset)
{
    while (samps > 0)
    {
        const auto to_render = std::min<int>(
            samps - m_BlipBuffs[0].samples_avail(),
            m_BlipBuffs[0].sample_rate() - m_BlipBuffs[0].samples_avail()
        );

        tick(to_render);
        const auto tmp_offset = m_BlipBuffs[0].samples_avail();
        samps -= m_BlipBuffs[0].samples_avail();

        for (int chan = 2; --chan >= 0;)
            { relocate_samples(data[chan] + offset, to_render, chan); }

        offset += tmp_offset;
    }
}

void meta::ER1::Voice::reset()
{
	setOscFreq(pitch);
    sync(MainOscillator::Min);
    m_ModOsc.sync(MainOscillator::Min);
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
    if (type != NOISE) { m_LastMix = 0.0f; }
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
    { m_Shape = waveType; }

void meta::ER1::Voice::setDecay(float time)
{
    m_Env.setSpeed(sampleRate, time / ER1::MainOscillator::OverSample);
}


void meta::ER1::Voice::setTempo(float bpm) { m_Delay.setBPM(bpm); }
void meta::ER1::Voice::setDelayTime(float time) { m_Delay.setTime(time); }
void meta::ER1::Voice::setDelayDepth(float depth) { m_Delay.setDepth(depth); }
void meta::ER1::Voice::setTempoSync(bool sync) { m_Delay.setTempoSync(sync); }

float meta::ER1::Voice::wave_shape(WaveShape shape, float accumulator_state)
{
    constexpr auto scale_factor =  MainOscillator::Min * -1;
    const auto scaled = accumulator_state / scale_factor;
    switch (shape)
    {
        case WaveShape::COSINE:
            return Shapes::cosin(scaled) * scale_factor;
        case WaveShape::TRIANGLE:
            return Shapes::tri(scaled) * scale_factor;
        case WaveShape::SQUARE:
            return Shapes::square(scaled) * scale_factor;
        case WaveShape::SAW:
            return accumulator_state;
        case INVERSE_SAW:
            return Shapes::inv_saw(accumulator_state);
    }

    return accumulator_state;
}

