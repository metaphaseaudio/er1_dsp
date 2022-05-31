//
// Created by Mattb on 5/26/2022.
//

#include "../inc/er1_dsp/Channel.h"


meta::ER1::Channel::Channel(float pan, float level, float sampleRate)
    : pan(pan)
    , level(level)
    , m_Delay(sampleRate)
{}

void meta::ER1::Channel::processBlock(const float* inData, float** outData, int nSamps, int offset)
{
    for (int s = 0; s < nSamps; s++)
    {
        const auto sample = inData[s] * level;
        const auto l = sample * (1.0f - pan);
        const auto r = sample * pan;
        const auto value = m_Delay.tick(l, r);

        if (outData == nullptr) { continue; }
        outData[0][s + offset] += value[0]; // level application might be down here...
        outData[1][s + offset] += value[1];
    }
}

void meta::ER1::Channel::setPan(float x) { pan = std::max(std::min(x, 1.0f), -1.0f); }
void meta::ER1::Channel::setLevel(float x) { level = std::max(std::min(x, 1.0f), 0.0f); }

void meta::ER1::Channel::setSampleRate(float sr) { m_Delay.setSampleRate(sr); }
void meta::ER1::Channel::setTempo(float bpm) { m_Delay.setBPM(bpm); }
void meta::ER1::Channel::setDelayTime(float time) { m_Delay.setTime(time); }
void meta::ER1::Channel::setDelayDepth(float depth) { m_Delay.setDepth(depth); }
void meta::ER1::Channel::setTempoSync(bool sync) { m_Delay.setTempoSync(sync); }


