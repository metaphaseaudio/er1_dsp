//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Voice.h"

void meta::ER1::Voice::processBlock(float **data, int chans, int samps, int offset)
{
    for (int i = offset; i < samps + offset; i++)
    {
        auto p = pitchModulator.tick();
        oscillator.params.freq = params.oscPitch + p;
        oscillator.updateParams();

        auto a = ampModulator.tick();
        auto o = oscillator.tick();

        auto sample = a * o;
        // TODO: set things up for stereo pan
        for (int c = 0; c < chans; c++)
            { data[c][i] += sample; }
    }
}

void meta::ER1::Voice::reset()
{
    oscillator.reset();
    ampModulator.reset();
    pitchModulator.reset();
}

void meta::ER1::Voice::updateParams()
{
    oscillator.updateParams();
    pitchModulator.updateParams();
    ampModulator.updateParams();
}

void meta::ER1::Voice::start()
{
    ampModulator.start();
    pitchModulator.start();
}
