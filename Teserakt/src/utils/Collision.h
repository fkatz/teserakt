#pragma once
#include <tuple>
#include "../states/GameState.cpp"

struct Registry;

tuple<entityId, int, int> getRayCollision(Registry* registry, int x1, int y1, int rayDistance, double radians, entityId originEntity = NULL);

tuple<entityId, int, int> getRayCollision(Registry* registry, entityId originEntity, double radians);