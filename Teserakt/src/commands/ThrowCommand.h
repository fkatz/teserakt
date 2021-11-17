#pragma once
#include <string>
#include "Command.h"
using namespace std;

struct ThrowCommand : public Command {
	ThrowCommand(double velocity, double angle, string entityFilename) 
		:velocity(velocity), angle(angle), entityFilename(entityFilename) {}
	double velocity;
	double angle;
	string entityFilename;
};