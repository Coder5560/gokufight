#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"
#include "RenderLayer.h"

class StateComponent;
class DecisionBase
{
public:
	DecisionBase();
	~DecisionBase();
	void setWorld(artemis::World *world);
	virtual void obsever(artemis::Entity &e);
	virtual void decision(artemis::Entity &e);
	virtual void move(StateComponent* stateComponent,R::Direction direction);
	artemis::World *world;
	bool isActive;
};
class GokuDecision : public DecisionBase{
public:
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
private:
	DecisionBase super;
};

class GiranDecision : public DecisionBase{
public:
	GiranDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	int totalChoose;
private:
	DecisionBase super;
};


class BearDecision : public DecisionBase{
public:
	BearDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	int totalChoose;
protected :
	bool isFarFromGoku;
private:
	DecisionBase super;
};

class JackiechunDecision : public DecisionBase{
public:
	JackiechunDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	int totalChoose;
protected:
	bool isFarFromGoku;
private:
	DecisionBase super;
};

class TegiacDecision : public DecisionBase{
public:
	TegiacDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
protected:
	bool doSpecificSkill;
	bool doneSpeicalSkill;
	bool isFarFromGoku;
	float timeInRageAttack;
	int solanTrungDon;

	bool doneStep;
	int currentStep;
	
private:
	DecisionBase super;
};

class RuaDecision : public DecisionBase{
public:
	RuaDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	
private:
	DecisionBase super;
};

class CamapDecision : public DecisionBase{
public:
	CamapDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	bool canAvoidGokuAttack;
	int totalAvoid;
private:
	DecisionBase super;
};

class KarillinDecision : public DecisionBase{
public:
	KarillinDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	bool canAvoidGokuAttack;
	int totalAvoid;
private:
	DecisionBase super;
};

class PicoloDecision : public DecisionBase{
public:
	PicoloDecision();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);

private:
	DecisionBase super;
};