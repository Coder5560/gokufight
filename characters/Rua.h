#pragma once
#include "characters/CharacterBase.h"
#include "R.h"
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"
class Rua: public CharacterBase
{
public:
	Rua();
	~Rua();
	virtual void changeState(artemis::Entity &e);

	void actionStart(artemis::Entity &e, R::Direction direction);
	void actionStand(artemis::Entity &e);
	void actionStandUp(artemis::Entity &e);
	void actionDie(artemis::Entity &e, R::Direction direction);

	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionTrungDonNga(artemis::Entity &e, R::Direction direction);
	


	void actionMove(artemis::Entity &e, R::Direction direction);
	void actionRun(artemis::Entity &e, R::Direction direction);
	void actionJump(artemis::Entity &e, R::Direction direction);

	void actionAttack1(artemis::Entity &e, R::Direction direction);
	void actionAttack2(artemis::Entity &e, R::Direction direction);
	void actionAttack3(artemis::Entity &e, R::Direction direction);
	void actionAttack4(artemis::Entity &e, R::Direction direction);
	void actionAttack5(artemis::Entity &e, R::Direction direction);

};



