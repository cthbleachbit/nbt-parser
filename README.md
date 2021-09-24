nbt-parser
==========

A parser library of NBT file format used in the game Minecraft.

### Building

Building is handled by CMake. This project uses C++20 features and may require G++ 10 or higher to successfully compile.

You'll need the following dependencies:

* `fmtlib` - required, handles formatting of human readable messages.
* `python3` and `pybind11` - optional, builds python bindings for NBTP.
* `boost_test` - optional, only if you want to run unit tests.