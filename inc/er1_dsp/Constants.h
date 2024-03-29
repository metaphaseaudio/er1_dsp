#pragma once
//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/dsp/BandlimitedOsc.h>
#include <meta/dsp/OversampledBuffer.h>
#include "../inc/er1_dsp/Waveshapes.h"


namespace meta::ER1
{
    using Shapes = SimpleWaveShapeFunc<>;

// Various numbers
#ifdef _DEBUG
    static constexpr int TWO_N_OVERSAMPLE = 3;
    static constexpr int NumOutBuses = 4;
#else
    static constexpr int TWO_N_OVERSAMPLE = 4;
    static constexpr int NumOutBuses = 8;
#endif
    static constexpr int ANALOG_SOUND_COUNT = 10;
    static constexpr int SAMPLE_SOUND_COUNT = 4;
    static constexpr int AUDIO_SOUND_COUNT = 2;
    static constexpr int ER1_SOUND_COUNT = ANALOG_SOUND_COUNT + AUDIO_SOUND_COUNT + SAMPLE_SOUND_COUNT;
    using Downsampler = meta::OversampledBuffer<16, meta::static_power<2, TWO_N_OVERSAMPLE>::value, 16>;
}

