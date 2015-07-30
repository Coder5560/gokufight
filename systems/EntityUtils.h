#pragma once
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"
#include "gokuartemis/Systems.h"
class EntityUtils
{
public:
	static EntityUtils* getInstance();
	EntityUtils();
	~EntityUtils();
	void setWorld(artemis::World* world);
	
	void push(artemis::Entity &entity, float rotate, float force);
	void stopPhysic(artemis::Entity &entity);
	void clampVelocity(artemis::Entity &entity, float minSpeed, float maxSpeed);


	void createAttackEntity(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuPunchAttack(artemis::Entity &attackEntity,AttackComponent* attackComponent);
	virtual void createGokuPunch1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuBeatAttack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuKickAttack(artemis::Entity &attackEntity, AttackComponent* attackComponent);

	virtual void createGiranAttack1(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGiranAttack2(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGiranAttack3(artemis::Entity &attackEntity, AttackComponent* attackComponent);

	virtual void removeEntity(artemis::Entity &e);
private :
	static EntityUtils* instance;
	artemis::World* world;
};

