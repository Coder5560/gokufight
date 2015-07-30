#pragma once
#include "characters/CharacterBase.h"
#include "R.h"
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"

class Giran :public CharacterBase
{
public:
	Giran();
	~Giran();

	virtual void changeState(artemis::Entity &e);

	void actionStand(artemis::Entity &e);
	void actionDie(artemis::Entity &e, R::Direction direction);
	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionMove(artemis::Entity &e, R::Direction direction);
	void actionMoveOn(artemis::Entity &e, R::Direction direction);
	void actionJump(artemis::Entity &e, R::Direction direction);
	
	void actionPunch1(artemis::Entity &e, R::Direction direction);
	void actionPunch2(artemis::Entity &e, R::Direction direction);
	void actionPunch3(artemis::Entity &e, R::Direction direction);
};