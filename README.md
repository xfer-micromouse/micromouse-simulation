# Micromouse Simulator (Webots + simavr)

This project simulates a micromouse robot in Webots while running real AVR firmware in simavr.

<div style="border:2px solid #b00020; border-left:8px solid #b00020; border-radius:6px; background:#ffe9ec; color:#4a0009; padding:12px; margin:16px 0;">
  <strong>IMPORTANT</strong><br/>
  This is not a polished or finished project. There may be bugs, and it requires hands-on work.<br/><br/>
  If you want to work on it, improve it, fix issues, or create new components, contributions are welcome.
</div>

## What This Project Does

- Runs AVR firmware (ATmega328P) from an ELF file through simavr
- Connects Webots sensors and motors to simulated MCU peripherals
- Lets you test embedded control logic with realistic microcontroller behavior

## Quick Start

1. Build firmware:

   ```bash
   cd controllers/mms_controller/arduino_src/arduino_mms_sim
   pio run
   ```

2. Build controller:

   ```bash
   cd controllers/mms_controller
   cmake -S . -B build
   cmake --build build
   ```

3. Open Webots world and run:

- `worlds/mms-sim.wbt`

## Documentation

Detailed Sphinx docs are in the `docs/` directory and configured for Read the Docs using `readthedocs.yml`.

## GitHub Docs Integration (Auto Publish on Push)

This repository now includes a GitHub Actions workflow that builds and deploys docs to GitHub Pages on push to `main` or `master`:

- `.github/workflows/docs-pages.yml`

One-time setup in your GitHub repository:

1. Go to **Settings -> Pages**.
2. Set **Source** to **GitHub Actions**.
3. Push to `main` (or run the workflow manually from **Actions**).

After deployment, docs will be available at:

- `https://<your-username>.github.io/<your-repo>/`
