//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>
#include <er1_dsp/Voice.h>
#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include "common_constants.h"


class VoiceTest
    : public meta::TestBase
{
public:
    VoiceTest()
        : voice(SAMP_RATE)
    {
        voice.setWaveType(meta::ER1::WaveShape::SINE);
        voice.level = 1.0f;
        voice.setDecay(0.50f);
    };

    meta::ER1::Voice voice;
};

TEST_F(VoiceTest, generate_synth_bass_drum)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("bass_drum.wav"));

    voice.setModulationType(meta::ER1::Voice::DECAY);
    voice.setWaveType(meta::ER1::WaveShape::SINE);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(5.0f);
    voice.setPitch(40);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 48000, 0);
    voice.processBlock(buffer.getArrayOfWritePointers(), 48000, 48000);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

TEST_F(VoiceTest, generate_sample_and_hold_snare)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("sah_snare.wav"));

    voice.setModulationType(meta::ER1::Voice::SANDH);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(44100.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(VoiceTest, generate_lazer_sound)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("lazer_sound.wav"));

    voice.setModulationType(meta::ER1::Voice::SAW);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(15.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
