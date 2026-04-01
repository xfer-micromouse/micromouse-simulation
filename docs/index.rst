Micromouse Simulator Documentation
==================================

This documentation describes the micromouse simulator architecture, how
simavr is used for realistic microcontroller behavior, and how to build,
run, test, and extend the project.

The simulator executes compiled AVR firmware (ELF) inside simavr while
Webots models the robot and environment. Device components bridge those
two worlds so the control logic runs as embedded firmware instead of a
desktop-only mock.

.. container:: important-notice

   **IMPORTANT**

   This is not a polished or finished project. There may be bugs, and it
   requires hands-on work.

   If you want to work on it, improve it, fix issues, or create new
   components, contributions are welcome.

.. toctree::
   :maxdepth: 2
   :caption: Contents

   overview
   quickstart
   architecture
   simulation_loop
   component_mapping
   creating_components
   firmware_workflow
   build_and_run
   development_roadmap
