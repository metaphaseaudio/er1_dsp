//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <JuceHeader.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>
#include <er1_dsp/Voice.h>
#include <meta/util/testing/TestBase.h>

class VoiceTest
    : public meta::TestBase
{
public:
    VoiceTest()
    {
        voice.oscillator.waveType = meta::ER1::Oscillator::WaveType::SINE;
        voice.level = 1.0f;
        voice.envelope.setSpeed(1.0f);
    };

    meta::ER1::Voice voice;
};

TEST_F(VoiceTest, generate_synth_bass_drum)
{
    initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile("bass_drum.wav"));

    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(1.5f);
    voice.setPitch(20);
    voice.reset();
    voice.start();

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 2, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

TEST_F(VoiceTest, generate_sample_and_hold_snare)
{
    initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile("sah_snare.wav"));

    voice.setModulationType(meta::ER1::Voice::SANDH);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(44100.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 2, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
