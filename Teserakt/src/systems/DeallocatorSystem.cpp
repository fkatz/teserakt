#include "DeallocatorSystem.h"
#include "../Registry.h"
DeallocatorSystem::DeallocatorSystem(Registry* registry) :
	System("input"),
	registry(registry) {
}

int maxEntities = 3;

void DeallocatorSystem::onUpdate()
{
	int clearableEntityQuantity = 0;
	do {
		clearableEntityQuantity = 0;
		uint32_t minCreatedAt = numeric_limits<uint32_t>::max();
		entityId minCreatedAtEntity = NULL;
		for (entityId entity : registry->gameState->entities) {
			if (registry->gameState->hasComponent(COMPONENT_DEALLOCATE, entity)) {
				DeallocateComponent* deallocateComponent = registry->gameState->getComponent<DeallocateComponent>(COMPONENT_DEALLOCATE, entity);
				if (deallocateComponent->clearOnMaxQuantity) clearableEntityQuantity++;
				if (deallocateComponent->createdAt < minCreatedAt) {
					minCreatedAt = deallocateComponent->createdAt;
					minCreatedAtEntity = entity;
				}
			}
		}
		if (clearableEntityQuantity > maxEntities && minCreatedAtEntity != NULL) {
			registry->gameState->removeEntity(minCreatedAtEntity);
		}
	} while (clearableEntityQuantity > maxEntities);
}
