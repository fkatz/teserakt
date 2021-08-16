#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct SizeComponent : public Component
{
	SizeComponent(nlohmann::json json) : Component(json) {
		width = json["width"].get<double>();
		height = json["height"].get<double>();
	}
	double width;
	double height;
};