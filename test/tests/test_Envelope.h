//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Envelope.h>
#include "common_constants.h"


class EnvelopeTest
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
            juce::AudioBuffer<float> buffer(1, n_samps);
            buffer.clear();
            for (int i = 0; i < n_samps; i++)
            {
                auto sample = envelope.tick();
                buffer.setSample(0,i, sample);
            }
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }

    }

    meta::ER1::Envelope envelope;
};

TEST_F(EnvelopeTest, generate_envelope_short)
{
    initializeTestFile("short_envelope.wav");
    envelope.setSpeed(SAMP_RATE, 10);
    envelope.start();
    run(100000);
}
