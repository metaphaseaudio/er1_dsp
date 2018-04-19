/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>

#include "TestHelpers.h"
#include "tests/test_Oscillator.h"
#include "tests/test_Noise.h"
#include "tests/test_Voice.h"

int main(int argc, char **argv)
{
    if (meta::ER1::TestHelpers::testFolder.exists())
        { meta::ER1::TestHelpers::testFolder.deleteRecursively(); }

    meta::ER1::TestHelpers::testFolder.createDirectory();

    testing::InitGoogleTest(&argc, argv);
    auto retval = RUN_ALL_TESTS();

    meta::ER1::TestHelpers::testFolder.deleteRecursively();
    return retval;
}
