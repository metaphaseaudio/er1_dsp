/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>
#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>

#include "tests/test_Noise.h"
#include "tests/test_SampleAndHold.h"
#include "tests/test_TiltFilter.h"
#include "tests/test_Envelope.h"
#include "tests/test_Delay.h"
#include "tests/test_AnalogSound.h"
#include "tests/test_PCMSound.h"

int main(int argc, char **argv)
{
    if (meta::TestHelpers::testFolder.exists())
        { meta::TestHelpers::testFolder.deleteRecursively(); }

    meta::TestHelpers::testFolder.createDirectory();

    testing::InitGoogleTest(&argc, argv);
    auto retval = RUN_ALL_TESTS();

    meta::TestHelpers::testFolder.deleteRecursively();
    return retval;
}
