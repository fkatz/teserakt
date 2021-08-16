#include "PhysicsSystem.h"
#include "../Registry.h"
#include <SDL.h>

PhysicsSystem::PhysicsSystem(Registry* registry) :
	System("physics"),
	registry(registry) {
	lastUpdateTime = SDL_GetTicks();
}

double gravity = 1000;

void PhysicsSystem::onUpdate()
{
	auto currentTime = SDL_GetTicks();
	double deltaSeconds = ((double)currentTime - (double)lastUpdateTime) / 1000;
	for (auto entity : registry->gameState->entities) {
		PhysicsComponent* physics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, entity);
		if (physics->collides) {
			PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entity);
			SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entity);
			physics->vy = physics->vy - ((physics->hasGravity ? gravity : 0) * deltaSeconds);
			float futureX = (float)(position->x + (physics->vx * deltaSeconds));
			float futureY = (float)(position->y + (physics->vy * deltaSeconds));
			if (futureX != position->x || futureY != position->y) {
				vector<entityId> intersectingEntities;
				for (auto otherEntity : registry->gameState->entities) {
					if (otherEntity != entity) {
						PhysicsComponent* otherEntityPhysics = registry->gameState->getComponent<PhysicsComponent>(COMPONENT_PHYSICS, otherEntity);
						if (otherEntityPhysics->collidable && futureIntersects(entity, otherEntity, futureX, futureY)) {
							intersectingEntities.push_back(otherEntity);
						}
					}
				}
				if (intersectingEntities.size() > 0) {
					float lastX = position->x;
					float lastY = position->y;
					float finalX = lastX;
					float finalY = lastY;
					bool collidedTotal, collidedX, collidedY;
					collide(finalX, finalY, futureX, futureY, intersectingEntities, entity, lastX, lastY, collidedTotal);
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
					collide(finalX, finalY, futureX, finalY, intersectingEntities, entity, lastX, lastY, collidedX);
					if (lastX != finalX) {
						physics->vx = physics->vx - physics->vx * physics->friction;
						finalX = lastX;
						touchingX = false;
					}
					collide(finalX, finalY, finalX, futureY, intersectingEntities, entity, lastX, lastY, collidedY);
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

void PhysicsSystem::collide(float currentX, float currentY, float futureX, float futureY, std::vector<entityId>& intersectingEntities, const entityId& entity, float& lastX, float& lastY, bool& intersected)
{
	double deltaX = (double)futureX - (double)currentX;
	double deltaY = (double)futureY - (double)currentY;
	double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	double angle = atan2(deltaY, deltaX);
	intersected = false;
	for (int sample = 1; sample <= ceil(distance); sample++) {
		double unitX = (deltaX == 0 ? 0 : sample * cos(angle));
		double unitY = (deltaY == 0 ? 0 : sample * sin(angle));
		double sampleX = currentX + unitX;
		double sampleY = currentY + unitY;
		for (auto otherEntity : intersectingEntities) {
			PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, otherEntity);
			SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, otherEntity);
			if (futureIntersects(entity, otherEntity, (float)sampleX, (float)sampleY)) {
				intersected = true;
				break;
			}
		}
		if (!intersected) {
			lastX = (float)sampleX;
			lastY = (float)sampleY;
			if ((sample == ceil(distance) && deltaX > 0 && sampleX > futureX)
				|| (sample == ceil(distance) && deltaX < 0 && sampleX < futureX)) {
				lastX = futureX;
			}
			if ((sample == ceil(distance) && deltaY > 0 && sampleY > futureY)
				|| (sample == ceil(distance) && deltaY < 0 && sampleY < futureY)) {
				lastY = futureY;
			}
		}
		else {
			break;
		}
	}
}

bool PhysicsSystem::futureIntersects(entityId entityIdA, entityId entityIdB, float futureX, float futureY)
{
	PositionComponent* positionA = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entityIdA);
	SizeComponent* sizeA = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entityIdA);
	SDL_Rect rectA = { (int)floor(min(positionA->x, futureX)),(int)floor(min(positionA->y,futureY)),(int)ceil(abs(positionA->x - futureX) + sizeA->width),(int)ceil(abs(positionA->y - futureY) + sizeA->height) };
	PositionComponent* positionB = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entityIdB);
	SizeComponent* sizeB = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entityIdB);
	SDL_Rect rectB = { (int)positionB->x,(int)positionB->y,(int)sizeB->width,(int)sizeB->height };
	return SDL_HasIntersection(&rectA, &rectB) == SDL_TRUE;
}
