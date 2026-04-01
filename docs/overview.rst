Project Overview
================

Purpose
-------

This project provides a realistic micromouse simulation workflow where
firmware intended for an AVR microcontroller is executed in simavr while
Webots provides the robot physics and sensors.

The core objective is to reduce the gap between simulated behavior and
on-device behavior by preserving the embedded software execution model:

- Firmware is compiled for ATmega328P and loaded from an ELF image.
- CPU cycles advance in a controlled way relative to Webots time steps.
- ADC and digital I/O interactions are passed through simavr IRQ lines.
- Motor commands are inferred from microcontroller pin activity.

Why simavr Instead of Native Controller Logic
---------------------------------------------

Traditional simulation setups often run control logic as native desktop
code. That is useful for algorithm prototyping but can hide embedded
constraints such as:

- Timer and interrupt behavior
- Fixed-frequency execution assumptions
- Peripheral access patterns
- UART diagnostics and serial timing side effects

By using simavr, the simulator can execute the same compiled firmware
artifact that is expected to run on hardware, making simulation a more
reliable verification stage.

Repository Areas
----------------

The main areas used in this workflow are:

- controllers/mms_controller: C++ simulator bridge and component model.
- controllers/mms_controller/arduino_src/arduino_mms_sim: PlatformIO
  firmware project targeting ATmega328.
- worlds/mms-sim.wbt: Webots world with robot and named devices.
