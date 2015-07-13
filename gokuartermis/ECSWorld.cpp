#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/Systems.h"

ECSWorld* ECSWorld::instance = NULL;
ECSWorld::ECSWorld() : goku(nullptr), sprite(nullptr), world(nullptr)
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
	

	// Tống vào world mấy cái system cho nó xử lý.
	setSystem(new GravitySystem());
	setSystem(new AfterPhysicSystem());
	setSystem(new WallSensorSystem());
	world->getSystemManager()->initializeAll();

	// Thêm một cái entity cho nó xôm

	goku = &(world->getEntityManager()->create());

	goku->addComponent(new PosComponent(400,300));
	goku->addComponent(new BoundComponent(0,0, 0,0));
	goku->addComponent(new WallSensorComponent());
	goku->addComponent(new GravityComponent());
	goku->addComponent(new PhysicComponent(100, 0));
	

	goku->refresh();


	
}

void ECSWorld::setSprite(Sprite* sprite){
	this->sprite = sprite;
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
	if (sprite != nullptr){
		PosComponent* position = (PosComponent*)(goku->getComponent<PosComponent>());
		sprite->setPosition(Vec2(position->x, position->y));
	}
}
