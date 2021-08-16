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
	}
	int x;
	int y;
	int z;
};