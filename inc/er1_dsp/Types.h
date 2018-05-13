//
// Created by Matt Zapp on 5/8/2018.
//

#pragma once

#include <meta/util/fixed_point/Value.h>
#include <cstdint>

namespace meta
{
    namespace ER1
    {
        using fixedBase_t = int32_t;
        using fixed_t = FixedPointValue<fixedBase_t, 16>;
    }
}