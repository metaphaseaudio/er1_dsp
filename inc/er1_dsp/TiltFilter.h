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
////filter
//omega = 2*pi*f0;
//n = 1/(sr3 + omega);
//a0 = 2*omega*n;
//b1 = (sr3 - omega)*n;
//
////==================================
//// sample loop
//// in -> input sample
//// out -> output sample
////==================================
//lp_out = a0*in + b1*lp_out;
//out = in + lgain*lp_out + hgain*(in - lp_out);


#pragma once

#include <meta/dsp/OnePoleLowPassFilter.h>
#include <meta/audio/SingletonSampleRate.h>

namespace meta
{
    namespace ER1
    {
        class TiltFilter
        {
        public:
            void setCutoff(float freq)
				{ m_Filter.setCutoff(meta::SingletonSampleRate<float>::getValue(), freq); };

            void setTilt(float factor)
            {
                m_LoGain = factor;
                m_HiGain = 1.0f - factor;
            }

            float tick(float in)
            {
                const auto lp = m_Filter.processSample(in);
				return lp;// in + (m_LoGain * lp) + (m_HiGain * (in - lp));
            }

        private:
            meta::OnePoleLowPassFilter m_Filter;
            float m_LoGain, m_HiGain;
        };
    }
}
