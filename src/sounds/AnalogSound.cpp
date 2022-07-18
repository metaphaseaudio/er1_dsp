//
// Created by Matt Zapp on 4/3/2018.
//

#include "meta/audio/SingletonSampleRate.h"
#include "meta/util/math.h"
#include <vector>
#include "juce_audio_utils/juce_audio_utils.h"
#include "er1_dsp/sounds/AnalogSound.h"
#include "meta/util/range.h"

#define MOD_RATE_FACTOR 1

meta::ER1::Noise meta::ER1::AnalogSound::m_Noise = meta::ER1::Noise();

meta::ER1::AnalogSound::AnalogSound(float sampleRate)
    : BaseSound(sampleRate)
    , m_Pitch(250)
    , m_MainOsc(-1.0f, 1.0f, sampleRate, 250)
    , m_ModOsc(-1.0f, 1.0f, sampleRate / meta::ER1::Downsampler::OverSample)
    , m_ModDepth(0.0f)
    , m_SampleCounter([&](){ tickMod(); })
    , m_LastMix(0.0f)
{}

void meta::ER1::AnalogSound::tickMod()
{
    switch (m_ModType)
    {
        case Mod::Shape::SINE: setOscFreq(m_Pitch + m_ModDepth * wave_shape(ER1::Wave::Shape::COSINE, m_ModOsc.tick()) * -1.0f); break;
        case Mod::Shape::TRIANGLE: setOscFreq(m_Pitch + m_ModDepth * wave_shape(ER1::Wave::Shape::TRIANGLE, m_ModOsc.tick()) * -1.0f); break;
        case Mod::Shape::SQUARE: setOscFreq(m_Pitch + m_ModDepth * wave_shape(ER1::Wave::Shape::SQUARE, m_ModOsc.tick()) * -1.0f); break;
        case Mod::Shape::SAW: setOscFreq(m_Pitch + m_ModDepth * wave_shape(ER1::Wave::Shape::SAW, m_ModOsc.tick()) * -1.0f); break;
        case Mod::Shape::INVERSE_SAW: setOscFreq(m_Pitch + m_ModDepth * wave_shape(ER1::Wave::Shape::INVERSE_SAW, m_ModOsc.tick()) * -1.0f); break;
        case Mod::Shape::SANDH:
            setOscFreq(m_Pitch + m_ModDepth * (m_SAH.tick(m_Noise.tick()) / fixed_t::maxSigned()).toFloat());
            break;
        case Mod::Shape::DECAY: setOscFreq(m_Pitch + m_ModDepth * m_ModEnv.tick()); break;
        case Mod::Shape::NOISE:
            m_LastNoise = static_cast<float>(m_Noise.tick() / fixed_t::maxSigned());
            m_LastMix = (1.0f + (wave_shape(ER1::Wave::Shape::COSINE, m_ModOsc.tick())) / 2) * m_ModDepth / 1100; break;
        default: break;
    }
}

float meta::ER1::AnalogSound::tick()
{
    if (!m_Env.hasEnded())
    {
        m_SampleCounter.tick();
        auto sample = wave_shape(m_Shape, m_MainOsc.tick());
        const auto env = m_Env.tick();

        // Mix in the noise if appropriate
        const auto invMix = 1.0f - m_LastMix; // How much of the raw osc
        return ((sample * invMix) + (m_LastNoise * m_LastMix)) * env;
    }

    return 0.0f;
}

void meta::ER1::AnalogSound::reset()
{
    BaseSound::reset();
	setOscFreq(m_Pitch);
    m_SampleCounter.sync(1);
    m_MainOsc.sync(m_MainOsc.min);
    m_ModOsc.sync(m_ModOsc.min);
    m_ModEnv.reset(sampleRate);
}

void meta::ER1::AnalogSound::start()
{
    BaseSound::start();
    m_SAH.start(m_Noise.tick());
    m_ModEnv.start();
}

void meta::ER1::AnalogSound::setModulationShape(meta::ER1::Mod::Shape type)
{
    if (type != Mod::Shape::NOISE) { m_LastMix = 0.0f; }
    m_ModType = type;
}

void meta::ER1::AnalogSound::setModulationSpeed(float speed)
{
    m_ModEnv.setSpeed(sampleRate, meta::Interpolate<float>::parabolic(0.1f, 200.0f, speed, 3));
    m_ModOsc.set_freq(meta::Interpolate<float>::parabolic(0.1f, 5000.0f, speed, 13));
    m_SAH.setResetCount(
        meta::Interpolate<float>::parabolic(
            (sampleRate / meta::ER1::Downsampler::OverSample) * 6.0f, 1.0f, speed, -10
        )
    );
}

void meta::ER1::AnalogSound::setPitch(float hz) { m_Pitch = meta::limit(20.0f, 8768.0f, hz); }

void meta::ER1::AnalogSound::setSampleRate(float newRate)
{
    BaseSound::setSampleRate(newRate);
    m_MainOsc.set_sample_rate(newRate);
    // run sample-accurate, not sub-sample accurate
    m_ModOsc.set_sample_rate(newRate / meta::ER1::Downsampler::OverSample);
}

void meta::ER1::AnalogSound::setModulationDepth(float depth)
{
    auto sign = depth >= 0 ? 1 : -1;
    depth = meta::Interpolate<float>::parabolic(0.0f, 1.0f, std::abs(depth), -4) * sign;
    m_ModDepth = depth * 10000.0f;  // ER-1 does 10k on either side of neutral
}

void meta::ER1::AnalogSound::setOscFreq(float freq)
{
    m_MainOsc.set_freq(meta::limit(20.0f, 21000.0f, freq));
}

void meta::ER1::AnalogSound::setWaveShape(meta::ER1::Wave::Shape waveType)
    { m_Shape = waveType; }


float meta::ER1::AnalogSound::wave_shape(Wave::Shape shape, float accumulator_state)
{
    switch (shape)
    {
        case Wave::Shape::COSINE:
            return Shapes::cosin(accumulator_state);
        case Wave::Shape::TRIANGLE:
            return Shapes::tri(accumulator_state);
        case Wave::Shape::SQUARE:
            return Shapes::square(accumulator_state);
        case Wave::Shape::SAW:
            return accumulator_state;
        case Wave::Shape::INVERSE_SAW:
            return Shapes::inv_saw(accumulator_state);
    }

    return accumulator_state;
}

void meta::ER1::AnalogSound::processBlock(float* data, const float* ringData, int samps, int offset)
{
    for (int s = 0; s < samps; s++)
        { data[s + offset] = tick() * (ringData != nullptr ? ringData[s + offset] : 1.0f); }
}
