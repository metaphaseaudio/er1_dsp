message("-- Building ER-1 DSP Test Suite")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE ER1DSP_TEST_SHARED_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
                                            ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
list(SORT ER1DSP_TEST_SHARED_SOURCE)

add_executable(run_er1dsp_tests ${ER1DSP_TEST_SHARED_SOURCE} AllTests.cpp)
target_include_directories(run_er1dsp_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests)

target_link_libraries(run_er1dsp_tests
        er1dsp ${GTEST_LIBRARIES} ${JUCE_LIBS} ${META_SHARED_LIBS}
        juce
        metaphase_core)