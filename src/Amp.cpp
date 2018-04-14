//
// Created by Matt Zapp on 4/3/2018.
//

#include "../inc/er1_dsp/Amp.h"

void meta::ER1::Amp::reset() { envelope.reset(); }

void meta::ER1::Amp::start() { envelope.start(); }

void meta::ER1::Amp::updateParams() { envelope.updateParams(); }
