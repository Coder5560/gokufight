#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/Systems.h"

ECSWorld* ECSWorld::instance = NULL;
ECSWorld::ECSWorld() : goku(nullptr), world(nullptr)
{
}
ECSWorld* ECSWorld::getInstance(){
	if (instance == NULL){
		instance = new ECSWorld();
	}
	return instance;
}


ECSWorld::~ECSWorld()
{
	delete world;
}



void ECSWorld::createWorld(){
	world = new artemis::World();
	
	
	

	//artemis::Entity& e = world->getTagManager()->getEntity("goku");
	inputSystem = new InputSystem();
	// Tống vào world mấy cái system cho nó xử lý.
	// importance step, the way orgnize system affect to how system running.
	
	setSystem(new GokuProcessingSystem());
	setSystem(new GameStateSystem());
	setSystem(inputSystem);
	
	setSystem(new YamchaCharacter("yamcha", "spine/skeleton.json", "spine/skeleton.atlas"));
	setSystem(new PhysicSystem());
	setSystem(new GravitySystem());
	setSystem(new MapCollisionSystem());
	setSystem(new WallSensorSystem());
	setSystem(new AfterPhysicSystem());
	setSystem(new SkeletonSystem());
	setSystem(new CharacterCollisionSystem());
	setSystem(new UICharacterSystem());

	world->getSystemManager()->initializeAll();
	
}


artemis::EntitySystem*  ECSWorld::setSystem(artemis::EntitySystem* system){
	return world->getSystemManager()->setSystem(system);
}

void ECSWorld::processWorld(float delta){
	if (world == nullptr) return;
	accumulate += delta;
	while (accumulate > STEP){
		world->setDelta(STEP);
		world->loopStart();
		world->processWorld();
		accumulate -= STEP;
	}
	
}
