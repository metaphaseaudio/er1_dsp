//
// Created by Matt Zapp on 4/3/2018.
//

#pragma once

#include <vector>
#include <array>

namespace meta
{
    namespace ER1
    {
        class Delay
        {
        public:
            explicit Delay(float sampleRate);
            void processBlock(float** data, size_t samps);

            void setTempoSync(bool sync)
            {
                m_Sync = sync;
                recalculatePlayheadAdvance();
            }

            void setBPM(float bpm)
            {
                m_BPM = std::max(0.0f, bpm);
                recalculatePlayheadAdvance();
            };

            void setDepth(float depth)
            {
                m_Depth = std::min(std::max(0.0f, depth), 1.0f);
                recalculatePlayheadAdvance();
            };

            void setTime(float time);


        private:
            static constexpr std::array<float, 16> tempoFractions = {
                1.0f / 4.0f,
                1.0f / 3.0f,
                1.0f / 2.0f,
                2.0f / 3.0f,
                3.0f / 4.0f,
                1.33f, 1.5f,
                2, 2.5,
                3, 4, 5, 6, 7, 8
            };

            void recalculatePlayheadAdvance();
            bool m_Sync;
            float m_BPM;
            float m_Depth; // 0 - 100
            float m_Time;  // 5ms - 2s tempo sync'd
            float m_PlayheadAdvance;
            float m_Playhead;
            float m_SampleRate;
            std::vector<float>[2] m_Data;
        };
    }
}
