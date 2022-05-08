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
    using MainOscillator = BandLimitedOsc<16, 16, 8, 2>;
    using Downsampler = meta::OversampledBuffer<16, 16, 8, 2>;
}

