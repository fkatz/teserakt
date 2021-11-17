#pragma once
#include "System.h"
#include "RenderSystem.h"
#include "../states/ApplicationState.cpp"

using namespace std;

struct Registry;

struct DeallocatorSystem : public System
{
	DeallocatorSystem(Registry* registry);

	void onUpdate();

protected:
	Registry* registry;
};