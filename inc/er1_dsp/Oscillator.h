#pragma once
//
// Created by Matt Zapp on 4/3/2018.
//

#include <meta/dsp/BandlimitedOsc.h>
#include "../inc/er1_dsp/Waveshapes.h"


namespace meta::ER1
{
    using Shapes = SimpleWaveShapeFunc<>;

    template<size_t bit_depth, size_t sub_samples, size_t blip_resolution = 8>
    class Oscillator
        : public BandLimitedOsc<bit_depth, sub_samples, blip_resolution>
    {
    public:
        using BaseOsc = BandLimitedOsc<bit_depth, sub_samples, blip_resolution>;
        using BandLimitedOsc<bit_depth, sub_samples, blip_resolution>::BandLimitedOsc;
        WaveShape shape;
    private:
        float wave_shape(float accumulator_state) override
        {
            constexpr auto scale_factor =  BaseOsc::Min * -1;
            const auto scaled = accumulator_state / scale_factor;
            switch (shape)
            {
                case WaveShape::SINE:
                    return Shapes ::sin(scaled) * scale_factor;
                case WaveShape::TRIANGLE:
                    return Shapes::tri(scaled) * scale_factor;
                case WaveShape::SQUARE:
                    return Shapes::square(scaled) * scale_factor;
                case WaveShape::SAW:
                    return accumulator_state;
                case INVERSE_SAW:
                    return Shapes::inv_saw(accumulator_state);
            }

            return BaseOsc::wave_shape(accumulator_state);
        }
    };

    using MainOscillator = Oscillator<12, 16, 12>;
    using Modulator = Oscillator<12, 4, 8>;  // Modulators can be of lower quality
}

