#pragma once
#include "characters/DecisionBase.h"
#include "gokuartemis/MapInfo.h"
class JackiechunAI : public DecisionBase
{
public:
	JackiechunAI();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	int totalChoose;
protected:
	bool isFarFromGoku;
private:
	DecisionBase super;
};

