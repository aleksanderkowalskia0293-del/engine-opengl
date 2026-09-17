# engine-opengl

A small 3D engine written in C using OpenGL for rendering, with a simple platform layer and basic audio support.

This project demonstrates a minimal real-time graphics setup: window creation, OpenGL function loading, shader usage, textured meshes, camera movement, and sound playback.

## Features

- OpenGL 3.x rendering
- Custom shader loading for vertex and fragment stages
- 3D cube and plane mesh generation
- Basic camera controller for first-person navigation
- PNG texture loading
- WAV audio playback
- Cross-platform windowing support for Linux and Windows
- Simple build pipeline via Makefile

## Project structure

- `src/` — engine source files, platform code, rendering, audio, math, and utilities
- `shaders/` — GLSL shaders used by the demo
- `assets/` — generated or bundled runtime assets such as textures and sound
- `tools/` — asset generation utility
- `Makefile` / `NMakefile` — build scripts

## Requirements

### Linux

- GCC or Clang
- OpenGL development libraries
- X11 development libraries
- ALSA development libraries
- libpng

Typical Debian/Ubuntu install:

```bash
sudo apt-get install build-essential libx11-dev libgl1-mesa-dev libasound2-dev libpng-dev
```

### Windows

- MSVC or MinGW
- OpenGL libraries
- libpng (for example via vcpkg)

## Build

From the repository root:

```bash
make
```

This builds the engine binary into `bin/engine` (or `bin/engine.exe` on Windows).

## Generate assets

The project generates required runtime assets before running:

```bash
make assets
```

This creates the `assets/` content used by the demo, including the checker texture and audio beep.

## Run

```bash
make run
```

Or run the generated binary directly:

```bash
./bin/engine
```

## Clean

```bash
make clean
```

## Notes

- The engine expects to be launched from the repository root so relative asset and shader paths resolve correctly.
- The demo loads `shaders/basic.vert` and `shaders/basic.frag` and renders a textured cube over a ground plane.
- Audio will play the `assets/beep.wav` sample when the scene updates or on startup.

## License

This project includes a license file at `LICENSE.txt`.

## Credits

This project is a lightweight OpenGL engine demo built around a custom C rendering pipeline and basic cross-platform abstractions.
