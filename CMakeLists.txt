#------------------------------------------------------------------------------
# ER-1 DSP library
#------------------------------------------------------------------------------
message("-- Gathering ER-1 DSP Library source files...")
file(GLOB_RECURSE ER1DSP_SOURCE src/*.cpp inc/*.h)
list(SORT ER1DSP_SOURCE)

# build the static library
add_library(er1dsp STATIC ${ER1DSP_SOURCE})
target_link_libraries(er1dsp metaphase_core)

# inc folder contains interface
target_include_directories(er1dsp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Required -fPIC for shared objects and DLLs
set_property(TARGET er1dsp PROPERTY POSITION_INDEPENDENT_CODE ON)

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
