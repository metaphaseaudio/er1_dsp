//
// Created by Mattb on 6/5/2022.
//

#pragma once
#include <meta/util/file/AudioFileHelpers.h>
#include <er1_dsp/Constants.h>
#include "er1_dsp/sounds/PCMSound.h"
#include "er1_dsp/ER1PCMSamples.h"
#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include "common_constants.h"


class PCMSoundTest
    : public meta::TestBase
{
public:
    std::unique_ptr<meta::ER1::PCMSound> sound;
};

TEST_F(PCMSoundTest, closed_hat_48kHz)
{
    const float sampleRate = 48000;
    initializeTestFile(meta::TestHelpers::testFolder.getChildFile("closed_hat.wav"), sampleRate, 1);

    sound = std::make_unique<meta::ER1::PCMSound>(
        sampleRate, ER1PCMSamples::loadBinaryPCMData(ER1PCMSamples::closed_hat_wav, ER1PCMSamples::closed_hat_wavSize), 32000
    );

    sound->start();
    juce::AudioBuffer<float> buffer(2, 96000);
    buffer.clear();

    sound->processBlock(buffer.getArrayOfWritePointers()[0], nullptr, 96000, 0);
    m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}
