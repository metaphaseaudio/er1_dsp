//
// Created by Matt on 5/16/2021.
//

#pragma once
#include <cmath>
#include <meta/dsp/BandlimitedWavetable.h>


namespace meta
{
    namespace ER1
    {
        enum WaveShape
        {
            SAW,
            INVERSE_SAW,
            SQUARE,
            TRIANGLE,
            SINE
        };

        template<typename NumType=float, size_t table_size=256>
        struct SimpleWaveShapeFunc
        {
            static constexpr auto sin_table = BandlimitedWavetable<NumType, table_size>::makeSin();

            static constexpr NumType saw(NumType x) { return x; };
            static constexpr NumType inv_saw(NumType x) { return -x; };
            static constexpr NumType square(NumType x) { return x > 0 ? 1 : -1; };
            static constexpr NumType tri(NumType x) { return (std::abs(x) * 2.0f) - 1.0f; };

            static constexpr NumType sin(NumType x)
            {
                const auto real_i = ((x + 1.0) / 2.0)  * (table_size);

                const size_t i = real_i;
                const size_t j = (i == table_size - 1) ? 0 : i + 1;

                const auto f = real_i - NumType(i);
                const auto a = sin_table.at(i);
                const auto b = sin_table.at(j);
                return (a * (1 - f)) + (b * f);
            };
        };
    }
}


