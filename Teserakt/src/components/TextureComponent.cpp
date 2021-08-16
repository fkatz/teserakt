#pragma once
#include "Component.cpp"
#include <nlohmann/json.hpp>
using namespace std;

struct TextureComponent : public Component
{
	TextureComponent(nlohmann::json json) : Component(json) {
		filename = json.at("filename").get<string>();
		auto framesData = json.at("frames").items();
		for (auto frames : framesData) {
			vector<string> stateFrames;
			if (frames.value().is_array()) {
				for (auto value : frames.value()) {
					stateFrames.push_back(value.get<string>());
				}
			}
			else {
				stateFrames.push_back(frames.value().get<string>());
			}
			stateFramesMap.emplace(frames.key(), stateFrames);
		}
	}
	map<string, vector<string>> stateFramesMap;
	string  filename;
};