#pragma once
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/Systems.h"
class EntityUtils
{
public:
	static EntityUtils* getInstance();
	EntityUtils();
	~EntityUtils();
	void setWorld(artemis::World* world);
	void createAttackEntity(artemis::Entity &attackEntity,artemis::Component* attackComponent);
	void push(artemis::Entity &entity, float rotate, float force);
	void stopPhysic(artemis::Entity &entity);
	void clampVelocity(artemis::Entity &entity, float minSpeed, float maxSpeed);
private :
	static EntityUtils* instance;
	artemis::World* world;
};

