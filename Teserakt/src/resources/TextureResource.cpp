#pragma once
#include "Resource.cpp"
#include <SDL.h>

struct TextureResource : public Resource
{
	TextureResource(SDL_Texture* texture) :texture(texture) {};
	SDL_Texture* texture;

	virtual ~TextureResource() {
		SDL_DestroyTexture(texture);
	}
};