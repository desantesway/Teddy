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

## 🎮 Functionality

### ✅ 2D Features

- ✔️ Scenes + Entity Component System (ECS)
- ✔️ Sprite rendering with texture batching
- ✔️ AABB collision detection & response
- ✔️ Real-time physics simulation (velocity, gravity, restitution)
- ✔️ Keyboard, mouse, and MIDI input abstraction
- ✔️ ImGui-powered scene editor and live inspector
- ✔️ Save/load scenes with YAML serialization
- ✔️ Runtime debug visualizations (bounding boxes, quad number, etc.)

---

### 🚧 Coming Soon

- 🔜 Vulkan backend (in progress)
- 🔜 Audio engine (SDL Audio)
- 🔜 Scene transitions and asset streaming

---

## 🛠️ Technologies

- **Language:** C++
- **Rendering:** OpenGL (with backend abstraction)
- **Window/Input:** SDL3
- **UI:** ImGui
- **Build System:** CMake
- **Serialization:** JSON (nlohmann/json)

---

## ⭐️ Star this project if you're into custom engine development!
