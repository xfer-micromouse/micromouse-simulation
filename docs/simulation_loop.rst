Simulation Timing and Execution Model
=====================================

Why Timing Synchronization Matters
----------------------------------

Embedded behavior is sensitive to timing. If firmware is advanced too
slowly or too quickly relative to simulated physics, control logic can
look stable in simulation but fail on hardware.

Cycle Budget per Webots Step
----------------------------

For each Webots step:

1. Compute step duration in seconds from basic time step.
2. Convert to AVR cycle budget:

   .. math::

      cycles\_per\_step = step\_seconds \times f_{cpu}

3. Run simavr until the cycle counter reaches the target.

Given the current configuration, :math:`f_{cpu} = 16\,\mathrm{MHz}`.

Per-Step Sequence
-----------------

The simulator loop executes this order:

1. Component update_avr for all components
2. AVR execution until target cycle
3. Component update_webots for all components

This ordering gives a clear sample-and-hold model:

- Sensor state is sampled and provided to firmware.
- Firmware computes outputs for one synchronized cycle window.
- Actuation is applied to Webots after firmware execution.

UART Behavior
-------------

The controller registers a UART0 TX callback in simavr. Every transmitted
byte is forwarded to stdout. This enables firmware diagnostics in a way
that mirrors hardware serial logs.

Determinism Notes
-----------------

The model is mostly deterministic when:

- Webots world and physics settings are unchanged.
- Firmware binary is unchanged.
- Component mapping and timing configuration remain fixed.

Potential sources of divergence include floating-point effects in sensor
scaling, host performance under heavy load, and asynchronous external
inputs if added later.
