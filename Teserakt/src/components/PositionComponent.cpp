#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct PositionComponent : public Component
{
	PositionComponent(nlohmann::json json) : Component(json) {
		x = json["x"].get<int>();
		y = json["y"].get<int>();
		z = json["z"].get<int>();
		realX = x;
		realY = y;
	}
	float realX;
	float realY;
	int x;
	int y;
	int z;
};