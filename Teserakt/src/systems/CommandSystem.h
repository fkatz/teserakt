#pragma once
#include "System.h"
#include <SDL_events.h>
#include <vector>
#include "../states/GameState.cpp"
#include "../commands/Command.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

struct Registry;

enum CommandType {
	MOVE_UP,
	MOVE_DOWN,
	JUMP,
	MOVE_LEFT,
	MOVE_RIGHT,
	STOP_MOVE_X,
	STOP_MOVE_Y,
	THROW,
	SPECIAL_ACTION,
};

struct CommandSystem : public System
{
	CommandSystem(Registry* registry);

	void onUpdate();

	void onCommand(CommandType commandType, entityId entity, Command* command = NULL);
protected:
	Registry* registry;
private:
	void setEntityVelocityX(entityId entityId, double vx);
	void setEntityVelocityY(entityId entityId, double vy);
	int lua_superjump(lua_State* L);
	void runLua();
};