//
// Created by mzapp on 4/10/18.
//

#pragma once

#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Noise.h>
#include <er1_dsp/TiltFilter.h>

class TiltFilterTest
    : public meta::TestBase
{
public:
    TiltFilterTest()
        : filter(48000, 1000, 5)
    {};

    void initializeTestFile(const juce::String& filename)
    {
        auto f = meta::TestHelpers::testFolder.getChildFile(filename);
        TestBase::initializeTestFile(f);
    }

    void runFilter()
    {
        juce::AudioBuffer<float> buffer(2, 48000);
        buffer.clear();
        for (int i = 0; i < 48000; i++)
        {
            auto fixed = noise.tick();
            auto sample = static_cast<float>(fixed) / static_cast<float>(fixed.maxSigned()) * 0.05;
            buffer.setSample(0,i, filter.tick(sample));
        }

        m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }

    meta::ER1::Noise noise;
    meta::ER1::TiltFilter filter;
};

TEST_F(TiltFilterTest, flat)
{
    initializeTestFile("tilt_filter_flat.wav");
    filter.setTilt(0);
    runFilter();
}

TEST_F(TiltFilterTest, boost_low)
{
    initializeTestFile("tilt_filter_boost_low.wav");
    filter.setTilt(-6);
    runFilter();
}

TEST_F(TiltFilterTest, boost_hi)
{
    initializeTestFile("tilt_filter_boost_hi.wav");
    filter.setTilt(6);
    runFilter();
}
