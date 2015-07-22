#pragma once
#include "characters/CharacterBase.h"
#include "R.h"
class Goku :public CharacterBase
{
public:
	Goku();
	~Goku();

	virtual void changeState(artemis::Entity &e);


	virtual void actionStand(artemis::Entity &e);
	void actionStandUp(artemis::Entity &e );
	void actionDie(artemis::Entity &e, R::Direction direction);
	void actionMove(artemis::Entity &e, R::Direction direction);
	void actionMoveOn(artemis::Entity &e, R::Direction direction);
	void actionRun(artemis::Entity &e, R::Direction direction);
	void actionStart(artemis::Entity &e, R::Direction direction);
	void actionTrungDon(artemis::Entity &e, R::Direction direction);
	void actionVictory(artemis::Entity &e);
	void actionBeat1(artemis::Entity &e, R::Direction direction);
	void actionBeat2(artemis::Entity &e, R::Direction direction);
	void actionBeat3(artemis::Entity &e, R::Direction direction);
	void actionJump1(artemis::Entity &e, R::Direction direction);
	void actionJump2(artemis::Entity &e, R::Direction direction);
	void actionJump3(artemis::Entity &e, R::Direction direction);
	void actionKick1(artemis::Entity &e, R::Direction direction);
	void actionKick2(artemis::Entity &e, R::Direction direction);
	void actionKick3(artemis::Entity &e, R::Direction direction);
	void actionPunch1(artemis::Entity &e, R::Direction direction);
	void actionPunch2(artemis::Entity &e, R::Direction direction);
	void actionPunch3(artemis::Entity &e, R::Direction direction);

};

