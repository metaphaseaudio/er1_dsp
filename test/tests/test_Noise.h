//
// Created by mzapp on 4/10/18.
//

#pragma once

#include <JuceHeader.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Noise.h>

class NoiseTest
    : public testing::Test
{
public:
    NoiseTest() {};

    void initializeTestFile(const juce::String& filename)
    {
        testFile = meta::ER1::TestHelpers::testFolder.getChildFile(filename);
        if (testFile.exists()) { testFile.deleteFile(); }
        testFile.create();
        m_Writer.reset(meta::AudioFileHelpers::createWriter(testFile, 48000, 2));
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

TEST_F(NoiseTest, generate_white_noise)
{
    initializeTestFile("white.wav");
    
    juce::AudioBuffer<float> buffer(2, 48000);
    buffer.clear();
    for (int i = 0; i < 48000; i++)
    {
        auto sample = noise.tick();
        buffer.setSample(0,i, static_cast<float>(sample));
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
