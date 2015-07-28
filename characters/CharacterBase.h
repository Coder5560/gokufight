#pragma once
#include "artemis/Artemis.h"
#include "gokuartermis/RenderLayer.h"
class CharacterBase
{
public:
	CharacterBase();
	~CharacterBase();

	virtual void changeState(artemis::Entity &e);
	virtual void actionStand(artemis::Entity &e);
};

