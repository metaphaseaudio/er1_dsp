//
// Created by Mattb on 6/8/2022.
//

#include "er1_dsp/LowBoost.h"
#include "meta/util/NumericConstants.h"
#include <cmath>

meta::ER1::IIRFilter::IIRFilter()
    : xn1(0), xn2(0), yn1(0), yn2(0)
{}


float meta::ER1::IIRFilter::processSample(float in)
{
    // Calculate the output based on the existing state
    const float out = b0 * in
                    + b1 * xn1
                    + b2 * xn2
                    - a1 * yn1
                    - a2 * yn2;

    // update the existing state based on the input
    xn2 = xn1;
    xn1 = in;
    yn2 = yn1;
    yn1 = out;
    return out;
}


meta::ER1::LowBoost::LowBoost(float sampleRate, float freq, float boost, float s)
{
    setFreq(sampleRate, freq, false);
    setBoost(boost, s);
}

void meta::ER1::LowBoost::setFreq(float sampleRate, float freq, bool recalculate)
{
    const float omega = meta::NumericConstants<float>::PI * 2 * (freq / sampleRate);
    sn = sin(omega);
    cs = cos(omega);
    if (recalculate)
        { recalculateCoeffs(); }
}

void meta::ER1::LowBoost::setBoost(float boost, float s, bool recalculate)
{
    A = exp(log(10.f) * boost * 0.025f);
    beta = sqrt((A + A) + (A * A + 1) * ((1.0f / s) - 1));
    if (recalculate)
        { recalculateCoeffs(); }
}

void meta::ER1::LowBoost::recalculateCoeffs()
{
    b0 = A * ((A + 1.f) - (A - 1.f) * cs + beta * sn);
    b1 = 2.f * A * ((A - 1.f) - (A + 1.f) * cs);
    b2 = A * ((A + 1.f) - (A - 1.f) * cs - beta * sn);
    a1 = -2.f * ((A - 1.f) + (A + 1.f) * cs);
    a2 = (A + 1.f) + (A - 1.f) * cs - beta * sn;
    const float ra0 = 1.0f / ((A + 1.f) + (A - 1.f) * cs + beta * sn);

    b0 *= ra0;
    b1 *= ra0;
    b2 *= ra0;
    a1 *= ra0;
    a2 *= ra0;
}
