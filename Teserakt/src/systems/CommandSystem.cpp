#include "CommandSystem.h"
#include "../Registry.h"
#include "../commands/ThrowCommand.h"

CommandSystem::CommandSystem(Registry* registry) :
	System("command"),
	registry(registry) {
}

void CommandSystem::onUpdate() {
	auto playableEntities = getPlayableEntities();
	for (auto playableEntityId : playableEntities) {
		PhysicsComponent* physics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, playableEntityId);
		CommandsComponent* commands = registry->gameState->getComponent<CommandsComponent>(COMPONENT_COMMANDS, playableEntityId);
		if (!physics->isStanding && !physics->isWallJumping) {
			commands->state = "airborne";
		}
		else if (physics->isStanding) {
			if (physics->vx > 0 || physics->vx < 0) {
				commands->state = "movingOnGround";
			}
			else {
				commands->state = "idle";
			}
		}
		if (physics->vx < 0) {
			commands->facingLeft = false;
		}
		else if (physics->vx > 0) {
			commands->facingLeft = true;
		}
	}
	if (registry->cameraState->followPlayers && playableEntities.size() > 0) {
		PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, playableEntities[0]);
		SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, playableEntities[0]);
		SDL_Rect playersUnionRect = { position->x,position->y,size->width,size->height };
		for (auto playableEntityId : playableEntities) {
			PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, playableEntityId);
			SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, playableEntityId);
			SDL_Rect playerRect = { position->x,position->y,size->width,size->height };
			SDL_UnionRect(&playersUnionRect, &playerRect, &playersUnionRect);
		}
		registry->cameraState->cameraX = playersUnionRect.x + playersUnionRect.w / 2;
		registry->cameraState->cameraY = playersUnionRect.y + playersUnionRect.h / 2;
	}

}

static int lua_get444(lua_State* L) {
	double d = lua_tonumber(L, 1);  /* get argument */
	lua_pushnumber(L, d + 444.0);  /* push result */
	return 1;  /* number of results */
}

typedef int (CommandSystem::* mem_func)(lua_State* L);
// This template wraps a member function into a C-style "free" function compatible with lua.
template <mem_func func>
int dispatch(lua_State* L) {
	CommandSystem* ptr = *static_cast<CommandSystem**>(lua_getextraspace(L));
	return ((*ptr).*func)(L);
}

int CommandSystem::lua_superjump(lua_State* L) {
	auto playableEntities = getPlayableEntities();
	for (auto playableEntityId : playableEntities) {
		setEntityVelocityY(playableEntityId, 2000);
	}
	return 0;  /* number of results */
}

void CommandSystem::runLua() {
	// initialization
	lua_State* L = luaL_newstate();
	CommandSystem* commandSystem = this;
	*static_cast<CommandSystem**>(lua_getextraspace(L)) = commandSystem;

	luaL_openlibs(L);
	lua_pushcfunction(L, &dispatch<&CommandSystem::lua_superjump>);
	lua_setglobal(L, "superjump");
	// execute script
	const char lua_script[] = "superjump()"; // a function that returns sum of two
	int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);
	lua_pcall(L, 0, 0, 0);
	// cleanup
	lua_close(L);
}

void CommandSystem::onCommand(CommandType commandType, Command* command)
{
	auto playableEntities = getPlayableEntities();
	switch (commandType) {
	case JUMP:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityY(playableEntityId, 500);
		}
		break;
	case MOVE_UP:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityY(playableEntityId, 200);
		}
		break;
	case MOVE_DOWN:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityY(playableEntityId, -200);
		}
		break;
	case MOVE_LEFT:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityX(playableEntityId, -200);
		}
		break;
	case MOVE_RIGHT:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityX(playableEntityId, 200);
		}
		break;
	case STOP_MOVE_X:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityX(playableEntityId, 0);
		}
		break;
	case STOP_MOVE_Y:
		for (auto playableEntityId : playableEntities) {
			setEntityVelocityY(playableEntityId, 0);
		}
		break;
	case THROW:
	{
		ThrowCommand* throwCommand = static_cast<ThrowCommand*> (command);
		PositionComponent* playerPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, throwCommand->originEntityId);
		SizeComponent* playerSize = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, throwCommand->originEntityId);

		entityId itemId = registry->gameState->createEntity(throwCommand->entityFilename);
		SizeComponent* itemSize = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, itemId);
		PositionComponent* itemPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, itemId);
		PhysicsComponent* itemPhysics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, itemId);

		double playerItemRectangleW = playerSize->width + itemSize->width;
		double playerItemRectangleH = playerSize->height + itemSize->height;
		double radius = sqrt(playerItemRectangleH * playerItemRectangleH + playerItemRectangleW * playerItemRectangleW) / 2;
		itemPosition->x = playerPosition->x + (playerSize->width / 2) + radius * cos(throwCommand->angle) - (itemSize->width / 2);
		itemPosition->y = playerPosition->y + (playerSize->height / 2) + radius * sin(throwCommand->angle) - (itemSize->height / 2);
		itemPhysics->vx = throwCommand->velocity * cos(throwCommand->angle);
		itemPhysics->vy = throwCommand->velocity * sin(throwCommand->angle);
	}
	break;
	case SPECIAL_ACTION:
		runLua();
		break;
	default:
		break;
	}
}

vector<entityId> CommandSystem::getPlayableEntities()
{
	vector<entityId> entities;
	for (auto entityId : registry->gameState->entities) {
		bool hasComponent = registry->gameState->hasComponent(COMPONENT_COMMANDS, entityId);
		if (hasComponent) {
			CommandsComponent* commands = registry->gameState->getComponent<CommandsComponent>(COMPONENT_COMMANDS, entityId);
			if (commands->playable) {
				entities.push_back(entityId);
			}
		}
	}
	return entities;
}

void CommandSystem::setEntityVelocityX(entityId entityId, double vx)
{
	PhysicsComponent* physics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, entityId);
	physics->vx = vx;
}
void CommandSystem::setEntityVelocityY(entityId entityId, double vy)
{
	PhysicsComponent* physics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, entityId);
	physics->vy = vy;
}