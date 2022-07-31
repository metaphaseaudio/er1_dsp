//
// Created by Matt Zapp on 5/22/2018.
//

//Code :
////=======================
//// tilt eq settings
////
//// srate -> sample rate
//// f0 -> 20-20khz
//// gain -> -6 / +6 db
////=======================
//amp = 6/log(2);
//denorm = 10^-30;
//pi = 22/7;
//sr3 = 3*srate;
//
//// conditition:
//// gfactor is the proportional gain
////
//gfactor = 5;
//if (gain > 0) {
//g1 = -gfactor*gain;
//g2 = gain;
//} else {
//g1 = -gain;
//g2 = gfactor*gain;
//};
//
////two separate gains
//lgain = exp(g1/amp)-1;
//hgain = exp(g2/amp)-1;
//



#pragma once

#include <meta/dsp/OnePoleLowPassFilter.h>
#include <meta/audio/SingletonSampleRate.h>

namespace meta::ER1
{
    class TiltFilter
    {
    public:
        TiltFilter(float sampleRate, float centerFreq, float gFactor)
            : m_GFactor(gFactor)
        {
            setFreq(sampleRate, centerFreq);
        };

        void setFreq(float sampleRate, float centerFreq)
        {
            const auto omega = meta::NumericConstants<float>::TWO_PI * centerFreq;
            const auto s3 = sampleRate * 3.0f;
            const auto n = 1.0f / (s3 + omega);
            m_A0 = 2.0f * omega * n;
            m_B1 = (s3 - omega) * n;
            reset();
        }

        void reset()
        {
            m_LPOut = 0.0f;
        }

        void setTilt(float gain)
        {
            const auto amp = 6.0f / std::log(2.0f);
            float g1, g2;
            if (m_GFactor > 0)
            {
                g1 = -m_GFactor * gain;
                g2 = gain;
            }
            else
            {
                g1 = -gain;
                g2 = m_GFactor * gain;
            }

            m_LoGain = std::exp(g1 / amp) - 1;
            m_HiGain = std::exp(g2 / amp) - 1;
        }

        float tick(float in)
        {
            m_LPOut = m_A0 * in + m_B1 * m_LPOut;
            return in + m_LoGain * m_LPOut + m_HiGain * (in - m_LPOut);
        }

    private:
        float m_LoGain, m_HiGain, m_A0, m_B1, m_GFactor, m_LPOut;
    };
}
