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
            for (int i = 0; i < n_samps; i++)
            {
                auto sample = envelope.tick();
                buffer.setSample(0, i, sample);
                buffer.setSample(1, i, sample);
            }
            delay.processBlock(buffer.getArrayOfWritePointers(), n_samps);
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }

    }

    meta::ER1::Delay delay = meta::ER1::Delay(SAMP_RATE);
    meta::ER1::Envelope envelope;
};

TEST_F(DelayTest, generate_delay_tempo_sync)
{
    initializeTestFile("delay_tempo_sync.wav");
    envelope.setSpeed(SAMP_RATE, 10);
    envelope.start();
    delay.setTempoSync(true);
    delay.setBPM(120);
    delay.setDepth(0.5);
    delay.setTime(0.5f);
    run(1000000);
}
