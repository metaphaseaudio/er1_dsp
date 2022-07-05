//
// Created by Mattb on 6/4/2022.
//

#include "er1_dsp/sounds/PCMSound.h"

meta::ER1::PCMSound::PCMSound(float sampleRate, std::vector<float> data, float dataSampleRate)
    : BaseSound(sampleRate)
    , m_Hz(1.0f)
    , m_DataSampleRate(dataSampleRate)
    , m_PCMData(std::move(data))
    , m_Delta(0.0f)
    , m_Phase(0.0f)
{
    recalculateDeltas();
}

void meta::ER1::PCMSound::processBlock(float* data, const float* ringData, int samps, int offset)
{
    for (int s = 0; s < samps; s++)
    {
        const auto envVal = m_Env.tick();
        if (m_Phase >= m_PCMData.size())
        {
            data[s + offset] = 0.0f;
            continue;
        }

        const auto table_size = m_PCMData.size();
        auto i = static_cast<int>(m_Phase) % table_size;
        auto f = m_Phase - static_cast<int>(m_Phase);
        auto j = (i == table_size - 1) ? 0 : i + 1;
        data[s + offset] = meta::Interpolate<float>::linear(m_PCMData[i], m_PCMData[j], f) * envVal;
        m_Phase += m_Delta;
    }
}

void meta::ER1::PCMSound::setSampleRate(float newRate)
{
    BaseSound::setSampleRate(newRate);
    recalculateDeltas();
}

void meta::ER1::PCMSound::start()
{
    BaseSound::start();
    m_Phase = 0.0f;
}

void meta::ER1::PCMSound::setPitch(float freq)
{
    m_Hz = freq;
    recalculateDeltas();
}

void meta::ER1::PCMSound::recalculateDeltas()
{
    m_Delta = m_Hz * m_DataSampleRate / sampleRate;
}

