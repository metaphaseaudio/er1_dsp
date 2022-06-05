//
// Created by Mattb on 6/4/2022.
//

#pragma once
#include "BaseSound.h"


namespace meta::ER1
{
    class PCMSound
        : public BaseSound
    {
    public:
        PCMSound(float sampleRate, std::vector<float> data);
        void processBlock(float* data, const float* ringData, int samps, int offset) override;
        void setSampleRate(float newRate) override;
        void start() override;

    private:
        std::vector<float> m_PCMData;
        meta::LoopingAccumulator m_Accum;
    };
}
