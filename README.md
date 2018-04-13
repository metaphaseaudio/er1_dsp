# ER-1 DSP Library
This library contains code that can replicate the virtual analog synthesis used
by the Korg ER-1.  It is not guaranteed to sound exactly like an ER-1, the code
used in the machines is thus far closed source, but the synthesis techniques
chosen for this library were selected and refined based on spectral and
waveform analysis of sounds generated from an original Mk1 ER-1.

### Requirements:
  - The [Metaphase Core](https://github.com/metaphaseaudio/metaphase_core)
library
  - [Google Test](https://github.com/google/googletest) for unit testing

### Build:
This library is designed to be as self-contained as possible, but it is still
recommended that one use the [MUDS](https://github.com/metaphaseaudio/MUDS)
project structure when including this this library into a project.  If a MUDS-
style project is used, [CMake](https://cmake.org) becomes a requirement.

**NOTE:** The Metaphase Core library requires
[JUCE](https://github.com/WeAreROLI/JUCE) to build, but the ER-1 DSP library
does not itself depend on JUCE directly or otherwise.  This is done to allow
this library to be compiled and linked for hardware that may not support
traditional JUCE-supported operating systems.

To build unit tests, pass `-DBUILD_TESTS=ON` to the build script. In this case
JUCE will be required to build as many of the Metaphase Core testing tools rely
on JUCE to simplify basic operations such as audio file format handling.

