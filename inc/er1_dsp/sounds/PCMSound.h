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
        PCMSound(float sampleRate, std::vector<float> data, float dataSampleRate);
        void processBlockInternal(float* data, const float* ringData, int samps, int offset) override;
        void setSampleRate(float newRate) override;
        void setPitch(float freq) override;
        void start() override;

    private:
        void recalculateDeltas();

        float m_Hz, m_DataSampleRate, m_Delta, m_Phase;
        std::vector<float> m_PCMData;
    };
}
