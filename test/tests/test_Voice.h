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
    {}; 

    virtual void TearDown()
    {
        m_Writer->flush();
        m_Writer.reset(nullptr);
        if (testFile.exists()) { testFile.deleteFile(); }
    }
};

TEST_F(VoiceTest, generate_synth_bass_drum)
{
    initializeTestFile(meta::ER1::TestHelpers::testFolder.getChildFile("bass_drum.wav"));

    meta::ER1::Voice voice;
    voice.oscillator.waveType = meta::ER1::Oscillator::WaveType::SINE;
    voice.level = 1.0f;
    voice.envelope.setSpeed(1.0f);
    voice.setModulationDepth(200.0f);
    voice.setModulationSpeed(1.5f);
    voice.pitch = 20;
    voice.reset();
    voice.start();

    // print one cycle
    juce::AudioBuffer<float> buffer(2, 480000);
    buffer.clear();

    voice.processBlock(buffer.getArrayOfWritePointers(), 2, 480000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
