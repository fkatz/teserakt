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
	ceilingPosition->y = 720;

	entityId wall1Id = registry->gameState->createEntity("wall.json");
	entityId wall2Id = registry->gameState->createEntity("wall.json");
	PositionComponent* wall2Position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, wall2Id);
	wall2Position->x = 1440;
	entityId wall3Id = registry->gameState->createEntity("wall.json");
	PositionComponent* wall3Position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, wall3Id);
	SizeComponent* wall3Size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, wall3Id);
	wall3Position->x = 720;
	wall3Position->y = -120;
	wall3Size->width = 24;
	wall3Size->height = 240;
	entityId platformId = registry->gameState->createEntity("ground.json");
	PositionComponent* platformPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, platformId);
	SizeComponent* platformSize = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, platformId);
	platformPosition->x = 720;
	platformPosition->y = 120;
	platformSize->width = 240;
	platformSize->height = 24;
	entityId platform2Id = registry->gameState->createEntity("ground.json");
	PositionComponent* platform2Position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, platform2Id);
	SizeComponent* platform2Size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, platform2Id);
	platform2Position->x = 1080;
	platform2Position->y = 240;
	platform2Size->width = 240;
	platform2Size->height = 24;
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