#pragma once

// To use on Teddy applications

// --- Core ------------------------------
#include "Teddy/Core/Base.h"
#include "Teddy/Core/Application.h"
#include "Teddy/Core/Layer.h"
#include "Teddy/Core/Log.h"
#include "Teddy/Core/Assert.h"
#include "Teddy/Core/Timestep.h"
#include "Teddy/Core/Input.h"
#include "Teddy/Core/KeyCodes.h"
#include "Teddy/Core/MouseCodes.h"
#include "Teddy/Core/MidiCodes.h"
// ------------------------------------------

// --- ImGuiLayer ------------------------------
#include "Teddy/ImGui/ImGuiLayer.h"
// ------------------------------------------

// --- Scene ------------------------------
#include "Teddy/Scene/Scene.h"
#include "Teddy/Scene/Entity.h"
#include "Teddy/Scene/ScriptableEntity.h"
#include "Teddy/Scene/Components.h"
#include "Teddy/Scene/SceneSerializer.h"
// ------------------------------------------

// --- Renderer ------------------------------
#include "Teddy/Renderer/Renderer.h"
#include "Teddy/Renderer/Renderer2D.h"
#include "Teddy/Renderer/RenderCommand.h"
#include "Teddy/Renderer/Buffer.h"
#include "Teddy/Renderer/Shader.h"
#include "Teddy/Renderer/Framebuffer.h"
#include "Teddy/Renderer/Texture.h"
#include "Teddy/Renderer/VertexArray.h"
// ------------------------------------------

// --- Post Processing ------------------------------
#include "Teddy/PostProcessing/PostProcessing.h"
// ------------------------------------------

// --- Utils ------------------------------
#include "Teddy/Utils/PlatformUtils.h"
#include "Teddy/Utils/FileWatcher.h"
// ------------------------------------------

// --- Image ------------------------------
#include "Teddy/Image/Atlas.h"
// ------------------------------------------