//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include <meta/dsp/BandlimitedOsc.h>
#include "Noise.h"
#include "Envelope.h"
#include "Oscillator.h"
#include "SampleAndHold.h"
#include "meta/util/Countdown.h"

namespace meta:: ER1
{
    /**
     * Comprises one oscillator and two modulators (amp and pitch) with the
     * associated handling code that allows them to be treated as a single
     * ER-1 voice.
     */
    class AnalogVoice
//        : ER1::MainOscillator
    {
    public:
        explicit AnalogVoice(float sampleRate);

        /// fill an array of floats with data from the voice
        void processBlock(float* data, const float* ringData, int samps, int offset);

        /// silence the voice
        void reset();

        /// trigger the voice
        void start();

        enum ModShape
        {
            SINE = 0,
            TRIANGLE,
            SQUARE,
            SAW,
            INVERSE_SAW,
            SANDH,
            NOISE,
            DECAY,
        };

        void setSampleRate(float sampleRate);
        void setWaveShape(ER1::WaveShape waveType);
        void setPitch(float freq);
        void setModulationShape(ModShape type);
        void setModulationSpeed(float speed);
        void setModulationDepth(float depth);
        void setDecay(float time);
        [[nodiscard]] bool hasEnded() const noexcept { return m_Env.hasEnded(); }

    private:
        void tickMod();
        float tick();

        float wave_shape(WaveShape shape, float accumulator_state);
        void setOscFreq(float freq);

        float sampleRate;
        float pitch;
        float m_ModDepth;
        float m_LastNoise;
        float m_LastMix;

        WaveShape m_Shape = WaveShape::COSINE;
        ModShape m_ModType = ModShape::DECAY;
        LoopingAccumulator m_MainOsc;
        LoopingAccumulator m_ModOsc;
        meta::ER1::SampleAndHold m_SAH;
        meta::ER1::Envelope m_ModEnv;
        meta::ER1::Envelope m_Env;
        meta::Countdown<MainOscillator::OverSample> m_SampleCounter;
        static meta::ER1::Noise m_Noise;
    };
}