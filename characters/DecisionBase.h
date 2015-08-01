#pragma once
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"
#include "RenderLayer.h"
class AI;
class JackiechunAIKhieuKhich;

class DecisionBase
{
public:
	DecisionBase();
	~DecisionBase();
	void setWorld(artemis::World *world);
	virtual void obsever(artemis::Entity &e);
	virtual void decision(artemis::Entity &e);
	artemis::World *world;
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


class JackiechunDecision2 : public DecisionBase{
public:
	JackiechunDecision2();
	void decision(artemis::Entity &e);
	void obsever(artemis::Entity &e);
	int totalChoose;
protected:
	bool isFarFromGoku;
private:
	AI* ai;
	DecisionBase super;
};



class RandomAI : public DecisionBase{

public:
	RandomAI();
	void decision(artemis::Entity &attacker);

	void obsever(artemis::Entity &attacker);
protected :
	bool doiThuRaChieuTamXa;
	bool doiThuRaChieuGan;
	bool trongPhamViDonDanhCuaDoiThu;
	bool coTheRaChuongTamXa;
	bool nhieuMauHonDoiThu;
	float phamViSatThuongCuaDoiThu;

private:

	DecisionBase super;
};

class AI{
public:
	AI();
	virtual bool process(artemis::Entity &attacker, artemis::Entity &defenser);
	virtual void setActive(bool active);
	
	float timeActive;
	bool active;
};
class JackiechunAIKhieuKhich : public AI{
	// step 1 : vỗ tay
public :
	JackiechunAIKhieuKhich();
	bool process(artemis::Entity &attacker, artemis::Entity &defenser);
	
	bool nextStep;
	bool alldone;

};




