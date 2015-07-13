#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Systems.h"
#include "gokuartermis/Components.h"
USING_NS_CC;
class ECSWorld
{
public:
	
	
	static ECSWorld* getInstance();
	static ECSWorld* instance;
	virtual void createWorld();
	void setSprite(Sprite* sprite);
	void processWorld(float delta);
	artemis::EntitySystem* setSystem(artemis::EntitySystem* system);
private:

	ECSWorld();
	~ECSWorld();
	artemis::Entity* goku;
	Sprite* sprite;
protected:
	const float STEP = 0.015f;
	float accumulate = 0;

	artemis::World* world;
	
};

