//
// Created by Matt on 3/24/2022.
//

#pragma once
#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Delay.h>
#include "common_constants.h"


class DelayTest
    : public meta::TestBase
{
public:
    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
    }

    void run(int samples)
    {
        constexpr int chunk_size = 10000;
        for (int mod = samples / chunk_size; --mod >= 0;)
        {
            const auto n_samps = std::min<int>(chunk_size, samples);
            juce::AudioBuffer<float> buffer(2, n_samps);
            buffer.clear();

            if (shouldImpulse)
            {
                buffer.setSample(0, 10, 1.0f);
                buffer.setSample(1, 10, 0.5);
                shouldImpulse = false;
            }

            delay.processBlock(buffer.getArrayOfWritePointers(), n_samps);
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }
    }

    bool shouldImpulse = true;
    meta::ER1::Delay delay = meta::ER1::Delay(SAMP_RATE);
};

TEST_F(DelayTest, generate_delay_tempo_sync)
{
    initializeTestFile("delay_tempo_sync.wav");
    delay.setTempoSync(true);
    delay.setBPM(150);
    delay.setDepth(0.5f);
    delay.setTime(0.0f);
    run(20000);
    delay.setTime(0.2f);
    run(100000);
}
