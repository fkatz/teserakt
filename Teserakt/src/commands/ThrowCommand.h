#pragma once
#include <string>
#include "Command.h"
using namespace std;

struct ThrowCommand : public Command {
	ThrowCommand(double velocity, double angle, string entityFilename, entityId originEntityId) 
		:velocity(velocity), angle(angle), entityFilename(entityFilename), originEntityId(originEntityId) {}
	double velocity;
	double angle;
	string entityFilename;
	entityId originEntityId;
};