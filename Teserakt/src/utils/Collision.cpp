#include "./Collision.h"
#include "../Registry.h"

tuple<entityId, int, int> getRayCollision(Registry* registry, int x1, int y1, int rayDistance, double radians, entityId originEntity) {
	int x2 = x1 + (rayDistance * cos(radians));
	int y2 = y1 + (rayDistance * sin(radians));
	int xf = x2;
	int yf = y2;
	entityId collidingEntity = NULL;
	for (entityId otherEntity : registry->gameState->entities) {
		if (originEntity == NULL || otherEntity != originEntity) {
			int ix1 = x1;
			int iy1 = y1;
			int ix2 = x2;
			int iy2 = y2;
			PositionComponent* otherPosition = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, otherEntity);
			SizeComponent* otherSize = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, otherEntity);
			SDL_bool intersects = SDL_IntersectRectAndLine(
				new SDL_Rect{ otherPosition->x,otherPosition->y,otherSize->width,otherSize->height },
				&ix1, &iy1, &ix2, &iy2);
			if (intersects == SDL_TRUE) {
				if (sqrt((pow((ix1 - x1), 2) + pow((iy1 - y1), 2))) < sqrt((pow((xf - x1), 2) + pow((yf - y1), 2)))) {
					collidingEntity = otherEntity;
					xf = ix1;
					yf = iy1;
				}
			}
		}
	}
	return std::make_tuple(collidingEntity, xf, yf);
}

tuple<entityId, int, int> getRayCollision(Registry* registry, entityId originEntity, double radians) {
	PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, originEntity);
	SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, originEntity);
	AIComponent* ai = registry->gameState->getComponent<AIComponent>(COMPONENT_AI, originEntity);
	int x1 = position->x + (size->width / 2);
	int y1 = position->y + (size->height / 2);
	return getRayCollision(registry, x1, y1, ai->visionDistance, radians, originEntity);
}
