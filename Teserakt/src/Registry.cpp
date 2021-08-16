#include "Registry.h"

Registry::Registry()
{
	Registry* registry = this;
	applicationState = new ApplicationState();
	gameState = new GameState();
	windowState = new WindowState();
	cameraState = new CameraState();
	commandSystem = new CommandSystem(this);
	renderSystem = new RenderSystem(this);
	inputSystem = new InputSystem(this);
	physicsSystem = new PhysicsSystem(this);
	systems = { commandSystem,inputSystem,renderSystem,physicsSystem };
}
