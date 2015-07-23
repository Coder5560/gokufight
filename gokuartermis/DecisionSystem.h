

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


class DecisionSystem: public artemis::EntityProcessingSystem {

public:
	DecisionSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<DecisionComponent> decisionMapper;
};

