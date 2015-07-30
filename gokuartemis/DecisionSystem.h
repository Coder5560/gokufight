

#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"
#include "gokuartemis/MapInfo.h"
#include "GameHud.h"
#include "spine/SkeletonAnimation.h"
#include "R.h"
#include "NodeInfo.h"
#include "gokuartemis/Systems.h"


class DecisionSystem: public artemis::EntityProcessingSystem {

public:
	DecisionSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<DecisionComponent> decisionMapper;
};

