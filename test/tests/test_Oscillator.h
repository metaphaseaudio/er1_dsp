//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <JuceHeader.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>

class OscillatorTest
    : public meta::TestBase
{
public:
    OscillatorTest() { osc.setFrequency(100); };

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile(f));
    }

    void runOscillator()
    {
        juce::AudioBuffer<float> buffer(2, 4800);
        buffer.clear();
        for (int i = 0; i < 4800; i++)
        {
            auto sample = osc.tick();
            buffer.setSample(0,i, sample);
        }

        m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }

    meta::ER1::Oscillator osc;
};

TEST_F(OscillatorTest, generate_square)
{
    initializeTestFile("square.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    runOscillator();
}

TEST_F(OscillatorTest, generate_square_sync)
{
    initializeTestFile("square_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    osc.sync();
    runOscillator();
}


TEST_F(OscillatorTest, generate_triangle)
{
    initializeTestFile("triangle.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;
    runOscillator();
}

TEST_F(OscillatorTest, generate_triangle_sync)
{
    initializeTestFile("triangle_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;
    osc.sync();
    runOscillator();
}

TEST_F(OscillatorTest, generate_sine)
{
    initializeTestFile("sine.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SINE;
    runOscillator();
}

TEST_F(OscillatorTest, generate_sine_sync)
{
    initializeTestFile("sine_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SINE;
    osc.sync();
    runOscillator();
}

TEST_F(OscillatorTest, generate_saw)
{
    initializeTestFile("saw.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SAW;
    runOscillator();
}

TEST_F(OscillatorTest, generate_saw_sync)
{
    initializeTestFile("saw_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SAW;
    osc.sync();
    runOscillator();
}