#pragma once
#include "artemis/Artemis.h"
#include "gokuartermis/Systems.h"


class CharacterStateSystem : public artemis::EntityProcessingSystem
{
public:
	CharacterStateSystem();
	~CharacterStateSystem();

	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
	virtual void changeState(artemis::Entity &e);



	virtual void actionStand(artemis::Entity &e);
	virtual void actionMove(artemis::Entity &e);
	virtual void actionMoveOn(artemis::Entity &e);
	virtual void actionJump(artemis::Entity &e);
	virtual void actionKick(artemis::Entity &e);
	virtual void actionBeat(artemis::Entity &e);
	virtual void actionPunch(artemis::Entity &e);
	virtual void actionAttact(artemis::Entity &e);
	virtual void actionDie(artemis::Entity &e);

protected:
	artemis::ComponentMapper<StateComponent> stateMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	
};

