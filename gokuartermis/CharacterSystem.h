

#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/MapInfo.h"
#include "GameHud.h"
#include "spine/SkeletonAnimation.h"
#include "R.h"
#include "NodeInfo.h"
#include "gokuartermis/Systems.h"

class GokuCharacter;
class YamchaCharacter;
class CharacterCollisionSystem;
class DicisionSystem;


class CharacterSystem : public artemis::EntityProcessingSystem{
public:
	CharacterSystem();
	~CharacterSystem();

	virtual void initialize();
	virtual void initSystemInformation();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual void actionStand();
	virtual void actionStandUp();
	virtual void actionDie(R::Direction direction);
	virtual void actionMove(R::Direction direction);
	virtual void actionMoveOn(R::Direction direction);
	virtual void actionRun(R::Direction direction);
	virtual void actionStart(R::Direction direction);
	virtual void actionTrungDon(R::Direction direction);
	virtual void actionVictory();
	virtual void actionBeat(R::Direction direction);
	virtual void actionJump(R::Direction direction);
	virtual void actionKick(R::Direction direction);
	virtual void actionPunch(R::Direction direction);
	virtual void actionThink();
	virtual void actionSpecialSkill(R::Direction direction);

	R::CharacterType characterType;
protected:

	std::string animation;
	std::string animation1;
	std::string animation2;
	bool custom;

	std::string skeletonDatafile;
	std::string skeletonAtlas;
	std::string tag;

	bool initSystem;
	artemis::Entity* character;
	PhysicSystem* physicSystem;

	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<PhysicComponent> physicMapper;

	typedef CharacterSystem super;

};


class GokuCharacter : public CharacterSystem{
public:
	GokuCharacter();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void end();
		
	void actionStandUp();
	void actionDie(R::Direction direction);
	void actionMove(R::Direction direction);
	void actionMoveOn(R::Direction direction);
	void actionRun(R::Direction direction);
	void actionStart(R::Direction direction);
	void actionTrungDon(R::Direction direction);
	void actionVictory();
	void actionBeat1(R::Direction direction);
	void actionBeat2(R::Direction direction);
	void actionBeat3(R::Direction direction);
	void actionJump1(R::Direction direction);
	void actionJump2(R::Direction direction);
	void actionJump3(R::Direction direction);
	void actionKick1(R::Direction direction);
	void actionKick2(R::Direction direction);
	void actionKick3(R::Direction direction);
	void actionPunch1(R::Direction direction);
	void actionPunch2(R::Direction direction);
	void actionPunch3(R::Direction direction);

};



class YamchaCharacter : public CharacterSystem{
public:
	YamchaCharacter(std::string tag, std::string skeletonDataFile, std::string atlasFile);
	virtual void initialize();
	virtual void initSystemInformation();
	virtual void processEntity(artemis::Entity &e);

private: 
	typedef CharacterSystem super;

};

class CharacterCollisionSystem : public artemis::EntityProcessingSystem{

public:

	CharacterCollisionSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual bool isHit(artemis::Entity* e1, artemis::Entity* e2);
	virtual void end();
};

class DecisionSystem: public artemis::EntityProcessingSystem {

public:
	DecisionSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<DecisionComponent> decisionMapper;
};

