Architecture
============

High-Level View
---------------

The runtime architecture is composed of four layers:

1. Webots world and robot devices
2. C++ simulator bridge controller
3. simavr microcontroller emulation
4. AVR firmware binary (ELF)

Data flows from Webots sensors into AVR peripherals before CPU execution,
then actuator outputs are propagated from AVR pin state back into Webots.

Core Classes and Responsibilities
---------------------------------

Simulator
~~~~~~~~~

Defined in the controller source, this class owns:

- AVR simulator lifecycle
- Webots Robot instance
- Component registration and initialization
- Time-step synchronized simulation loop

IComponent Interface
~~~~~~~~~~~~~~~~~~~~

Component abstraction includes:

- init(robot, avr): attach to both simulation backends
- update_avr(): push Webots-side state into AVR peripherals
- update_webots(): push AVR-side outputs into Webots actuators

DistanceSensorComponent
~~~~~~~~~~~~~~~~~~~~~~~

- Reads Webots distance sensor values.
- Scales sensor reading to millivolts based on sensor max value.
- Raises the corresponding ADC IRQ in simavr.

MotorComponent
~~~~~~~~~~~~~~

- Subscribes to AVR GPIO pin transitions via IRQ callback.
- Estimates PWM duty cycle from edge timing in AVR cycle units.
- Applies duty cycle to motor velocity (or torque in non-velocity mode).

Device Registration
-------------------

Current registration in the simulator bridge:

- Distance sensors:
  - ds_left -> ADC0
  - ds_left_front -> ADC1
  - ds_front -> ADC2
  - ds_right_front -> ADC3
  - ds_right -> ADC4
- Motors:
  - l_motor -> PORTB pin 2
  - r_motor -> PORTB pin 1

Microcontroller Profile
-----------------------

The AVR model is configured as:

- MCU: atmega328p
- CPU clock: 16 MHz
- Vcc: 3300 mV

This matches common micromouse control-board assumptions and keeps ADC
conversion scaling physically interpretable in the simulator.
