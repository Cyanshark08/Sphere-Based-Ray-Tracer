# XRay — Recursive Sphere Ray Tracer with Physics

An interactive, recursive ray tracer in C++ built on [Walnut](https://github.com/TheCherno/Walnut) (Dear ImGui + Vulkan). Renders reflective spheres and an infinite ground plane with real recursive light bounces, and includes a live rigid-body physics simulation so spheres can bounce and collide in the scene while it renders.

<img width="4096" height="2160" alt="HQScreenshot" src="https://github.com/user-attachments/assets/9c1b9ee5-18cb-45a7-b49d-e4569cc1c6de" />

*High Quality Render*

https://github.com/user-attachments/assets/d5315392-4131-4fc8-b680-d02043bdf1b2

*Click to watch a demo - replace with a real recording of the interactive viewport (camera movement, sphere physics, live reflections).*

## Overview

Each pixel casts a camera ray that recursively bounces off whatever it hits — spheres or the ground plane — up to a fixed number of bounces, attenuating color contribution at each additional bounce so reflections fade out realistically rather than looping forever. On top of the renderer, spheres in the scene are live rigid bodies: they have position, radius, color, and elasticity, and a physics step updates their motion every frame (toggleable, with an adjustable time multiplier), so you can watch reflective spheres bounce and collide while the reflections update in real time.

## Features

- **Recursive ray tracing with reflections** - each ray traces up to 3 bounces off spheres and the ground plane, with each bounce's contribution attenuated by a diffusion-rate falloff so the reflection contribution decays realistically rather than staying full-strength indefinitely.
- **Live rigid-body physics** - spheres have elasticity and move/collide via a physics update step each frame, with a UI toggle and time-multiplier slider to speed up, slow down, or pause the simulation.
- **Full interactive scene editor** - add spheres on the fly; adjust each sphere's color, position, radius, and elasticity with live sliders; adjust background color and light direction while the scene renders.
- **Free camera movement** — WASD/QE axis movement with an adjustable speed, plus a one-click "Origin" reset.
- **Scene persistence with auto-save** - save/save-as/load named scene files, plus automatic background auto-saving (time- and action-count-based) so in-progress scenes aren't lost.
- **Screenshot export** - save the current viewport render, or a fixed high-quality 4096×2160 render, to a `.ppm` image file.
- **Multithreaded rendering** - parallelized per-pixel using C++17 execution policies (`std::execution::par`), togglable via a compile-time flag.

## Tech Stack

- **C++**
- **[Walnut](https://github.com/TheCherno/Walnut)** - application framework wrapping Dear ImGui and Vulkan for windowing, layers, and image display
- **Dear ImGui** (via Walnut) - all runtime UI (Settings, Objects, Viewport, Misc panels)
- Important Custom engine components: `Renderer`, `Scene`, `RigidSphere`, `Ray`, `Vec3`, under the `XRay` namespace

## Architecture

- `ExampleLayer` - the application layer: renders all ImGui panels (camera/background/light controls, per-sphere property sliders, screenshot/save/load buttons), drives the physics update each frame, and triggers the renderer.
- `XRay::Renderer` - builds a camera ray per pixel and recursively traces it (see pipeline below); also handles resizing, viewport-resolution and fixed high-quality PPM screenshot export, and both single- and multi-threaded render paths.
- `XRay::Scene` - holds the list of `RigidSphere` objects in the scene.
- `XRay::Sphere` - base geometry and material: center, radius, and color, an analytic ray-sphere intersection (solving the quadratic for the near root), and direct (non-shadowed) Lambertian shading based on the surface normal and light direction. Inherits from a common `Entity` base.
- `XRay::RigidSphere` - (not directly inspected, but referenced from the UI/physics code) appears to extend `Sphere` with physics properties (elasticity) and the per-frame physics update (`s_UpdatePhysics`) that drives sphere motion and collisions.
- `XRay::Vec3` / `XRay::Ray` - vector math and ray representation used throughout.

### Rendering pipeline (per pixel, per bounce)
1. Build a camera ray through the pixel, correctly mapping raster row/column to normalized device coordinates (accounting for aspect ratio and vertical flip so image-top corresponds to scene-up).
2. Test the ray against every sphere in the scene; if none are hit, test against an infinite ground plane.
3. On a plane hit: reflect the ray off the plane normal and continue bouncing; on a miss entirely, return the background color attenuated by the current bounce depth.
4. On a sphere hit: compute the surface normal at the intersection, shade it via the sphere's material and the scene light direction, and reflect the ray for the next bounce.
5. Recurse up to a fixed bounce limit (3), adding each subsequent bounce's contribution scaled by a diffusion-rate falloff so later bounces matter less than the primary hit.

## Requirements & Build

This project depends on Walnut, which requires Vulkan. Follow [Walnut's setup instructions](https://github.com/TheCherno/Walnut) to install the Vulkan SDK and get the framework building, then add this project's source files as an application layer per Walnut's app-template structure.

## Usage

| Input | Action |
|---|---|
| W / A / S / D | Move camera along Z / X axes |
| Q / E | Move camera along Y axis |
| "Origin" button | Reset camera to a fixed starting position |
| Background/Light sliders | Adjust scene background color and light direction live |
| "Add Sphere" | Add a new sphere to the scene |
| Per-object sliders | Adjust each sphere's color, position, radius, and elasticity |
| "Time Multiplier" slider | Speed up/slow down/pause the physics simulation |
| "Screenshot" / "High-Quality Screenshot" | Export the current render to a `.ppm` file (viewport resolution or fixed 4096×2160) |
| "Save Scene" / "Save Scene As" / "Load Scene" / "New Scene" | Manage scene files, with automatic background auto-saving |

## Notes
- Screenshot export writes `.ppm` files, which aren't natively viewable in most image viewers or browsers — convert to PNG (e.g. with ImageMagick: `magick Screenshot.ppm Screenshot.png`) before using one as the high-resolution demo image above.
