//
// Created by Matt Zapp on 4/3/2018.
//
#pragma once

namespace meta
{
    namespace ER1
    {
        class Amp
        {
        public:
            struct Params
            {
                char decay;    // 0 - 100
                char level;    // 0 - 100
                char pan;      // 0 - 100
                char lowBoost; // 0 - 100
            };
        };
    }
}
