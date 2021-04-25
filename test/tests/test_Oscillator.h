//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>

class OscillatorTest
    : public meta::TestBase
{
public:
    OscillatorTest() { osc.setFrequency(100); };

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
            for (int i = 0; i < n_samps; i++)
            {
                auto sample = osc.tick();
                buffer.setSample(0,i, sample);
            }
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }

    }

    meta::ER1::Oscillator osc;
};

TEST_F(OscillatorTest, generate_square)
{
    initializeTestFile("square.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    runOscillator(100000);
}

TEST_F(OscillatorTest, generate_square_sync)
{
    initializeTestFile("square_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SQUARE;
    osc.sync();
    runOscillator(10000);
}


TEST_F(OscillatorTest, generate_triangle)
{
    initializeTestFile("triangle.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_triangle_sync)
{
    initializeTestFile("triangle_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::TRIANGLE;
    osc.sync();
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_sine)
{
    initializeTestFile("sine.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SINE;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_pure_sine)
{
	initializeTestFile("pure_sine.wav");
	osc.waveType = meta::ER1::Oscillator::WaveType::PURE_SINE;
	runOscillator(10000);
}

TEST_F(OscillatorTest, generate_sine_sync)
{
    initializeTestFile("sine_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SINE;
    osc.sync();
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_saw)
{
    initializeTestFile("saw.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SAW;
    runOscillator(10000);
}

TEST_F(OscillatorTest, generate_saw_sync)
{
    initializeTestFile("saw_sync.wav");
    osc.waveType = meta::ER1::Oscillator::WaveType::SAW;
    osc.sync();
    runOscillator(10000);
}
