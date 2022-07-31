//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Constants.h>
#include "er1_dsp/sounds/AnalogSound.h"
#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include "common_constants.h"


class AnalogSoundTest
    : public meta::TestBase
{
public:
    AnalogSoundTest()
        : voice(SAMP_RATE)
    {
        voice.setWaveShape(meta::ER1::Wave::Shape::COSINE);
        voice.setDecay(0.50f);
    };

    meta::ER1::AnalogSound voice;
};


TEST_F(AnalogSoundTest, generate_simple_tone)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("simple_tone.wav"));

    voice.setModulationShape(meta::ER1::Mod::Shape::DECAY);
    voice.setWaveShape(meta::ER1::Wave::Shape::COSINE);
    voice.setModulationDepth(0.0f);
    voice.setPitch(20000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}



TEST_F(AnalogSoundTest, generate_synth_bass_drum)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("bass_drum.wav"));

    voice.setModulationShape(meta::ER1::Mod::Shape::DECAY);
    voice.setWaveShape(meta::ER1::Wave::Shape::COSINE);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(5.0f);
    voice.setPitch(40);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(AnalogSoundTest, generate_synth_bass_drum_retrigger)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("bass_drum_retrigger.wav"));

    voice.setModulationShape(meta::ER1::Mod::Shape::DECAY);
    voice.setWaveShape(meta::ER1::Wave::Shape::COSINE);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(0.5f);
    voice.setPitch(40);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 24000);
    buffer.clear();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    buffer.clear();
    voice.reset();
    voice.start();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    buffer.clear();
    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 24000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}

TEST_F(AnalogSoundTest, generate_sample_and_hold_snare)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("sah_snare.wav"));

    voice.setModulationShape(meta::ER1::Mod::Shape::SANDH);
    voice.setModulationDepth(2000.0f);
    voice.setModulationSpeed(44100.0f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}


TEST_F(AnalogSoundTest, generate_lazer_sound)
{
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("lazer_sound.wav"));

    voice.setWaveShape(meta::ER1::Wave::Shape::TRIANGLE);
    voice.setModulationShape(meta::ER1::Mod::Shape::SAW);
    voice.setModulationDepth(0.3f);
    voice.setModulationSpeed(0.45f);
    voice.setDecay(0.75f);
    voice.setPitch(1000);
    voice.reset();
    voice.start();

    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    voice.processBlockInternal(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
