#pragma once
//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/dsp/BandlimitedOsc.h>
#include "../inc/er1_dsp/Waveshapes.h"


namespace meta::ER1
{
    using BaseOsc = BandLimitedOsc<16, 16, 8>;
    using Shapes = SimpleWaveShapeFunc<>;

    class Oscillator
        : public BaseOsc
    {
    public:
        using BaseOsc::BandLimitedOsc;
        WaveShape shape;
    private:
        float wave_shape(float accumulator_state) override;
    };
}

