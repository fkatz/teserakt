#pragma once
#include "System.h"
#include "../states/GameState.cpp"

struct Registry;

struct PhysicsSystem :
	public System
{
	PhysicsSystem(Registry* registry);

	void onUpdate();

	void collide(float currentX, float currentY, float futureX, float futureY, std::vector<entityId>& intersectingEntities, const entityId& entity, float& lastX, float& lastY, bool& intersected);

	unsigned long long int lastUpdateTime;

protected:
	Registry* registry;
private:
	bool futureIntersects(entityId, entityId, float, float);
};

