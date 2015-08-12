#pragma once
#include "artemis/Artemis.h"
#include "gokuartemis/RenderLayer.h"
class CharacterBase
{
public:
	CharacterBase();
	~CharacterBase();
	virtual void process();
	virtual void changeState(artemis::Entity &e);
	virtual void actionStand(artemis::Entity &e);
};

