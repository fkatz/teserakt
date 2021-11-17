#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct PhysicsComponent : public Component
{
	PhysicsComponent(nlohmann::json json) : Component(json) {
		hasGravity = json["hasGravity"].get<bool>();
		collides = json["collides"].get<bool>();
		collidable = json["collidable"].get<bool>();
		friction = json["friction"].get<float>();
	}
	bool hasGravity;
	bool collides;
	bool collidable;
	double friction;
	int vx = 0;
	int vy = 0;
	bool isStanding = false;
	uint64_t standingOnEntity = 0;
	bool isWallJumping = false;
	uint64_t wallJumpingEntity = 0;
};