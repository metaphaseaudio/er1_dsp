//
// Created by mzapp on 4/10/18.
//

#pragma once

#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/util/fixed_point/Math.h>
#include <er1_dsp/SampleAndHold.h>
#include <er1_dsp/Types.h>

class SampleAndHoldTest
    : public meta::TestBase
{
public:
    void initializeTestFile(const juce::String& f)
        { meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f)); }

    meta::ER1::Noise noise;
    meta::ER1::SampleAndHold sah;
};

TEST_F(SampleAndHoldTest, pass_noise)
{
    initializeTestFile("pass_noise.wav");

    juce::AudioBuffer<float> buffer(2, 48000);
    sah.setSampleRate(48000);

    buffer.clear();
    sah.setResetCount(0);

    for (int i = 0; i < 48000; i++)
    {
        auto sample = sah.tick(noise.tick()) * 0.25f;
        buffer.setSample(0, i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

TEST_F(SampleAndHoldTest, single_value)
{
    juce::AudioBuffer<float> buffer(2, 4800);
    buffer.clear();
    sah.setResetCount(4800);

    for (int i = 0; i < 4800; i++)
    {
        auto sample = sah.tick(noise.tick());
        buffer.setSample(0,i, static_cast<float>(sample));
    }

    auto max = juce::FloatVectorOperations::findMaximum(buffer.getArrayOfReadPointers()[0], 4800);
    auto min = juce::FloatVectorOperations::findMinimum(buffer.getArrayOfReadPointers()[0], 4800);
    ASSERT_EQ(max, min);
}
