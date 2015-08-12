#pragma once
#include "characters/CharacterBase.h"
#include "R.h"
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "systems/EntityUtils.h"
class CaMap : public CharacterBase
{
public:
	CaMap();
	~CaMap();


	virtual void changeState(artemis::Entity &e);

	void actionStart(artemis::Entity &e, R::Direction direction);
	void actionStand(artemis::Entity &e);
	void actionStandUp(artemis::Entity &e);
	void actionDie(artemis::Entity &e, R::Direction direction);

	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionTrungDonNga(artemis::Entity &e, R::Direction direction);
	void actionBack(artemis::Entity &e, R::Direction direction);

	void actionMove(artemis::Entity &e, R::Direction direction);
	// thằng này có 2 loại Jump, 1 loại jump bình thường, một loại xoay vòng
	void actionJump(artemis::Entity &e, R::Direction direction);

	void actionSkill(artemis::Entity &e, R::Direction direction);
	void actionPunchAir(artemis::Entity &e, R::Direction direction);
	void actionPunch1(artemis::Entity &e, R::Direction direction);
	void actionPunch2(artemis::Entity &e, R::Direction direction);
	void actionPunch3(artemis::Entity &e, R::Direction direction);
	void actionKick2(artemis::Entity &e, R::Direction direction);
	
};

