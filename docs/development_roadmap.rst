Development Roadmap
===================

Near-Term Improvements
----------------------

- Replace absolute firmware path with configurable runtime argument.
- Add configuration file for sensor and motor mappings.
- Add optional structured logging for ADC and PWM debug channels.

Mid-Term Improvements
---------------------

- Add encoder simulation or wheel odometry feedback path.
- Model sensor noise, nonlinearity, and latency.
- Add deterministic replay mode for regression analysis.

Long-Term Improvements
----------------------

- Support multiple MCU targets and board profiles.
- Add CI pipeline for firmware build plus simulation smoke tests.
- Introduce automated benchmark worlds and score metrics.

Documentation Expansion Ideas
-----------------------------

- Firmware API contract for pin/channel expectations.
- Tuning guide for wall-following and maze-solving controllers.
- Performance profiling guide with timing budget methodology.
