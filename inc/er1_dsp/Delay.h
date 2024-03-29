//
// Created by Matt Zapp on 4/3/2018.
//

#pragma once

#include <vector>
#include <array>
#include "meta/audio/LoopingAccumulator.h"


namespace meta::ER1
{
    class Delay
    {
    public:
        explicit Delay(float sampleRate);
        void reset();

        std::array<float, 2> tick(float left, float right);
        void setTempoSync(bool sync);
        void setBPM(float bpm);
        void setDepth(float depth);
        void setTime(float time);
        void setSampleRate(float sr);

        float getDelaySamps() const { return m_DelaySampsTarget; }

    private:
        static constexpr std::array<float, 16> tempoFractions = {
            1.0f / 4.0f,
            1.0f / 3.0f,
            1.0f / 2.0f,
            2.0f / 3.0f,
            3.0f / 4.0f,
            1, 1.33f, 1.5f,
            2, 2.5,
            3, 4, 5, 6, 7, 8
        };

        void recalculateDelaySamps(bool hard=false);

        bool m_Sync;
        float m_BPM;
        float m_Depth; // 0 - 100
        float m_Time;  // 5ms - 2s tempo sync'd
        float m_SampleRate;
        float m_DelaySampsTarget;
        float m_DelaySampsCurrent;
        float m_Playhead;
        int m_Writehead;
        std::vector<float> m_Data[2];
    };
}
