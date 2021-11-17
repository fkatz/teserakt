#include "InputSystem.h"
#include "../Application.h"
#include <SDL.h>
#include "../Registry.h"
#include "../commands/ThrowCommand.h"

InputSystem::InputSystem(Registry* registry) :
	System("input"),
	registry(registry) {
}

void InputSystem::onUpdate() {
	auto playableEntities = registry->gameState->getPlayableEntities();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		for (auto entity : playableEntities) {
			switch (event.type) {
			case SDL_QUIT:
				registry->renderSystem->onQuit();
				registry->applicationState->quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.key.repeat == 0) {
					registry->commandSystem->onCommand(JUMP, entity);
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_Q && event.key.repeat == 0) {
					registry->commandSystem->onCommand(SPECIAL_ACTION, entity);
				}
				//if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				//	registry->commandSystem->onCommand(MOVE_LEFT);
				//}
				//if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				//	registry->commandSystem->onCommand(MOVE_RIGHT);
				//}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_D) {
					registry->commandSystem->onCommand(STOP_MOVE_X, entity);
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.key.repeat == 0) {
					registry->commandSystem->onCommand(STOP_MOVE_Y, entity);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				CommandsComponent* entityCommands = registry->gameState->getComponent<CommandsComponent>(COMPONENT_COMMANDS, entity);
				if (entityCommands->playable) {
					PositionComponent* playerPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entity);
					SizeComponent* playerSize = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entity);

					double mouseX = event.button.x;
					double mouseY = (double)SCREEN_HEIGHT - (double)event.button.y;
					double mouseXRelative = mouseX - (playerPosition->x + (playerSize->width / 2));
					double mouseYRelative = mouseY - (playerPosition->y + (playerSize->height / 2));
					double angle = atan2(mouseYRelative, mouseXRelative);

					registry->commandSystem->onCommand(THROW, entity, new ThrowCommand(3000, angle, "rock.json"));
				}
			}
			break;
			default:
				break;
			}
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	for (auto entity : playableEntities) {
		if (state[SDL_SCANCODE_A]) {
			registry->commandSystem->onCommand(MOVE_LEFT, entity);
		}
		if (state[SDL_SCANCODE_D]) {
			registry->commandSystem->onCommand(MOVE_RIGHT, entity);
		}
	}
}
