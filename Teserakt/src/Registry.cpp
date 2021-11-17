#include "Registry.h"

Registry::Registry()
{
	Registry* registry = this;
	applicationState = new ApplicationState();
	gameState = new GameState();
	windowState = new WindowState();
	cameraState = new CameraState();
	inputSystem = new InputSystem(this);
	aiSystem = new AISystem(this);
	commandSystem = new CommandSystem(this);
	physicsSystem = new PhysicsSystem(this);
	deallocatorSystem = new DeallocatorSystem(this);
	renderSystem = new RenderSystem(this);
	systems = {
		inputSystem,
		aiSystem,
		commandSystem,
		physicsSystem,
		deallocatorSystem,
		renderSystem,
	};
}
