#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct AIComponent : public Component
{
	AIComponent(nlohmann::json json) : Component(json) {
		shoots = json["shoots"].get<bool>();
		shootCooldown = json["shootCooldown"].get<int>();
		visionDistance = json["visionDistance"].get<int>();
		visionAngleStart = json["visionAngleStart"].get<int>();
		visionAngleEnd = json["visionAngleEnd"].get<int>();
		patrolDistance = json["patrolDistance"].get<int>();
		patrolOriginX = json["patrolOriginX"].get<int>();
		patrolOriginY = json["patrolOriginY"].get<int>();
	}
	bool shoots;
	int shootCooldown;
	int visionDistance;
	int targetX = 0;
	int targetY = 0;
	int visionAngleStart;
	int visionAngleEnd;
	uint64_t lastShoot = 0;
	uint64_t targetEntity = NULL;
	int patrolDistance;
	int patrolOriginX;
	int patrolOriginY;
	bool patrollingRight = false;
};