#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "R.h"
#include "gokuartemis/Systems.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"
#include "characters/DecisionBase.h"
#include "gokuartemis/DecisionSystem.h"
#include "systems/CharacterStateSystem.h"
#include "systems/AttackSystem.h"
#include "RenderSystem.h"
#include "Characters/Goku.h"
#include "Characters/Giran.h"
#include "Characters/Bear.h"
#include "Characters/Tegiac.h"
#include "Characters/AITegiac.h"
#include "Characters/Jackiechun.h"
#include "Characters/CaMap.h"
#include "Characters/Rua.h"
#include "Characters/Karillin.h"
#include "Characters/Picolo.h"
#include "Characters/EnemyNormalAI.h"
#include "Characters/EnemyAttackAI.h"
#include "Characters/KarillinAI.h"
#include "AdsManager.h"


USING_NS_CC;

class ECSWorld
{
public:
	static ECSWorld* getInstance();
	static ECSWorld* instance;
	virtual void createWorld(R::Match_Type matchType);
	virtual void resetCurrentMatch();
	virtual void resetCurrentMatch(bool isReplayGame);
	virtual void nextMatch();
	virtual void notifyInput(Touch* touch, GameHud::EventType event, GameHud::TouchType touchType);
	artemis::World* getWorld();

	void ignoreWorld(bool ignore);
	void processWorld(float delta);
	artemis::EntitySystem* setSystem(artemis::EntitySystem* system);
	InputSystem* inputSystem;
	IntroduceSystem* introduceSystem;
	void createMainCharacter();
	void createEnemyCharacter();
	
	R::Match_Type matchType;
private:
	
	void createPicoloCharacter();
	void createKarillinCharacter();
	void createGiranCharacter();
	void createBearCharacter();
	void createBearIntroduceCharacter();
	void createJackiechunCharacter();
	void createTegiacCharacter();
	void createCamapCharacter();
	void createRuaCharacter();
	void createEnemyAssistant();
	void createCameraFollowEntity();
	ECSWorld();
	~ECSWorld();
	GameHud* gameHud;
	artemis::Entity* goku;
	
protected:
	bool isReplayGame;
	bool isIgnoreWorld;
	const float STEP;
	float accumulate;

	artemis::World* world;
	
};

