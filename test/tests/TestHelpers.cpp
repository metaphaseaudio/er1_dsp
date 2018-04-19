//
// Created by mzapp on 4/18/18.
//

#include "TestHelpers.h"

const juce::File meta::ER1::TestHelpers::testFolder
    = juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)
                 .getChildFile("er1_audio_tests");
