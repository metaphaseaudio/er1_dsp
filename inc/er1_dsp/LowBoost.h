//
// Created by Mattb on 6/8/2022.
//

#pragma once

/**
 * This whole section is heavily inspired by
 * https://github.com/mrbid/Borg-ER-2/blob/main/Borg2/Filters.h
 */

namespace meta::ER1
{
    struct IIRFilter
    {
        IIRFilter();

        float processSample(float in);

        void reset();

        float xn1, xn2, yn1, yn2;
        float b0, b1, b2, a1, a2;
    };


    class LowBoost
        : public IIRFilter
    {
    public:
        LowBoost(float sampleRate, float freq, float boost, float s);
        void setFreq(float sampleRate, float freq, bool recalculate = true);
        void setBoost(float boost, float s, bool recalculate = true);

    private:
        void recalculateCoeffs();
        float A, beta, cs, sn;
    };
}
