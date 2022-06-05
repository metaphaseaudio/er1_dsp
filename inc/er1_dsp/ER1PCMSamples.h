/* (Auto-generated binary data file). */

#pragma once

#include <vector>
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"

namespace ER1PCMSamples
{
    extern const char*  clap_wav;
    const int           clap_wavSize = 60984;

    extern const char*  closed_hat_wav;
    const int           closed_hat_wavSize = 10744;

    extern const char*  crash_wav;
    const int           crash_wavSize = 91260;

    extern const char*  open_hat_wav;
    const int           open_hat_wavSize = 41536;

    static std::vector<float> loadBinaryPCMData(const char* pcm_data, const int nData)
    {
        auto stream = std::make_unique<juce::MemoryInputStream>(pcm_data, nData, false);
        juce::WavAudioFormat fmt;
        std::unique_ptr<juce::AudioFormatReader> reader(fmt.createReaderFor(stream.release(), false));
        std::vector<float> rv;
        rv.resize(reader->lengthInSamples);
        auto data = rv.data();
        reader->read(&data, 1, 0, reader->lengthInSamples);
        return rv;
    }
}
