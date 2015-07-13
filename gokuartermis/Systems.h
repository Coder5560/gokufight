#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"
USING_NS_CC;

class GravitySystem;
class AfterPhysicSystem;
class WallSensorSystem;

class Systems
{
public:
	Systems();
	~Systems();
};

class GravitySystem : public artemis::EntityProcessingSystem{

public :
	GravitySystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();

private :
	const float GRAVITY_FACTOR = 20;
	artemis::ComponentMapper<PosComponent> pm;
	artemis::ComponentMapper<GravityComponent> gm;
};

class AfterPhysicSystem : public artemis::EntityProcessingSystem{
public :
	AfterPhysicSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();
private:
	artemis::ComponentMapper<PosComponent> psm;
	artemis::ComponentMapper<PhysicComponent> pym;
	artemis::ComponentMapper<WallSensorComponent> wm;
};

class WallSensorSystem : public artemis::EntityProcessingSystem{
public:
	 float GROUND ;
	 float WALL_LEFT;
	 float WALL_RIGHT;
	  float WALL_UP;


	WallSensorSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();
	bool checkCollide(float x, float y);
private :
	
	artemis::ComponentMapper<PosComponent> pom;
	artemis::ComponentMapper<PhysicComponent> pym;
	artemis::ComponentMapper<WallSensorComponent> wm;
	artemis::ComponentMapper<BoundComponent> bm;
};

