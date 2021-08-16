#pragma once
#include "System.h"
#include "RenderSystem.h"
#include "../states/ApplicationState.cpp"

using namespace std;

struct Registry;

struct InputSystem : public System
{
	InputSystem(Registry* registry);

	void onUpdate();

protected:
	Registry* registry;
};