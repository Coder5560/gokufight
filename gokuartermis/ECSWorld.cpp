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
	
	 inputSystem = new InputSystem();
	// Tống vào world mấy cái system cho nó xử lý.
	// importance step, the way orgnize system affect to how system running.
	setSystem(new GameStateSystem());
	setSystem(new GravitySystem());
	setSystem(new PhysicSystem());
	setSystem(new MapCollisionSystem());
	setSystem(new AfterPhysicSystem());
	setSystem(new WallSensorSystem());
	setSystem(new AfterPhysicSystem());
	setSystem(inputSystem);
	
	world->getSystemManager()->initializeAll();
	// tạo những component dùng chung cho nhiều entity
	GameStateComponent* gameStateComponent = new GameStateComponent();


	// Thêm một cái entity cho nó xôm
	goku = &(world->getEntityManager()->create());

	goku->addComponent(new PosComponent(400,300));
	goku->addComponent(new BoundComponent(0,0,60,60));
	goku->addComponent(new WallSensorComponent());
	goku->addComponent(new GravityComponent());
	goku->addComponent(new PhysicComponent());
	goku->addComponent(gameStateComponent);
	goku->setTag("goku");
	goku->refresh();

	//artemis::Entity& e = world->getTagManager()->getEntity("goku");
	
	
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
