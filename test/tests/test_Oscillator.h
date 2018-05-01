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
    OscillatorTest()
    {
        osc.setFrequency(100);
    };

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile(f));
    }

    meta::ER1::Oscillator osc;
};

TEST_F(OscillatorTest, generate_square)
{
    initializeTestFile("Square.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    
    // print one cycle
    juce::AudioBuffer<float> buffer(2, 4800);
    buffer.clear();
    for (int i = 0; i < 4800; i++)
    {
        auto sample = osc.tick();
        buffer.setSample(0,i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(OscillatorTest, generate_triangle)
{
    initializeTestFile("Triangle.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 4800);
    buffer.clear();
    for (int i = 0; i < 4800; i++)
    {
        auto sample = osc.tick();
        buffer.setSample(0,i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(OscillatorTest, generate_sine)
{
    initializeTestFile("Sine.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SINE;
    
    // print one cycle
    juce::AudioBuffer<float> buffer(2, 4800);
    buffer.clear();
    for (int i = 0; i < 4800; i++)
    {
        auto sample = osc.tick();
        buffer.setSample(0,i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(OscillatorTest, generate_saw)
{
    initializeTestFile("Saw.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SAW;

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 4800);
    buffer.clear();
    for (int i = 0; i < 4800; i++)
    {
        auto sample = osc.tick();
        buffer.setSample(0,i, sample);
    }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}