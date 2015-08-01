#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
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
#include "Characters/Jackiechun.h"
#include "R.h"
USING_NS_CC;

class ECSWorld
{
public:
	static ECSWorld* getInstance();
	static ECSWorld* instance;
	virtual void createWorld(R::Match_Type matchType);
	artemis::World* getWorld();

	void processWorld(float delta);
	artemis::EntitySystem* setSystem(artemis::EntitySystem* system);
	InputSystem* inputSystem;
	void createMainCharacter();
	void createEnemyCharacter();
	

	R::Match_Type matchType;
private:
	void createGiranCharacter();
	void createBearCharacter();
	void createJackiechunCharacter();
	ECSWorld();
	~ECSWorld();
	GameHud* gameHud;
	artemis::Entity* goku;
	
protected:
	const float STEP = 0.015f;
	float accumulate = 0;

	artemis::World* world;
	
};

