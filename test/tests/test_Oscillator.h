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
        osc.params.freq = 100;
    };

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile(f));
    }

    virtual void TearDown()
    {
        m_Writer->flush();
        m_Writer.reset(nullptr);
    }

    meta::ER1::Oscillator osc;
};

TEST_F(OscillatorTest, generate_square)
{
    initializeTestFile("Square.wav");
    osc.params.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    osc.updateParams();

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
    osc.params.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;
    osc.updateParams();

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
    osc.params.waveType = meta::ER1::Oscillator::WaveType::SINE;
    osc.updateParams();

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
    osc.params.waveType = meta::ER1::Oscillator::WaveType::SAW;
    osc.updateParams();

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