#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartermis/Components.h"

class DecisionBase
{
public:
	DecisionBase();
	~DecisionBase();
	void setWorld(artemis::World *world);
	virtual void obsever(artemis::Entity &e);
	virtual void decision(artemis::Entity &e);
	artemis::World *world;
};
class GokuDecision : public DecisionBase{
public : 
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
};

class GiranDecision : public DecisionBase{
public: 
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
};


