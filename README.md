# 🧸 Teddy – Game Engine (C++, OpenGL, SDL3 & ImGui)

**Teddy** is a custom game engine built with **C++**, **OpenGL**, **SDL3**, designed to make my own games while learning render APIs.  
The project began as a deep dive into engine architecture by following the foundational principles of The Cherno's Hazel engine tutorial series.
Moving beyond the initial tutorial structure, Teddy has been extensively extended with custom systems, an SDL3 backend, and a focus on tooling, making it a powerful platform for 2D development and a testbed for future 3D and multi-API work.

---

# If you want to see how it is in real use check the Cuphead branch [here](https://github.com/desantesway/Teddy/tree/cuphead), where I am remaking a Cuphead level and menus.

## 🧪 Usage (Windows)

### ⚠️ Requires VulkanSDK

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
Keyboard/Mouse/Midi IO | Batching                    |                        | Vulkan
Scene Editor           | Perspective Camera          |                        | Compression
Profiler               | Orthographic Camera         |                        | GPU profiler
Stats                  | Dynamic font rendering      |                        | 2D lights
Post Processing        | Physics                     |                        | Protection
Scene&Tex Hot Reload   | Collider rendering          |                        | 
Atlas Render&Generator |                             |                        | 
Physics                |                             |                        | 
