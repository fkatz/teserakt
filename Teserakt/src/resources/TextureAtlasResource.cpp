#pragma once
#include <nlohmann/json.hpp>
#include <map>
#include "TextureResource.cpp"
using namespace std;

struct Frame {
	Frame(int x, int y, int w, int h, int duration) :rect{ x,y,w,h }, duration(duration) {};
	SDL_Rect rect;
	int duration;
};

struct TextureAtlasResource : public TextureResource
{
	TextureAtlasResource(SDL_Texture* texture, nlohmann::json json) :TextureResource(texture) {
		nlohmann::json frames = json.at("frames");
		for (auto frameData : frames.items()) {
			auto frameValues = frameData.value().at("frame");
			auto frame = new Frame(
				frameValues.at("x").get<int>(),
				frameValues.at("y").get<int>(),
				frameValues.at("w").get<int>(),
				frameValues.at("h").get<int>(),
				frameData.value().at("duration").get<int>()
			);
			sprites[frameData.key()] = frame;
		}
	};
	map<string, Frame*> sprites;
	~TextureAtlasResource() {
		for (auto const& item : sprites) {
			delete item.second;
		}
	};
};