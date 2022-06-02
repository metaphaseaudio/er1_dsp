//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Constants.h>
#include "er1_dsp/voices/AnalogVoice.h"
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
        voice.setWaveShape(meta::ER1::WaveShape::COSINE);
        voice.setDecay(0.50f);
    };

    meta::ER1::AnalogVoice voice;
};

TEST_F(VoiceTest, generate_synth_bass_drum)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("bass_drum.wav"));

    voice.setModulationShape(meta::ER1::AnalogVoice::DECAY);
    voice.setWaveShape(meta::ER1::WaveShape::COSINE);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(5.0f);
    voice.setPitch(40);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(VoiceTest, generate_synth_bass_drum_retrigger)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("bass_drum_retrigger.wav"));

    voice.setModulationShape(meta::ER1::AnalogVoice::DECAY);
    voice.setWaveShape(meta::ER1::WaveShape::COSINE);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(5.0f);
    voice.setPitch(40);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 24000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    buffer.clear();
    voice.reset();
    voice.start();

    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    buffer.clear();
    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

TEST_F(VoiceTest, generate_sample_and_hold_snare)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("sah_snare.wav"));

    voice.setModulationShape(meta::ER1::AnalogVoice::SANDH);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(44100.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(VoiceTest, generate_lazer_sound)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("lazer_sound.wav"));

    voice.setWaveShape(meta::ER1::WaveShape::TRIANGLE);
    voice.setModulationShape(meta::ER1::AnalogVoice::SAW);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(15.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
