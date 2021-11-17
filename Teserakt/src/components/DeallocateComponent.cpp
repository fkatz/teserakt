#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
#include <SDL.h>
using namespace std;

struct DeallocateComponent : public Component
{
	DeallocateComponent() {
		createdAt = SDL_GetTicks();
	}
	DeallocateComponent(nlohmann::json json) : Component(json) {
		clearAfter = json["clearAfter"].get<int>();
		clearOnMaxQuantity = json["clearOnMaxQuantity"].get<bool>();
		createdAt = SDL_GetTicks();
	}
	int clearAfter = 0;
	bool clearOnMaxQuantity = false;
	uint32_t createdAt;
	bool forceDeallocate = false;
};