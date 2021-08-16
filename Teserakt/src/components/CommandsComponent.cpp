#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct CommandsComponent : public Component
{
	CommandsComponent(nlohmann::json json) : Component(json) {
		playable = json["playable"].get<bool>();
	}
	bool playable;
	string state;
	bool facingLeft = true;
};