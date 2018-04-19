//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <JuceHeader.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>

class OscillatorTest
    : public testing::Test
{
public:
    OscillatorTest()
    {
        osc.params.freq = 100;
    };

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
    meta::ER1::Oscillator osc;
    std::unique_ptr<juce::AudioFormatWriter> m_Writer;
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