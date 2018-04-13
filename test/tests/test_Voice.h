//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <JuceHeader.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Oscillator.h>
#include <er1_dsp/Voice.h>

class VoiceTest
    : public testing::Test
{
public:
    VoiceTest()
        : testFile(juce::File::getSpecialLocation
                  (juce::File::SpecialLocationType::tempDirectory)
                  .getChildFile("er1voicetest.wav"))
    {
        if (testFile.exists()) { testFile.deleteFile(); }
        testFile.create();
        m_Writer.reset(meta::AudioFileHelpers::createWriter(testFile, 48000, 2));
    };

    virtual void TearDown()
    {
        m_Writer->flush();
        m_Writer.reset(nullptr);
        if (testFile.exists()) { testFile.deleteFile(); }
    }

    juce::File testFile;
    std::unique_ptr<juce::AudioFormatWriter> m_Writer;
};

TEST_F(VoiceTest, generate_no_pitch_modulation)
{
    meta::ER1::Voice voice;
    voice.oscillator.params.waveType = meta::ER1::Oscillator::WaveType::SINE;
    voice.ampModulator.params.modAmp = 1.0f;
    voice.ampModulator.params.modSpeed = 1.0f;
    voice.pitchModulator.params.modAmp = 200.0f;
    voice.pitchModulator.params.modSpeed = 1.5f;
    voice.params.oscPitch = 20;
    voice.updateParams();
    voice.reset();
    voice.start();

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 480000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 2, 480000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
