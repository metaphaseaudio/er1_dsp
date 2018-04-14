//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Voice.h"


meta::ER1::Voice::Voice()
    : oscPitch(250)
    , pan(0.5f)
{}

void meta::ER1::Voice::processBlock(float **data, int chans, int samps, int offset)
{
    for (int i = offset; i < samps + offset; i++)
    {
        auto p = pitchModulator.tick();
        oscillator.params.freq = oscPitch + p;
        oscillator.updateParams();
        auto o = oscillator.tick();

        auto sample = amplifier.processSample(o);

        // TODO: set things up for stereo pan
        for (int c = 0; c < chans; c++)
            { data[c][i] += sample; }
    }
}

void meta::ER1::Voice::reset()
{
    oscillator.reset();
    amplifier.reset();
    pitchModulator.reset();
}

void meta::ER1::Voice::updateParams()
{
    oscillator.updateParams();
    pitchModulator.updateParams();
    amplifier.updateParams();
}

void meta::ER1::Voice::start()
{
    amplifier.start();
    pitchModulator.start();
}

