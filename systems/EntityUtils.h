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
	
	virtual void createGokuPunch1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuPunch2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuBeat1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuBeat2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuBeat3Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuKick1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuKick2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGokuKick3Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);


	virtual void createGiranAttack1(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGiranAttack2(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createGiranAttack3(artemis::Entity &attackEntity, AttackComponent* attackComponent);

	virtual void createBearAttack1(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createBearAttack2(artemis::Entity &attacker, AttackComponent* attackComponent);

	virtual void createJackiechunBeat3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createJackiechunKick2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createJackiechunKick3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createJackiechunPunch1(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createJackiechunPunch2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createJackiechunPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent);


	virtual void createTegiacBeat3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacKick2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacKick3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacPunch1(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacPunch2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacPunch3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createTegiacPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent);

	virtual void createRuaAttack1(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createRuaAttack2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createRuaAttack3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createRuaAttack4(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createRuaAttack5(artemis::Entity &attacker, AttackComponent* attackComponent);



	virtual void createCamapSkill(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createCamapPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createCamapPunch1(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createCamapPunch2(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createCamapPunch3(artemis::Entity &attacker, AttackComponent* attackComponent);
	virtual void createCamapKick2(artemis::Entity &attacker, AttackComponent* attackComponent);

	virtual void createKarilinPunch1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinPunch2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinBeat1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinBeat2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinBeat3Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinKick1Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinKick2Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createKarilinKick3Attack(artemis::Entity &attackEntity, AttackComponent* attackComponent);



	virtual void createPicoloAttack1(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createPicoloAttack2(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createPicoloAttack3(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createPicoloAttackKick(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createPicoloAttackPower1(artemis::Entity &attackEntity, AttackComponent* attackComponent);
	virtual void createPicoloAttackPower2(artemis::Entity &attackEntity, AttackComponent* attackComponent);

	virtual void removeEntity(artemis::Entity &e);
private :
	static EntityUtils* instance;
	artemis::World* world;
};

