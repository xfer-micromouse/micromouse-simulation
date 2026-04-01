Quickstart
==========

This section gives the shortest path from source code to a running
firmware-in-the-loop simulation.

Prerequisites
-------------

Install the following on Linux:

- Webots (matching the world/controller setup)
- CMake and a C++ compiler
- simavr development headers and libraries
- libelf development package
- PlatformIO CLI

Typical package names (distribution dependent):

- cmake
- g++
- libsimavr-dev (or equivalent)
- libelf-dev
- python3-pip

Build Firmware (ATmega328)
--------------------------

From the firmware project directory:

.. code-block:: bash

   cd controllers/mms_controller/arduino_src/arduino_mms_sim
   pio run

Expected output artifact:

- .pio/build/nanoatmega328/firmware.elf

Build Webots Controller
-----------------------

From the controller directory:

.. code-block:: bash

   cd controllers/mms_controller
   mkdir -p build
   cd build
   cmake ..
   cmake --build .

The controller binary is copied to the source directory by the existing
post-build CMake rule.

Run Simulation
--------------

Open the world:

- worlds/mms-sim.wbt

Run the Webots simulation. The simulator controller initializes simavr,
loads the ELF firmware, and starts synchronized execution.

Check Serial Output
-------------------

UART0 TX is connected to a callback that writes bytes to stdout. If your
firmware uses Serial.print/println on UART0, logs should appear in the
controller output stream.
