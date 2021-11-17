#pragma once
#include "systems/CommandSystem.h"
#include "systems/InputSystem.h"
#include "systems/RenderSystem.h"
#include "systems/System.h"
#include "states/ApplicationState.cpp"
#include "states/GameState.cpp"
#include "states/WindowState.cpp"
#include "systems/PhysicsSystem.h"
#include "states/CameraState.cpp"
#include "systems/AISystem.h"
#include "systems/DeallocatorSystem.h"

struct Registry
{
	Registry();

	ApplicationState* applicationState;
	GameState* gameState;
	WindowState* windowState;
	CameraState* cameraState;

	InputSystem* inputSystem;
	AISystem* aiSystem;
	CommandSystem* commandSystem;
	RenderSystem* renderSystem;
	PhysicsSystem* physicsSystem;
	DeallocatorSystem* deallocatorSystem;
	vector<System*> systems;
};

