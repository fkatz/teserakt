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

struct Registry
{
	Registry();

	ApplicationState* applicationState;
	GameState* gameState;
	WindowState* windowState;
	CameraState* cameraState;

	CommandSystem* commandSystem;
	InputSystem* inputSystem;
	RenderSystem* renderSystem;
	PhysicsSystem* physicsSystem;
	vector<System*> systems;
};

