#pragma once
#include "System.h"
#include "../states/GameState.cpp"

struct Registry;

struct PhysicsSystem :
	public System
{
	PhysicsSystem(Registry* registry);

	void onUpdate();

	void collide(int currentX, int currentY, int futureX, int futureY, std::vector<entityId>& intersectingEntities, const entityId& entity, float& lastX, float& lastY, bool& intersected, bool shrinkX, bool shrinkY);

	unsigned long long int lastUpdateTime;

protected:
	Registry* registry;
private:
	bool futureIntersects(entityId, entityId, int, int, bool, bool);
};

