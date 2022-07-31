//
// Created by Mattb on 5/26/2022.
//

#pragma once
#include "Delay.h"

static float S = 0.75;
static float F = 1000;

namespace meta:: ER1
{
    class Channel
    {
    public:
        explicit Channel(float pan=0.0f, float level=1.0f, float sampleRate=48000);
        void processBlock(const float* inData, float** outData, int nSamps, int offset);

        void setSampleRate(float sr);

        void setPan(float x);
        void setLevel(float x);

        void setTempo(float bpm);
        void setTempoSync(bool sync);
        void setDelayTime(float time);
        void setDelayDepth(float depth);
        void setAccentGain(float vol);

        void reset();

    private:
        float halfRoot2, leftAmp, rightAmp;
        float level, boostGain, accentGain;
        meta::ER1::Delay m_Delay;
    };
}