Component and Peripheral Mapping
================================

Overview
--------

This document describes how Webots devices are mapped to AVR peripherals
through the component bridge.

Distance Sensors -> ADC Channels
--------------------------------

Each DistanceSensorComponent:

1. Reads a Webots distance sensor value.
2. Normalizes by sensor max value.
3. Scales to AVR supply voltage in millivolts.
4. Raises the matching ADC IRQ.

Scaling relationship:

.. math::

   adc\_mv = clamp\left(\frac{sensor\_value}{sensor\_max} \times V_{cc}, 0, V_{cc}\right)

Current map:

- ds_left -> ADC0
- ds_left_front -> ADC1
- ds_front -> ADC2
- ds_right_front -> ADC3
- ds_right -> ADC4

Motors <- PWM/GPIO Pin Activity
-------------------------------

Each MotorComponent registers an IRQ callback on a configured I/O pin.
From pin transitions, it computes:

- PWM period from rising-edge to rising-edge time.
- High duration from rising-edge to falling-edge time.
- Duty cycle from high_duration / period.

When a stable PWM period is not available yet, fallback behavior uses the
current pin level as 0% or 100% duty. This avoids undefined output during
startup transients.

Timeout Handling
----------------

If no transitions are observed beyond a threshold, the component assumes
a flatline signal and resets synchronization state.

Current map:

- l_motor <- PORTB2
- r_motor <- PORTB1

Velocity vs Torque Modes
------------------------

Current motor instances are created in velocity-control mode.

- Velocity mode: duty cycle scales max motor velocity.
- Torque mode: duty cycle scales max motor torque.

Pin and Timer Expectations
--------------------------

For realistic behavior, firmware PWM pins and timers should correspond to
the declared mapping. If firmware changes pin assignments, the simulator
mapping must be updated to match.
