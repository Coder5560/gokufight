#pragma once
#include "characters/DecisionBase.h"

class AITegiac: public DecisionBase{
public:
	AITegiac();
	void obsever(artemis::Entity &e);

	void processSkill();
	void processAvoidAttack();
	void processCloseRangeAtaack();
	void processMove();
	void processJump();
	void processNormal();

protected:
	bool skill;
	bool moveBack;
	bool moveOn;
	bool avoidAttack;
	bool jump;
	bool closeRangeAttack;


	int countBack;
	float backDistance;
	float offsetMoveback;

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
private:
	DecisionBase super;
};