//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Oscillator.h"


using namespace meta;
using namespace meta::ER1;


float meta::ER1::Oscillator::wave_shape(float accumulator_state)
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

    return BandLimitedOsc::wave_shape(accumulator_state);
}


