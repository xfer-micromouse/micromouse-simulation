Build and Run Guide
===================

Host Dependencies
-----------------

At minimum, install:

- Webots and controller headers/libraries
- simavr headers and libraries
- libelf
- CMake and C++17-capable compiler
- PlatformIO

Controller Build Details
------------------------

The CMake configuration in controllers/mms_controller does the following:

- Builds all C and C++ files in that directory.
- Adds include paths for Webots and simavr.
- Links against Webots controller libraries, simavr, simavrparts, and elf.
- Copies built controller binary to source directory after build.

If Webots is installed in a non-default location, update WEBOTS_HOME or
the relevant include/link directories accordingly.

Build Commands
--------------

Firmware:

.. code-block:: bash

   cd controllers/mms_controller/arduino_src/arduino_mms_sim
   pio run

Controller:

.. code-block:: bash

   cd controllers/mms_controller
   cmake -S . -B build
   cmake --build build

Run in Webots
-------------

1. Open worlds/mms-sim.wbt in Webots.
2. Ensure robot controller points to mms_controller binary.
3. Start simulation and monitor controller console.

Common Build Problems
---------------------

Missing simavr headers:

- Verify include directory contains simavr/sim_avr.h.

Missing simavr library:

- Ensure libsimavr and libsimavrparts are installed where CMake searches.

ELF load failure at runtime:

- Confirm firmware build succeeded and expected ELF path exists.
