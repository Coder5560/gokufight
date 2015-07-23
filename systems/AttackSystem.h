#pragma once
#include "gokuartermis/Systems.h"
#include "gokuartermis/Components.h"

class AttackSystem : public artemis::EntityProcessingSystem
{
public:
	AttackSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual void end();

	virtual bool attackToEntity(artemis::Entity& attackEntity, artemis::Entity &entity);

protected :
	artemis::ComponentMapper<AttackComponent> attackMapper;
	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<BoundComponent> boundMapper;
};
