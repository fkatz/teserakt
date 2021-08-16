#include "RenderSystem.h"
#include "../components/PositionComponent.cpp"
#include "../components/SizeComponent.cpp"
#include "../resources/TextureResource.cpp"
#include "../resources/TextureAtlasResource.cpp"
#include "../states/GameState.cpp"
#include <SDL.h>
#include <SDL_image.h>
#include <nlohmann/json.hpp>
#include "../Registry.h"
using namespace nlohmann;

SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

RenderSystem::RenderSystem(Registry* registry) :
	System("render"),
	registry(registry) {
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	auto texture = loadTexture("resources/sprites/cosmo.png", registry->windowState->renderer);
	ifstream jsonFile("resources/sprites/cosmo.json");
	auto json = json::parse(jsonFile);
	registry->gameState->resources.emplace("cosmo.png", new TextureAtlasResource(texture, json));
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

void RenderSystem::onUpdate() {
	SDL_SetRenderDrawColor(registry->windowState->renderer, 0xA5, 0xA6, 0xA1, 0xFF);
	//Clear screen
	SDL_RenderClear(registry->windowState->renderer);

	//Update screen
	int screenWidth;
	int screenHeight;
	SDL_GetWindowSize(registry->windowState->window, &screenWidth, &screenHeight);
	for (auto entityId : registry->gameState->entities) {
		PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entityId);
		SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entityId);
		SDL_Rect entityRect = {
			(int)position->x - ((int)registry->cameraState->cameraX - SCREEN_WIDTH / 2),
			screenHeight - size->height - ((int)position->y - ((int)registry->cameraState->cameraY - SCREEN_HEIGHT / 2)),
			size->width,
			size->height
		};
		if (registry->gameState->hasComponent(COMPONENT_TEXTURE, entityId)) {
			TextureComponent* textureComponent = registry->gameState->getComponent<TextureComponent>(COMPONENT_TEXTURE, entityId);
			if (TextureAtlasResource* textureAtlasResource = dynamic_cast<TextureAtlasResource*>(registry->gameState->resources[textureComponent->filename]))
			{
				CommandsComponent* commandsComponent = registry->gameState->getComponent<CommandsComponent>(COMPONENT_COMMANDS, entityId);
				if (!commandsComponent->state.empty()
					&& textureComponent->stateFramesMap.contains(commandsComponent->state)) {
					vector<Frame*> frames;
					for (auto frameName : textureComponent->stateFramesMap.at(commandsComponent->state)) {
						frames.push_back(textureAtlasResource->sprites.at(frameName));
					}

					int numberOfFrames = frames.size();
					int totalDuration = 0;
					for (auto frame : frames) {
						totalDuration += frame->duration;
					}
					auto now = SDL_GetTicks();
					int relativeTime = (now % totalDuration);
					int currentFrame = 0;
					for (int frameIndex = 0; frameIndex < frames.size(); frameIndex++) {
						if (relativeTime >= 0) {
							currentFrame = frameIndex;
						}
						else {
							break;
						}
						relativeTime -= frames[frameIndex]->duration;
					}
					SDL_RenderCopyEx(registry->windowState->renderer, textureAtlasResource->texture, &frames[currentFrame]->rect, &entityRect, 0, 0, commandsComponent->facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
				}
			}

		}
		else {
			SDL_SetRenderDrawColor(registry->windowState->renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(registry->windowState->renderer, &entityRect);
		}
	}
	SDL_RenderPresent(registry->windowState->renderer);
	SDL_UpdateWindowSurface(registry->windowState->window);
}

void RenderSystem::onQuit() {
	SDL_DestroyWindow(registry->windowState->window);
}
