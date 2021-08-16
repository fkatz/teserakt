#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct SizeComponent : public Component
{
	SizeComponent(nlohmann::json json) : Component(json) {
		width = json["width"].get<int>();
		height = json["height"].get<int>();
	}
	int width;
	int height;
};