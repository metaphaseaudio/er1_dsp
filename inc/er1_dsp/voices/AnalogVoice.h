//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include "BaseVoice.h"
#include "../Noise.h"
#include "../Envelope.h"
#include "../Constants.h"
#include "../SampleAndHold.h"
#include "meta/util/Countdown.h"

namespace meta:: ER1
{
    /**
     * Comprises one oscillator and two modulators (amp and pitch) with the
     * associated handling code that allows them to be treated as a single
     * ER-1 voice.
     */
    class AnalogVoice
        : public Voice
    {
    public:
        explicit AnalogVoice(float sampleRate);

        /// fill an array of floats with data from the voice
        void processBlock(float* data, const float* ringData, int samps, int offset) override;
        void reset() override;
        void start() override;

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

        void setSampleRate(float sampleRate) override;
        void setWaveShape(ER1::WaveShape waveType);
        void setPitch(float freq);
        void setModulationShape(ModShape type);
        void setModulationSpeed(float speed);
        void setModulationDepth(float depth);

    private:
        void tickMod();
        float tick();

        float wave_shape(WaveShape shape, float accumulator_state);
        void setOscFreq(float freq);

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
        meta::Countdown<MainOscillator::OverSample> m_SampleCounter;
        static meta::ER1::Noise m_Noise;
    };
}
