#pragma once
#include "System.h"
#include "../states/WindowState.cpp"
#include "../states/GameState.cpp"

struct Registry;

struct RenderSystem : public System
{
	RenderSystem(Registry* registry);

	void onUpdate();

	void onQuit();

protected:
	Registry* registry;
};