//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/util/math.h>
#include <vector>
#include "er1_dsp/sounds/AnalogSound.h"

static constexpr float MIN_USER_FREQ = 40;
static constexpr float MAX_USER_FREQ = 8768;
static constexpr float MAX_OSC_FREQ = 19532;
static constexpr float MAX_OSC_DEPTH = MAX_OSC_FREQ - MAX_USER_FREQ;


meta::ER1::AnalogSound::AnalogSound(float sampleRate)
    : BaseSound(sampleRate)
    , m_Pitch(250)
    , m_MainOsc(-1.0f, 1.0f, sampleRate, 250)
    , m_ModOsc(-1.0f, 1.0f, sampleRate / meta::ER1::Downsampler::OverSample)
    , m_ModDepth(0.0f)
    , m_SampleCounter([&](){ tickMod(); })
    , m_LastMix(0.0f)
    , m_StdNoise(-1.0f, 1.0f)
    , m_LastNoise(0)
{
    m_ModOsc.onReset = [&](float x)
    {
        m_LastNoise = m_StdNoise.next();
    };
}

void meta::ER1::AnalogSound::tickMod()
{
    float modVal = 1;

    switch (m_ModType)
    {
        case Mod::Shape::SINE: modVal = wave_shape(ER1::Wave::Shape::COSINE, m_ModOsc.tick()); break;
        case Mod::Shape::TRIANGLE: modVal = wave_shape(ER1::Wave::Shape::TRIANGLE, m_ModOsc.tick()); break;
        case Mod::Shape::SQUARE: modVal = wave_shape(ER1::Wave::Shape::SQUARE, m_ModOsc.tick()); break;
        case Mod::Shape::SAW: modVal =  wave_shape(ER1::Wave::Shape::SAW, m_ModOsc.tick()); break;
        case Mod::Shape::INVERSE_SAW: modVal =  wave_shape(ER1::Wave::Shape::INVERSE_SAW, m_ModOsc.tick()); break;
        case Mod::Shape::SANDH: modVal = m_SAH.tick(m_StdNoise.next()); break;
        case Mod::Shape::DECAY: modVal = m_ModEnv.tick(); break;
        case Mod::Shape::NOISE:
            m_LastNoise = m_StdNoise.next();
            m_LastMix = (1.0f + (wave_shape(ER1::Wave::Shape::COSINE, m_ModOsc.tick())) / 2) * m_ModDepth;
            break;
        default: break;
    }

    // This gets the right range but is too exponential
    //    auto modValAdd = std::pow(500, m_ModDepth * modVal + 1) / 500;
    //    setOscFreq(m_Pitch * modValAdd);

    // This is also too exponential
    //    auto noteNum = m_NoteNumber + (m_ModDepth * 100 * modVal);
    //    setOscFreq(440 * std::pow(2.0, (noteNum - 69) / 12.0));

    // This is also too exponential
    //    auto noteNum = m_NoteNumber + (m_ModDepth * 100 * modVal);
    //    setOscFreq(m_Pitch * std::pow(2, (noteNum - 69) / 12.0));

    // This is too linear, doesn't get the right sort of range
    //    setOscFreq(m_Pitch + m_ModDepth * modVal * 10000);

    // Close enough.
    auto modValAdd = meta::Interpolate<float>::parabolic(-10000, 10000 + m_Pitch, (m_ModDepth * modVal + 1) / 2.0f, 2);
    setOscFreq(m_Pitch + modValAdd);

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
    m_SAH.start(m_StdNoise.next());
    m_ModEnv.start();
}

void meta::ER1::AnalogSound::setModulationShape(meta::ER1::Mod::Shape type)
{
    if (type != Mod::Shape::NOISE) { m_LastMix = 0.0f; }
    m_ModType = type;
}

void meta::ER1::AnalogSound::setModulationSpeed(float speed)
{
    m_ModEnv.setSpeed(sampleRate, meta::Interpolate<float>::parabolic(0.1f, 200.0f, speed, 8));
    m_ModOsc.set_freq(meta::Interpolate<float>::parabolic(0.1f, 19000.0f, speed, 13));
    m_SAH.setResetCount(
        meta::Interpolate<float>::parabolic(
            (sampleRate / meta::ER1::Downsampler::OverSample) * 6.0f, 1.0f, speed, -10
        )
    );
}

void meta::ER1::AnalogSound::setPitch(float hz)
{
    m_Pitch = meta::limit(MIN_USER_FREQ, MAX_USER_FREQ, hz);
}

void meta::ER1::AnalogSound::setSampleRate(float newRate)
{
    BaseSound::setSampleRate(newRate);
    m_MainOsc.set_sample_rate(newRate);

    // run sample-accurate, not sub-sample accurate
    m_ModOsc.set_sample_rate(newRate / meta::ER1::Downsampler::OverSample);
}

void meta::ER1::AnalogSound::setModulationDepth(float depth)
{
    auto sign = depth >= 0 ? 1.0f : -1.0f;
    depth = meta::Interpolate<float>::parabolic(0.0f, 1.0f, std::abs(depth));
    m_ModDepth = depth * sign;
}

void meta::ER1::AnalogSound::setOscFreq(float freq)
{
    m_MainOsc.set_freq(meta::limit(20.0f, MAX_OSC_FREQ, freq));
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
