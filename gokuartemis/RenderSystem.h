#pragma once

#include "cocos2d.h"
#include "artemis/artemis.h"
#include "gokuartemis/Systems.h"
#include "gokuartemis/Components.h"
#include "gokuartemis/RenderComponent.h"
#include "R.h"
USING_NS_CC;

class RenderSystem : public artemis::EntityProcessingSystem
{
public:
	RenderSystem();
	~RenderSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity& e);
	void renderBlood(artemis::Entity &e, BloodComponent* bloodComponent, PosComponent* posCompoent);
protected :
	artemis::ComponentMapper<RenderComponent> renderMapper;
	artemis::ComponentMapper<PosComponent> posMapper;

};

