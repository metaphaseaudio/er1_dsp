//
// Created by Matt Zapp on 5/8/2018.
//

#pragma once

//TODO: GET RID OF JUCE IN THIS FREAKIN' LIBRARY ALREADY!
#include <JuceHeader.h>

#include <meta/util/BitMask.h>
#include <meta/util/fixed_point/Math.h>
#include "Types.h"
#include "Noise.h"

namespace meta
{
    namespace ER1
    {
        class SampleAndHold
        {
        public:
            SampleAndHold();

            void setResetCount(uint32_t count) 
			{
				m_ResetCount = count; 
			};

            void start(fixed_t value) { m_Value = value; };

            /// Produce the next sample of the waveform
            fixed_t tick(fixed_t in);

        private:
            fixed_t m_Value;
            uint32_t m_Count;
            uint32_t m_ResetCount;
        };
    }
};


