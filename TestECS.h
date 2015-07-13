#pragma once

#include "artemis/Artemis.h"
#include "cocos2d.h"
USING_NS_CC;

class VelocityComponent;
class PositionComponent;
class MovementSystem;


class TestECS
{
public:
	TestECS(artemis::World* world);
	~TestECS();
	virtual void update(float delta);

	artemis::World* world;
	MovementSystem* mvSystem;
	artemis::Entity* player;
	
};

class VelocityComponent : public artemis::Component{

public :
	VelocityComponent(float vX, float vY);
	float _VelocityX;
	float _VelocityY;
	
};

class PositionComponent: public artemis::Component{

public :
	PositionComponent(float posX, float posY);
	float _PosX;
	float _PosY;
	

};


class MovementSystem : public artemis::EntityProcessingSystem{
public :
	MovementSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<VelocityComponent> vm;
	artemis::ComponentMapper<PositionComponent> pm;

};

