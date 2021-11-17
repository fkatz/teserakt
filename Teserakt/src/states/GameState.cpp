#pragma once
#include <map>
#include <vector>
#include <stack>
#include <optional>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../components/Component.cpp"
#include "../components/TextureComponent.cpp"
#include "../components/SizeComponent.cpp"
#include "../components/PositionComponent.cpp"
#include "../components/CommandsComponent.cpp"
#include "../components/PhysicsComponent.cpp"
#include "../resources/Resource.cpp"
#include "../components/AIComponent.cpp"
#include "../components/DeallocateComponent.cpp"
using namespace std;
using namespace nlohmann;

#define COMPONENT_TEXTURE "texture"
#define COMPONENT_SIZE "size"
#define COMPONENT_POSITION "position"
#define COMPONENT_COMMANDS "commands"
#define COMPONENT_PHYSICS "physics"
#define COMPONENT_AI "ai"
#define COMPONENT_DEALLOCATE "deallocate"

typedef uint64_t entityId;
typedef  uint64_t entityIndex;
typedef  uint64_t resourceIndex;
template<typename T> Component* createComponentInstance(json json) { return new T(json); };
template<typename T> void setComponentType(string componentTypeName, map<string, Component* (*)(json)>* componentTypes, map < string, vector<optional<Component*>>>* components) {
	componentTypes->emplace(componentTypeName, &createComponentInstance<T>);
	components->emplace(componentTypeName, vector<optional<Component*>>());
};

struct GameState
{
	GameState() {
		setComponentType<TextureComponent>(COMPONENT_TEXTURE, &componentTypes, &components);
		setComponentType<SizeComponent>(COMPONENT_SIZE, &componentTypes, &components);
		setComponentType<PositionComponent>(COMPONENT_POSITION, &componentTypes, &components);
		setComponentType<CommandsComponent>(COMPONENT_COMMANDS, &componentTypes, &components);
		setComponentType<PhysicsComponent>(COMPONENT_PHYSICS, &componentTypes, &components);
		setComponentType<AIComponent>(COMPONENT_AI, &componentTypes, &components);
		setComponentType<DeallocateComponent>(COMPONENT_DEALLOCATE, &componentTypes, &components);
	}

	vector<entityId> entities;
	map<string, Resource*> resources;

	entityId createEntity(string fileName) {
		entityId newId = ++lastId;
		entities.push_back(newId);
		if (deallocatedEntityIndexes.empty()) {
			entityIdIndex.emplace(newId, entities.size() - 1);
			for (auto componentItem : components) {
				components[componentItem.first].push_back(optional<Component*>());
			}
		}
		else {
			entityIndex replacedIndex = deallocatedEntityIndexes.top();
			entityIdIndex.emplace(newId, replacedIndex);
			deallocatedEntityIndexes.pop();
		}
		ifstream ifs("resources/entities/" + fileName);
		json config = json::parse(ifs);
		for (auto componentVector : components) {
			string componentType = componentVector.first;
			bool exists = config.find(componentType) != config.end();
			if (componentTypes.contains(componentType)) {
				if (exists) {
					components[componentType][entityIdIndex[newId]] = optional(componentTypes.at(componentType)((config.at(componentType))));
				}
			}
		}
		return newId;
	}

	bool hasComponent(string componentName, entityId entityId) {
		return components[componentName][entityIdIndex[entityId]].has_value();
	};
	template <typename T> T* getComponent(string componentName, entityId entityId) {
		return static_cast<T*>(components[componentName][entityIdIndex[entityId]].value());
	}
	void setComponent(string componentName, entityId entityId, Component* component) {
		components[componentName].at(entityIdIndex[entityId]) = optional(component);
	}
	void removeComponent(string componentName, entityId entityId) {
		if (hasComponent(componentName, entityId)) {
			delete (components[componentName][entityIdIndex[entityId]].value());
		}
		components[componentName].at(entityIdIndex[entityId]) = optional<Component*>();
	}
	void removeEntity(entityId entity) {
		for (auto typeItem : componentTypes) {
			removeComponent(typeItem.first, entity);
		}
		auto vecIt = find(entities.begin(), entities.end(), entity);
		entities.erase(vecIt);
		deallocatedEntityIndexes.push(entityIdIndex[entity]);
		auto mapIt = entityIdIndex.find(entity);
		entityIdIndex.erase(mapIt);
	}
	bool isEntityAlive(entityId entityId) {
		return entityIdIndex.contains(entityId);
	}

	vector<entityId> getPlayableEntities()
	{
		vector<entityId> playableEntities;
		for (auto entityId : entities) {
			bool hasCommands = hasComponent(COMPONENT_COMMANDS, entityId);
			if (hasCommands) {
				CommandsComponent* commands = getComponent<CommandsComponent>(COMPONENT_COMMANDS, entityId);
				if (commands->playable) {
					playableEntities.push_back(entityId);
				}
			}
		}
		return playableEntities;
	}

private:
	entityId lastId = 0;
	map <string, vector<optional<Component*>>>components;
	map<string, Component* (*)(json)> componentTypes;
	map<entityId, entityIndex> entityIdIndex;
	stack<entityIndex> deallocatedEntityIndexes;
};