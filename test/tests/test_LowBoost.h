//
// Created by Mattb on 6/8/2022.
//

#pragma once

#include <er1_dsp/LowBoost.h>
#include <er1_dsp/Noise.h>

class LowBoostTest
        : public testing::Test
{
public:
    static constexpr float sample_rate = 44100;
    LowBoostTest() {};

    void initializeTestFile(const juce::String& filename)
    {
        testFile = meta::TestHelpers::testFolder.getChildFile(filename);
        if (testFile.exists()) { testFile.deleteFile(); }
        testFile.create();
        m_Writer.reset(meta::AudioFileHelpers::createWriter(testFile, LowBoostTest::sample_rate, 2));
    }

    virtual void TearDown()
    {
        m_Writer->flush();
        m_Writer.reset(nullptr);
    }

    juce::File testFile;
    meta::ER1::Noise noise;
    std::unique_ptr<juce::AudioFormatWriter> m_Writer;
};

TEST_F(LowBoostTest, generate_white_noise_with_boost)
{
    initializeTestFile("white_noise_boost.wav");
    auto boost = meta::ER1::LowBoost(LowBoostTest::sample_rate, 150, 20);

    juce::AudioBuffer<float> buffer(2, LowBoostTest::sample_rate);
    buffer.clear();
    for (int i = 0; i < LowBoostTest::sample_rate; i++)
    {
        auto sample = static_cast<float>(noise.tick() / meta::ER1::fixed_t::maxSigned()) * 0.25;
        buffer.setSample(0,i, sample);
        sample = boost.processSample(sample);
        buffer.setSample(1,i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
