//
// Created by Matt on 5/16/2021.
//

#pragma once
#include <cmath>
#include <meta/dsp/BandlimitedWavetable.h>


namespace meta::ER1
{
    enum WaveShape
    {
        COSINE = 0,
        TRIANGLE,
        SQUARE,
        SAW,
        INVERSE_SAW,
    };

    template<typename NumType=float, size_t table_size=256>
    struct SimpleWaveShapeFunc
    {
        static constexpr auto cosin_table = BandlimitedWavetable<NumType, table_size>::makeCosin();

        static constexpr NumType saw(NumType x)
            { return x; };
        static constexpr NumType inv_saw(NumType x)
            { return -x; };
        static constexpr NumType square(NumType x)
            { return NumType(x > 0 ? 1 : -1); };
        static constexpr NumType tri(NumType x)
            { return NumType((std::abs(x) * 2.0f) - 1.0f); };
        static constexpr NumType cosin(NumType x)
        {
            const auto real_i = (x / 2.0f + 0.5f)  * (table_size - 1);

            const size_t i = real_i;
            const size_t j = (i == table_size - 1) ? 0 : i + 1;

            const auto f = real_i - NumType(i);
            const auto a = cosin_table.at(i);
            const auto b = cosin_table.at(j);
            return (a * (1 - f)) + (b * f);
        };
    };
}


