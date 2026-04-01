Firmware Workflow with simavr
=============================

Firmware Build Target
---------------------

The embedded project under:

- controllers/mms_controller/arduino_src/arduino_mms_sim

is configured for PlatformIO environment:

- env: nanoatmega328
- platform: atmelavr
- board: nanoatmega328
- framework: arduino

This produces an ELF image suitable for simavr execution.

Execution Path
--------------

The simulator performs these steps:

1. Read ELF firmware into simavr firmware structure.
2. Create ATmega328P virtual MCU instance.
3. Initialize MCU and load firmware.
4. Set clock and supply voltage.
5. Register UART output callback.

At runtime, firmware runs exactly as AVR code, not as recompiled desktop
logic. Accesses to ADC, GPIO, timers, and UART operate through simavr.

Current Firmware Location Assumption
------------------------------------

The current controller source initializes the simulator with a fixed,
absolute path to firmware.elf. This is reliable on one machine but not
portable across environments.

Recommended improvement path:

- Read firmware path from CLI argument or environment variable.
- Fallback to a relative path from the controller directory.
- Validate path and emit clear error diagnostics on startup.

UART Logging Semantics
----------------------

The UART0 TX IRQ callback writes each transmitted byte to stdout.
This provides low-friction firmware tracing and helps debug control loop
timing, sensor calibration, and state-machine transitions.

Firmware Design Guidance for Simulation Fidelity
------------------------------------------------

To preserve hardware realism:

- Prefer timer-driven control loops over delay-heavy logic.
- Keep ADC sampling strategy explicit and consistent.
- Separate sensing, estimation, and actuation timing where possible.
- Emit compact UART diagnostics with timestamps or cycle counters.
