#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct PositionComponent : public Component
{
	PositionComponent(nlohmann::json json) : Component(json) {
		x = json["x"].get<float>();
		y = json["y"].get<float>();
		z = json["z"].get<float>();
	}
	float x;
	float y;
	float z;
};