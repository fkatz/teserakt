#pragma once
#include <string>
using namespace std;

struct System
{
	System(string name);;
	string name;
	virtual void onUpdate() = 0;
};