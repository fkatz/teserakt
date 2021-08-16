#pragma once
#include <vector>
#include <SDL.h>
#include "Application.h"
#include <iostream>
using namespace std;

Application::Application() {
	registry = new Registry();
}

void Application::run() {
	entityId playerId = registry->gameState->createEntity("player.json");
	entityId blockId = registry->gameState->createEntity("block.json");
	entityId groundId = registry->gameState->createEntity("ground.json");

	entityId ceilingId = registry->gameState->createEntity("ground.json");
	PositionComponent* ceilingPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, ceilingId);
	ceilingPosition->y = 710;

	entityId wall1Id = registry->gameState->createEntity("wall.json");
	entityId wall2Id = registry->gameState->createEntity("wall.json");
	PositionComponent* wall2Position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, wall2Id);
	wall2Position->x = 1270;
	auto lastTime = SDL_GetTicks();
	while (!registry->applicationState->quit) {
		for (auto system : registry->systems) {
			auto startTime = SDL_GetTicks();
			if (!registry->applicationState->quit)system->onUpdate();
			auto endTime = SDL_GetTicks();
			//cout << "system " << system->name << " took " << endTime - startTime << endl;
		}
		auto currentTime = SDL_GetTicks();
		auto timeDelta = currentTime - lastTime;
		if (timeDelta < 10) {
			SDL_Delay(10 - timeDelta);
		}
		lastTime = SDL_GetTicks();
	}
}