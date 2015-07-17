#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/MapInfo.h"
#include "GameHud.h"
#include "spine/SkeletonAnimation.h"
#include "R.h"
USING_NS_CC;

class GravitySystem;
class PhysicSystem;
class AfterPhysicSystem;
class WallSensorSystem;
class GameStateSystem;
class MapCollisionSystem;
class InputSystem;
class SkeletonSystem;
class GokuProcessingSystem;
class CharacterProcessingSystem;
class YamchaCharacter;
class CharacterCollisionSystem;

class Systems
{
public:
	Systems();
	~Systems();
};

class GravitySystem : public artemis::EntityProcessingSystem{

public:
	GravitySystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();

private:
	float GRAVITY_FACTOR;
	artemis::ComponentMapper<PhysicComponent> pm;
	artemis::ComponentMapper<GravityComponent> gm;
};

class PhysicSystem : public artemis::EntityProcessingSystem{
public:
	PhysicSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void push(artemis::Entity &entity, float rotate, float force);
	void stopPhysic(artemis::Entity &entity);
	void clampVelocity(artemis::Entity &entity,float minSpeed, float maxSpeed);

private:
	
	artemis::ComponentMapper<PhysicComponent> physicMapper;

};



class AfterPhysicSystem : public artemis::EntityProcessingSystem{
public:
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

	WallSensorSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();

private:
	MapInfo* mapInfo;
	artemis::ComponentMapper<PosComponent> pom;
	artemis::ComponentMapper<WallSensorComponent> wm;
	artemis::ComponentMapper<BoundComponent> bm;
	artemis::ComponentMapper<PhysicComponent> physicMapper;
};

class GameStateSystem : public artemis::EntityProcessingSystem{
public:
	GameStateSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();
	void switchToState(GameStateComponent::GameState state, artemis::Entity* e);
private:

	artemis::ComponentMapper<GameStateComponent> gameStateMapper;
	// thời gian kể từ lúc state bắt đầu kích hoạt
	float time_stay_on_state;

};

class MapCollisionSystem : public artemis::EntityProcessingSystem{
public:
	MapCollisionSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();
	
private:
	MapInfo* mapInfo;
	artemis::ComponentMapper<PhysicComponent> physicMapper;
	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<BoundComponent> boundMapper;
};

class InputSystem : public artemis::EntityProcessingSystem{
public :
	InputSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void notifyInput(GameHud::EventType event, GameHud::TouchType touchType);
	
	
};
class GokuProcessingSystem : public artemis::EntityProcessingSystem{
public:
	GokuProcessingSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void end();

	void actionStand();
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

	


private :

	artemis::Entity* goku;
	PhysicSystem* physicSystem;
	
	//spine::SkeletonAnimation* skeletonAnimation;
	//Node* node;
	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<PhysicComponent> physicMapper;
};

class SkeletonSystem : public artemis::EntityProcessingSystem{

public:
	SkeletonSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();

private:

	artemis::ComponentMapper<PhysicComponent> physicMapper;
	artemis::ComponentMapper<BoundComponent> boundMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	artemis::ComponentMapper<PosComponent> positionMapper;
	
};




class CharacterProcessingSystem : public artemis::EntityProcessingSystem{
public:
	CharacterProcessingSystem();
	~CharacterProcessingSystem();

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



protected:
	std::string skeletonDatafile;
	std::string skeletonAtlas;
	std::string tag;
	


	bool initSystem;
	artemis::Entity* character;
	PhysicSystem* physicSystem;

	spine::SkeletonAnimation* skeletonAnimation;
	Node* node;
	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<PhysicComponent> physicMapper;
};

class YamchaCharacter : public CharacterProcessingSystem{

public:
	
	YamchaCharacter(std::string tag, std::string skeletonDataFile, std::string atlasFile);
	
	virtual void initialize();
	virtual void initSystemInformation();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	
	virtual void actionTrungDon(R::Direction direction);
};
class CharacterCollisionSystem : public artemis::EntityProcessingSystem{

public:

	CharacterCollisionSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual bool isHit(artemis::Entity* e1, artemis::Entity* e2);
	virtual void end();
};