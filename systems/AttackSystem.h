#pragma once
#include "gokuartemis/Systems.h"
#include "gokuartemis/Components.h"
#include "renders/Effects.h"
#include "systems/EntityUtils.h"

class AttackSystem : public artemis::EntityProcessingSystem
{
public:
	AttackSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual void end();

	virtual bool attackToEntity(artemis::Entity& attackEntity, artemis::Entity &entity);

protected:
	artemis::ComponentMapper<AttackComponent> attackMapper;
	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<BoundComponent> boundMapper;
	artemis::ComponentMapper<PhysicComponent> physicMapper;
	artemis::ComponentMapper<GravityComponent> gravityMapper;
	artemis::ComponentMapper<WallSensorComponent> wallSensorMapper;

};

