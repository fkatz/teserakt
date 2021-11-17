#include "AISystem.h"
#include "../Registry.h"
#include "../commands/ThrowCommand.h"
#include "../utils/Collision.h"

AISystem::AISystem(Registry* registry) :
	System("input"),
	registry(registry) {
}

vector<entityId> getAIEntities(Registry* registry) {
	vector<entityId> entities;
	for (entityId entity : registry->gameState->entities) {
		if (registry->gameState->hasComponent(COMPONENT_AI, entity)) entities.push_back(entity);
	}
	return entities;
}

void AISystem::onUpdate()
{
	auto aiEntities = getAIEntities(registry);
	for (entityId entity : aiEntities) {
		PositionComponent* position = registry->gameState->getComponent<PositionComponent>(COMPONENT_POSITION, entity);
		SizeComponent* size = registry->gameState->getComponent<SizeComponent>(COMPONENT_SIZE, entity);
		AIComponent* ai = registry->gameState->getComponent<AIComponent>(COMPONENT_AI, entity);
		auto currentTime = SDL_GetTicks();
		double sinTime = (sin((double)currentTime / 500));
		double minAngle = (ai->patrollingRight ? ai->visionAngleStart : 180 - ai->visionAngleStart) * (atan(1) * 4) / 180;
		double maxAngle = (ai->patrollingRight ? ai->visionAngleEnd : 180 - ai->visionAngleEnd) * (atan(1) * 4) / 180;
		double midAngle = (maxAngle - minAngle) / 2;
		double radians = minAngle + midAngle + sinTime * midAngle;
		auto collision = getRayCollision(registry, entity, radians);
		ai->targetEntity = std::get<0>(collision);
		ai->targetX = std::get<1>(collision);
		ai->targetY = std::get<2>(collision);
		if (ai->targetEntity != NULL
			&& registry->gameState->isEntityAlive(ai->targetEntity)
			&& registry->gameState->hasComponent(COMPONENT_COMMANDS, ai->targetEntity)
			&& registry->gameState->getComponent<CommandsComponent>(COMPONENT_COMMANDS, ai->targetEntity)->playable) {
			if (ai->lastShoot == 0 || (currentTime - ai->lastShoot) >= ai->shootCooldown) {
				registry->commandSystem->onCommand(THROW, entity, new ThrowCommand(1600, radians, "rock.json"));
				ai->lastShoot = currentTime;
			}
		}
		if (ai->patrolDistance > 0) {
			if (ai->patrollingRight) {
				if (position->x < ai->patrolOriginX + (ai->patrolDistance / 2)) {
					registry->commandSystem->onCommand(MOVE_RIGHT, entity);
				}
				else ai->patrollingRight = false;
			}
			else {
				if (position->x > ai->patrolOriginX - (ai->patrolDistance / 2)) {
					registry->commandSystem->onCommand(MOVE_LEFT, entity);
				}
				else ai->patrollingRight = true;
			}
		}
	}
}