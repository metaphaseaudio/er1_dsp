//
// Created by Mattb on 6/4/2022.
//

#include "er1_dsp/sounds/PCMSound.h"

meta::ER1::PCMSound::PCMSound(float sampleRate, std::vector<float> data)
    : BaseSound(sampleRate)
    , m_PCMData(std::move(data))
    , m_Accum(0, m_PCMData.size(), sampleRate, 1.0f)
{}

void meta::ER1::PCMSound::processBlock(float* data, const float* ringData, int samps, int offset)
{
    for (int s = 0; s < samps; s++)
    {
        const auto table_size = m_PCMData.size();
        auto phase = m_Accum.tick();
        auto i = static_cast<int>(phase) % table_size;
        auto f = phase - static_cast<int>(phase);
        auto j = (i == table_size - 1) ? 0 : i + 1;
        data[s + offset] = meta::Interpolate<float>::linear(m_PCMData[i], m_PCMData[j], f) * m_Env.tick();
    }
}

void meta::ER1::PCMSound::setSampleRate(float newRate)
{
    BaseSound::setSampleRate(newRate);
    m_Accum.set_sample_rate(newRate);
    m_Accum.set_freq(1.0f);
}

void meta::ER1::PCMSound::start()
{
    BaseSound::start();
    m_Accum.sync(0.0f);
    m_Accum.set_end(m_PCMData.size());
}
