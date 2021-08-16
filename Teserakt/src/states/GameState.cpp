#pragma once
#include <map>
#include <vector>
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
using namespace std;
using namespace nlohmann;

#define COMPONENT_TEXTURE "texture"
#define COMPONENT_SIZE "size"
#define COMPONENT_POSITION "position"
#define COMPONENT_COMMANDS "commands"
#define COMPONENT_PHYSICS "physics"

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
	}

	vector<entityId> entities;
	map<string, Resource*> resources;

	entityId createEntity(string fileName) {
		ifstream ifs("resources/entities/" + fileName);
		json config = json::parse(ifs);
		for (auto componentVector : components) {
			string componentType = componentVector.first;
			bool exists = config.find(componentType) != config.end();
			updateState(componentType, exists, config);
		}
		entityIndex newIndex = entities.empty() ? 1 : entities.back() + 1;
		entities.push_back(newIndex);
		entityIdIndex.emplace(newIndex, entities.size() - 1);
		return newIndex;
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
		components[componentName].assign(entityIdIndex[entityId], optional<Component*>());
	}

private:

	map <string, vector<optional<Component*>>>components;
	map<string, Component* (*)(json)> componentTypes;
	map<entityId, entityIndex> entityIdIndex;

	void updateState(string itemName, bool exists, json config) {
		for (auto typeItem : componentTypes) {
			string typeName = typeItem.first;
			auto typeConstructor = *typeItem.second;
			if (itemName == typeName) {
				if (exists) {
					auto instance = typeConstructor(config.at(itemName));
					components[typeName].push_back(optional(instance));
				}
				else {
					components[typeName].push_back(optional<Component*>());
				}
			}
		}
	}
};
