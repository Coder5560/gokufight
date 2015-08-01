#pragma once

#include "characters/CharacterBase.h"
#include "R.h"
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"
class Bear :public CharacterBase
{
public:
	Bear();
	~Bear();

	virtual void changeState(artemis::Entity &e);

	void actionStand(artemis::Entity &e);
	void actionDie(artemis::Entity &e, R::Direction direction);
	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionMove(artemis::Entity &e, R::Direction direction);

	void actionAttack1(artemis::Entity &e, R::Direction direction);
	void actionAttack2(artemis::Entity &e, R::Direction direction);

};

