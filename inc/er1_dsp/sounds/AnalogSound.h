//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

#include "BaseSound.h"
#include "../Noise.h"
#include "../Envelope.h"
#include "../Constants.h"
#include "../SampleAndHold.h"
#include "meta/util/Countdown.h"

namespace meta:: ER1
{
    /**
     * Comprises one oscillator and two modulators (amp and m_Pitch) with the
     * associated handling code that allows them to be treated as a single
     * ER-1 voice.
     */
    class AnalogSound
        : public BaseSound
    {
    public:
        explicit AnalogSound(float sampleRate);

        /// fill an array of floats with data from the voice
        void processBlock(float* data, const float* ringData, int samps, int offset) override;
        void reset() override;
        void start() override;


        void setSampleRate(float sampleRate) override;
        void setWaveShape(ER1::Wave::Shape waveType) override;
        void setPitch(float freq) override;
        void setModulationShape(ER1::Mod::Shape type) override;
        void setModulationSpeed(float speed) override;
        void setModulationDepth(float depth) override;

    private:
        void tickMod();
        float tick();

        float wave_shape(Wave::Shape shape, float accumulator_state);
        void setOscFreq(float freq);

        float m_Pitch;
        float m_ModDepth;
        float m_LastNoise;
        float m_LastMix;

        Wave::Shape m_Shape = Wave::Shape::COSINE;
        Mod::Shape m_ModType = Mod::Shape::DECAY;
        LoopingAccumulator m_MainOsc;
        LoopingAccumulator m_ModOsc;
        meta::ER1::SampleAndHold m_SAH;
        meta::ER1::Envelope m_ModEnv;
        meta::Countdown<MainOscillator::OverSample> m_SampleCounter;
        static meta::ER1::Noise m_Noise;
    };
}
