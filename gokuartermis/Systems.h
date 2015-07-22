#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/MapInfo.h"
#include "GameHud.h"
#include "spine/SkeletonAnimation.h"
#include "R.h"
#include "NodeInfo.h"
USING_NS_CC;

class GravitySystem;
class PhysicSystem;
class MotionSystem;
class WallSensorSystem;
class GameStateSystem;
class MapCollisionSystem;
class InputSystem;
class SkeletonSystem;
class DebugSystem;
class UICharacterSystem;

class Systems {
public:
	Systems();
	~Systems();
};

class GravitySystem : public artemis::EntityProcessingSystem {

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

class PhysicSystem : public artemis::EntityProcessingSystem {
public:
	PhysicSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void push(artemis::Entity &entity, float rotate, float force);
	void stopPhysic(artemis::Entity &entity);
	void clampVelocity(artemis::Entity &entity, float minSpeed, float maxSpeed);

private:

	artemis::ComponentMapper<PhysicComponent> physicMapper;

};

class MotionSystem : public artemis::EntityProcessingSystem {
public:
	MotionSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();
private:
	artemis::ComponentMapper<PosComponent> psm;
	artemis::ComponentMapper<PhysicComponent> pym;
};

class WallSensorSystem : public artemis::EntityProcessingSystem {
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

class GameStateSystem : public artemis::EntityProcessingSystem {
public:
	GameStateSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkProcessing();
	virtual void end();

	virtual void switchToWin();
	virtual void switchToLose();
	virtual void switchToReady();
	virtual void switchToFighting();
	virtual void switchToPause();
	virtual void switchToResume();

	void switchToState(GameStateComponent::GameState state, artemis::Entity* e);
private:

	artemis::ComponentMapper<GameStateComponent> gameStateMapper;
	// thời gian kể từ lúc state bắt đầu kích hoạt
	float time_stay_on_state;

};

class MapCollisionSystem : public artemis::EntityProcessingSystem {
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

class InputSystem : public artemis::EntityProcessingSystem {
public:
	InputSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void notifyInput(GameHud::EventType event, GameHud::TouchType touchType);

};

class SkeletonSystem : public artemis::EntityProcessingSystem {

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

class UICharacterSystem : public artemis::EntityProcessingSystem {

public:
	UICharacterSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void createNodeForCharacter(CharacterInfoComponent* characterInfo);
	virtual void processNodeForCharacter(CharacterInfoComponent* characterInfo);
protected:
	std::map<std::string, NodeInfo*> renderObjects;
	artemis::ComponentMapper<CharacterInfoComponent> characterInfoMapper;
};


class DebugSystem : public artemis::EntityProcessingSystem {

public:
	DebugSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
protected:

	artemis::ComponentMapper<PosComponent> posMapper;
	artemis::ComponentMapper<BoundComponent> boundMapper;

};