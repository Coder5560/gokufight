#pragma once
#include "characters/DecisionBase.h"
#include "gokuartemis/MapInfo.h"
class KarillinAI : public DecisionBase
{
public:
	KarillinAI();
	~KarillinAI();

	void obsever(artemis::Entity &e);
	void processInRangeAttack();
	void processOutRangeAttack();
	void processAvoidAttack();
	virtual void closeRangeAttack();
	virtual void moveAway();

protected:

	MapInfo* mapInfo;

	PosComponent* characterPosition;
	BoundComponent* characterBound;
	StateComponent* characterState;
	CharacterInfoComponent* characterInfo;
	PhysicComponent* characterPhysic;
	SkeletonComponent* characterSkeleton;

	PosComponent* targetPosition;
	BoundComponent* targetBound;
	StateComponent* targetState;
	CharacterInfoComponent* targetInfo;
	PhysicComponent* targetPhysic;
	SkeletonComponent* targetSkeleton;

};

