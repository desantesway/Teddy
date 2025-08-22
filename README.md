# 🧸 Teddy – Game Engine (C++, OpenGL, SDL3 & ImGui)

**Teddy** is a custom game engine built with **C++**, **OpenGL**, **SDL3**, designed to make my own games while learning render APIs.  
It provides a modern, modular foundation for both 2D and future 3D game development — with extensibility for Vulkan, Metal, and DirectX.

---

## 🧪 Usage

1. Clone the repository
```bash
  git clone https://github.com/desantesway/Teddy.git
```

2. Get all submodules in Scripts/Win-UpdateGitSubmodules.bat

3. Build the project in Scripts/Win-GenerateProject.bat

4. Open VS-build/Teddy.sln to run the Visual Studio solution

---

## 🚧 Motivation

Teddy was created after hitting performance and flexibility ceilings in the [KeyStar-in-SDL-renderer](https://github.com/desantesway/KeyStar-in-SDL-renderer) project.  
The goal: Build a **scalable, low-level game engine** to enable:

- Deep performance optimizations
- Cross-platform rendering backends (OpenGL now, Vulkan/Metal/DirectX planned)
- A robust editor for scene management and debugging
- Full control over input, physics, rendering, and game loop timing

---

## 🧩 Architecture Highlights

- 🧱 **Graphics Abstraction Layer** – clean interface to swap or extend rendering backends
- 🎹 **Multi-source Input Handling** – supports keyboard, mouse, and MIDI input
- ⚙️ **Custom Physics Simulation** – deterministic 2D physics with debug visualization
- 📝 **Dynamic Text Rendering** – for UI and runtime data display
- 🧰 **ImGui-based Editor** – live entity management, scene loading/saving, physics debugging, real-time graphs and metrics

---

## 🎮 Features

Engine                 | 2D                          | 3D                     | Coming
-------------          | -------------               | -------------          | -------------
Scenes Serialization   | Quad/Circle/Line rendering  | Perspective Camera     | Fustrum Culling
ECS                    | Textures                    |                        | Audio Engine
Physics                | Collider rendering          |                        | Protection
Keyboard IO            | Batching                    |                        | Vulkan
Mouse IO               | Perspective Camera          |                        | Compression
Midi IO                | Orthographic Camera         |                        | Mip Mapping
Scene editor           | Dynamic font rendering      |                        | Better profiler
Profiler               |                             |                        | 2D lights
Stats                  |                             |                        | ...
