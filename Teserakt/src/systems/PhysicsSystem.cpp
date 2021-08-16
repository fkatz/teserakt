#include "PhysicsSystem.h"
#include "../Registry.h"
#include <SDL.h>

PhysicsSystem::PhysicsSystem(Registry* registry) :
	System("physics"),
	registry(registry) {
	lastUpdateTime = SDL_GetTicks();
}

double gravity = 1000;
double minV = -1000;

void PhysicsSystem::onUpdate()
{
	auto currentTime = SDL_GetTicks();
	double deltaSeconds = ((double)currentTime - (double)lastUpdateTime) / 1000;
	for (auto entity : registry->gameState->entities) {
		PhysicsComponent* physics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, entity);
		if (physics->collides) {
			PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entity);
			SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entity);
			physics->vy = max(physics->vy - ((physics->hasGravity ? gravity : 0) * deltaSeconds), minV);
			int futureX = (position->x + (int)(physics->vx * deltaSeconds));
			int futureY = (position->y + (int)(physics->vy * deltaSeconds));
			if (futureX != position->x || futureY != position->y) {
				vector<entityId> intersectingEntities;
				for (auto otherEntity : registry->gameState->entities) {
					if (otherEntity != entity) {
						PhysicsComponent* otherEntityPhysics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, otherEntity);
						if (otherEntityPhysics->collidable && futureIntersects(entity, otherEntity, round(futureX), round(futureY), -1, -1)) {
							intersectingEntities.push_back(otherEntity);
						}
					}
				}
				if (intersectingEntities.size() > 0) {
					int lastX = position->x;
					int lastY = position->y;
					int finalX = lastX;
					int finalY = lastY;
					bool collidedTotal, collidedX, collidedY;
					collide(round(finalX), round(finalY), round(futureX), round(futureY), intersectingEntities, entity, lastX, lastY, collidedTotal, false, false);
					bool touchingX = false;
					bool touchingBottom = false;
					if (lastX != futureX) {
						touchingX = true;
					}
					if (lastY != futureY) {
						if (futureY < lastY) {
							touchingX = false;
							touchingBottom = true;
						}
					}
					finalX = lastX;
					finalY = lastY;
					collide(round(finalX), round(finalY), round(futureX), round(finalY), intersectingEntities, entity, lastX, lastY, collidedX, 0, 1);
					if (lastX != finalX) {
						physics->vx = physics->vx - physics->vx * physics->friction;
						finalX = lastX;
						touchingX = false;
					}
					collide(round(finalX), round(finalY), round(finalX), round(futureY), intersectingEntities, entity, lastX, lastY, collidedY, 1, 0);
					if (lastY != finalY) {
						physics->vy = physics->vy - physics->vy * physics->friction;
						finalY = lastY;
						touchingBottom = false;
					}
					if (touchingX && !touchingBottom) {
						physics->isStanding = false;
						physics->isWallJumping = true;
						physics->wallJumpingEntity = intersectingEntities[0];
						physics->standingOnEntity = 0;
					}
					else if (touchingBottom) {
						physics->isStanding = true;
						physics->isWallJumping = false;
						physics->wallJumpingEntity = 0;
						physics->standingOnEntity = intersectingEntities[0];
					}
					futureX = finalX;
					futureY = finalY;
					if (collidedX) physics->vx = 0;
					if (collidedY) physics->vy = 0;
				}
				else {
					physics->isStanding = false;
					physics->isWallJumping = false;
					physics->wallJumpingEntity = 0;
					physics->standingOnEntity = 0;
				}
			}
			position->x = futureX;
			position->y = futureY;
		}

	}
	lastUpdateTime = currentTime;
}

void PhysicsSystem::collide(int currentX, int currentY, int futureX, int futureY, std::vector<entityId>& intersectingEntities, const entityId& entity, int& lastX, int& lastY, bool& intersected, bool shrinkX, bool shrinkY)
{
	int deltaX = futureX - currentX;
	int deltaY = futureY - currentY;
	double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	double angle = atan2(deltaY, deltaX);
	intersected = false;
	for (int sample = 1; sample <= ceil(distance); sample++) {
		int unitX = (deltaX == 0 ? 0 : sample * cos(angle));
		int unitY = (deltaY == 0 ? 0 : sample * sin(angle));
		int sampleX = currentX + unitX;
		int sampleY = currentY + unitY;
		for (auto otherEntity : intersectingEntities) {
			PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, otherEntity);
			SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, otherEntity);
			if (futureIntersects(entity, otherEntity, sampleX, sampleY, shrinkX, shrinkY)) {
				intersected = true;
				break;
			}
		}
		if (intersected) {
			break;
		}
		else {
			lastX = sampleX;
			lastY = sampleY;
		}
	}
}

bool PhysicsSystem::futureIntersects(entityId entityIdA, entityId entityIdB, int futureX, int futureY, int paddingX, int paddingY)
{
	PositionComponent* positionA = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entityIdA);
	SizeComponent* sizeA = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entityIdA);
	SDL_Rect rectA = { min(positionA->x, futureX )+ paddingX,min(positionA->y,futureY )+ paddingY,abs(positionA->x - futureX) + sizeA->width - 2* paddingX,abs(positionA->y - futureY) + sizeA->height - 2* paddingY };
	PositionComponent* positionB = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entityIdB);
	SizeComponent* sizeB = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entityIdB);
	SDL_Rect rectB = { (int)positionB->x,(int)positionB->y,(int)sizeB->width,(int)sizeB->height };
	bool intersected = SDL_HasIntersection(&rectA, &rectB) == SDL_TRUE;
	return intersected;
}
