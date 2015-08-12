#pragma once
#include "characters/CharacterBase.h"
#include "R.h"
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"
class Picolo : public CharacterBase
{
public:
	Picolo();
	~Picolo();
	virtual void changeState(artemis::Entity &e);

	void actionStart(artemis::Entity &e, R::Direction direction);
	void actionStand(artemis::Entity &e);
	void actionStandUp(artemis::Entity &e);
	void actionDie(artemis::Entity &e, R::Direction direction);

	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionTrungDonNga(artemis::Entity &e, R::Direction direction);
	void actionMove(artemis::Entity &e, R::Direction direction);


	void actionAttack1(artemis::Entity &e, R::Direction direction);
	void actionAttack2(artemis::Entity &e, R::Direction direction);
	void actionAttack3(artemis::Entity &e, R::Direction direction);
	void actionAttackKick(artemis::Entity &e, R::Direction direction);
	void actionAttackPower1(artemis::Entity &e, R::Direction direction);
	void actionAttackPower2(artemis::Entity &e, R::Direction direction);

};
