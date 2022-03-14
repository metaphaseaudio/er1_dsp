//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>
#include "common_constants.h"


class OscillatorTest
    : public meta::TestBase
{
public:
    OscillatorTest()
        : osc(SAMP_RATE)
    {
        osc.set_freq(8);
    };

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
    }

    void runOscillator(int samples)
    {
        constexpr int chunk_size = 10000;
        for (int mod = samples / chunk_size; --mod >= 0;)
        {
            const auto n_samps = std::min<int>(chunk_size, samples);
            juce::AudioBuffer<float> buffer(1, n_samps);
            buffer.clear();
            osc.processBlock(buffer.getWritePointer(0), buffer.getNumSamples());
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }

    }

    meta::ER1::Oscillator osc;
};

TEST_F(OscillatorTest, generate_square)
{
    initializeTestFile("square.wav");
    osc.shape = meta::ER1::WaveShape::SQUARE;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_square_sync)
{
    initializeTestFile("square_sync.wav");
    osc.shape = meta::ER1::WaveShape::SQUARE;
    osc.sync(1.0f);
    runOscillator(10000);
}


TEST_F(OscillatorTest, generate_triangle)
{
    initializeTestFile("triangle.wav");
    osc.shape = meta::ER1::WaveShape::TRIANGLE;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_triangle_sync)
{
    initializeTestFile("triangle_sync.wav");
    osc.shape = meta::ER1::WaveShape::TRIANGLE;
    osc.sync(1.0f);
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_sine)
{
    initializeTestFile("sine.wav");
    osc.shape = meta::ER1::WaveShape::SINE;
    runOscillator(10000);
}


TEST_F(OscillatorTest, generate_sine_sync)
{
    initializeTestFile("sine_sync.wav");
    osc.shape = meta::ER1::WaveShape::SINE;
    osc.sync(1.0f);
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_saw)
{
    initializeTestFile("saw.wav");
    osc.shape = meta::ER1::WaveShape::SAW;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_saw_sync)
{
    initializeTestFile("saw_sync.wav");
    osc.shape = meta::ER1::WaveShape::SAW;
    osc.sync(1.0f);
    runOscillator(10000);
}
