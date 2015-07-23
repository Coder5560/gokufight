#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Systems.h"
#include "gokuartermis/Components.h"
#include "systems/EntityUtils.h"
#include "characters/DecisionBase.h"
USING_NS_CC;
class ECSWorld
{
public:
	
	
	static ECSWorld* getInstance();
	static ECSWorld* instance;
	virtual void createWorld();
	
	void processWorld(float delta);
	artemis::EntitySystem* setSystem(artemis::EntitySystem* system);

	InputSystem* inputSystem;
private:
	void createMainCharacter();
	void createEnemyCharacter();
	ECSWorld();
	~ECSWorld();
	
	artemis::Entity* goku;
	
protected:
	const float STEP = 0.015f;
	float accumulate = 0;

	artemis::World* world;
	
};

