#pragma once
#include "Registry.h"

struct Application {
	Application();

	void run();

	Registry* registry;
};