#include "TestECS.h"


TestECS::TestECS(artemis::World* world)
{
	
	this->world = world;


	artemis::SystemManager *sm = world->getSystemManager();
	mvSystem = (MovementSystem*)sm->setSystem(new MovementSystem());
	artemis::EntityManager* em = world->getEntityManager();
	sm->initializeAll();

	player = &(em->create());
	player->addComponent(new VelocityComponent(10,0));
	player->addComponent(new PositionComponent(400,300));
	player->refresh();
	
}

void TestECS::update(float delta){
	//mvSystem->process();
}

TestECS::~TestECS()
{

}

VelocityComponent::VelocityComponent(float vX, float vY){
	this->_VelocityX = vX;
	this->_VelocityY = vY;
}

PositionComponent::PositionComponent(float posX, float posY){
	this->_PosX = posX;
	this->_PosY = posY;
}

MovementSystem::MovementSystem(){
	addComponentType<VelocityComponent>();
	addComponentType<PositionComponent>();
}

void MovementSystem::initialize(){
	vm.init(*world);
	pm.init(*world);
}

void MovementSystem::processEntity(artemis::Entity &e){
	pm.get(e)->_PosX += vm.get(e)->_VelocityX*world->getDelta();
	pm.get(e)->_PosY += vm.get(e)->_VelocityY*world->getDelta();
}