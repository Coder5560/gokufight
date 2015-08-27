#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"
#include "gokuartemis/MapInfo.h"
#include "GameHud.h"
#include "spine/SkeletonAnimation.h"
#include "R.h"
#include "renders/NodeInfo.h"
#include "renders/Effects.h"
#include "ui/UIText.h"
#include "renders/HomeScreen.h"
#include "AdsManager.h"
#include "FacebookManager.h"
#include "CppJavaMethodManager.h"
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
class RemoveEntitySystem;
class DelaySystem;
class SkeletonCollisonSystem;
class CatFollowGokuSystem;
class BombSystem;
class CameraFollowSystem;
class SpecialSkillSystem;
class CharacterRenderSystem;
class IntroduceSystem;
class SkillSystem;
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
	virtual bool switchToLose();
	virtual void switchToAppear();
	virtual void switchToReady();
	virtual void switchToFighting();
	virtual void switchToPause();
	virtual void switchToResume();

	virtual bool checkLives();

	void switchToState(R::GameState state, artemis::Entity* e);
	bool createLoseMessage;

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
	virtual void begin();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void processInputTap(int tapCount);
	void notifyInput(Touch* touch,GameHud::EventType event, GameHud::TouchType touchType);
	float _timeLastTouch;
	int _tapCount;
	int match = 0;
	int bgIndex = 0;
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
class RemoveEntitySystem : public artemis::EntityProcessingSystem {

public:
	RemoveEntitySystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<RemoveableComponent> removeEntityMapper;
};
class DelaySystem : public artemis::EntityProcessingSystem {

public:
	DelaySystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<DelayComponent> delayMapper;
};

class SkeletonCollisonSystem : public artemis::EntityProcessingSystem{

public:
	SkeletonCollisonSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual bool checkCollision(artemis::Entity &attacker,artemis::Entity &denfeser);
protected :
	artemis::ComponentMapper<CollisionComponent> collisionMapper; 
	Vec2 collisionPoint;
};

class CatFollowGokuSystem : public artemis::EntityProcessingSystem{

public:
	CatFollowGokuSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void createBomAtack(PosComponent* positionComponent, bool isLeftDirection);

	bool prepareAttack;
	bool readyToAttack;

protected:
	artemis::ComponentMapper<PosComponent> positionMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	artemis::ComponentMapper<CatFollowComponent> catFollowMapper;
	artemis::ComponentMapper<CharacterTypeComponent> characterTypeMapper;
	
};

class BombSystem : public artemis::EntityProcessingSystem{

public:
	BombSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);

protected:
	artemis::ComponentMapper<PosComponent> positionMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	artemis::ComponentMapper<BomComponent> bombMapper;
	artemis::ComponentMapper<CharacterTypeComponent> characterTypeMapper;
};

class SpecialSkillSystem : public artemis::EntityProcessingSystem{

public:
	SpecialSkillSystem();
	virtual void initialize();
	virtual void begin();
	virtual void processEntity(artemis::Entity &e);
	virtual void processPicolo(artemis::Entity &e);
	virtual void processGoku(artemis::Entity &e);
	virtual void processCamap(artemis::Entity &e);

protected:
	artemis::ComponentMapper<PosComponent> positionMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	artemis::ComponentMapper<CharacterTypeComponent> typeMapper;
	artemis::ComponentMapper<StateComponent> stateMapper;
	artemis::ComponentMapper<CharacterInfoComponent> infoMapper;
};


class CameraFollowSystem : public artemis::EntityProcessingSystem{
	
public:
	CameraFollowSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);

protected:
	artemis::ComponentMapper<CameraFollowComponent> cameraMapper;
};


class CharacterRenderSystem : public artemis::EntityProcessingSystem {
public:
	CharacterRenderSystem();
	virtual void begin();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void onGameState(bool isPlaying);
	virtual void pauseGame();
	virtual void resumeGame();

	virtual bool checkLives();
protected:
	artemis::ComponentMapper<CharacterUIComponent> characterUIMapper;
	RemainingLife* remainingLife;
	PlayerInfoLeft* infoLeft;
	PlayerInfoRight* infoRight;
	bool isPlaying;
	ui::ImageView*	pauseIcon;
	bool isCreated;
	bool isTouch;
};

class IntroduceSystem : public artemis::EntityProcessingSystem{
public: 
	IntroduceSystem();
	virtual void begin();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	void callBackInputDone();
	void notifyInput(Touch* touch, GameHud::EventType event, GameHud::TouchType touchType);
	int _tapCount;
	float timeOnState;
	int subStep;
	bool enableInput;
	bool doneInput;
	int step;
	Node* node;
	ui::Text* text;
	ui::ImageView* button;
//	HowToPlay* howToPlay;
protected:
	artemis::ComponentMapper<IntroduceComponent> introduceMapper;
};

class SkillSystem : public artemis::EntityProcessingSystem{
public:
	SkillSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<SkillComponent> skillMapper;
};

