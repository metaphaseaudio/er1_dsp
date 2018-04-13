//
// Created by Matt Zapp on 4/3/2018.
//

#pragma once
namespace meta
{
    namespace ER1
    {
        class Delay
        {
        public:
            struct Params
            {
                char delayDepth; // 0 - 100
                char delayTime;  // 5ms - 2s
            };
        };
    }
}
