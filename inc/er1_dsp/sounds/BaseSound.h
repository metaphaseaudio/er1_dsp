//
// Created by Mattb on 6/2/2022.
//

#pragma once

#include "../Envelope.h"
#include "../LowBoost.h"
#include "../TiltFilter.h"
#include "meta/audio/decibels.h"

namespace meta::ER1
{
    class BaseSound
    {
    public:
        explicit BaseSound(float sampleRate, float decay=1.0f)
            : sampleRate(sampleRate)
            , m_LowBoost(0.0f)
            , m_TiltFilter(sampleRate, F, 5)
        { setDecay(decay); }

        virtual ~BaseSound() = default;

        void processBlock(float* data, const float* ringData, int samps, int offset)
        {
            processBlockInternal(data, ringData, samps, offset);

            for (int s = offset; s < samps + offset; s++)
            {
                auto x = m_TiltFilter.tick(data[s]) * m_BoostGain;
                x = (2.0f / meta::NumericConstants<float>::PI) * atan(x);
                data[s] = x;
            }
        }

        void setDecay(float time)
        {
            m_Env.setSpeed(sampleRate, meta::Interpolate<float>::parabolic(30, 0.05, time, -5));
        };

        virtual void setSampleRate(float newRate)
        {
            sampleRate = newRate;
            m_Env.setSampleRate(sampleRate);
            m_TiltFilter.setFreq(sampleRate, F);
        };

        /// silence the voice
        virtual void reset()
        {
            m_Env.reset(sampleRate);
            m_TiltFilter.reset();
        };

        /// trigger the voice
        virtual void start()
        {
            m_Env.start();
            for (auto sound : m_ChokeList) { sound->reset(); }
        };

        /// Indicates whether or not the envelope has reached 0.
        [[nodiscard]] float getEnvValue() const noexcept { return m_Env.getValue(); }
        [[nodiscard]] bool hasEnded() const noexcept { return m_Env.hasEnded(); }

        virtual void setWaveShape(Wave::Shape waveType) {};
        virtual void setPitch(float freq) {};
        virtual void setModulationShape(Mod::Shape type) {};
        virtual void setModulationSpeed(float speed) {};
        virtual void setModulationDepth(float depth) {};

        void setLowBoost(float x)
        {
            m_TiltFilter.setTilt(-6 * x);
            m_BoostGain = meta::db_to_gain_coeff(-6 * std::abs(x));
        };

        enum AudioChannel
        {
            NONE = 0,
            LEFT,
            RIGHT
        };

        [[nodiscard]] virtual AudioChannel wantsAudioChannel() const { return NONE; }
        void addSoundToChokeList(BaseSound* other) { m_ChokeList.push_back(other); };

    protected:
        virtual void processBlockInternal(float* data, const float* ringData, int samps, int offset) = 0;
        float sampleRate;
        meta::ER1::Envelope m_Env;

    private:
        static constexpr float F = 1000;
        float m_LowBoost, m_BoostGain;
        meta::ER1::TiltFilter m_TiltFilter;
        std::vector<BaseSound*> m_ChokeList;
    };
}