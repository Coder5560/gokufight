#pragma once
#include "artemis/Artemis.h"
#include "GameHud.h"
#include "spine/spine-cocos2dx.h"
#include "R.h"
#include "characters/CharacterBase.h"
#include "characters/DecisionBase.h"
#include "renders/Effects.h"

class PosComponent;
class GravityComponent;
class PhysicComponent;
class BoundComponent;
class WallSensorComponent;
class GameStateComponent;
class CharacterInfoComponent;
class SkeletonComponent;
class DecisionComponent;
class AttackComponent;
class DecisionBase;
class CallBackComponent;
class DelayComponent;
class CollisionComponent;
class SkeletonBound;
class CatFollowComponent;
class IntroduceComponent;
class SkillComponent;

class KameKameHa;



class Components
{
public:
	Components();
	~Components();
	
};


class PosComponent : public artemis::Component{
public:
	PosComponent();
	PosComponent(float posX, float posY);
	float x;
	float y;
};

class GravityComponent : public artemis::Component{

public:
	GravityComponent();
	GravityComponent(float gravityY);
	float DEFAULT_GRAVITY_Y;
	float gravityX;
	float gravityY;
	bool enable;
};


class PhysicComponent : public artemis::Component{
public:
	PhysicComponent(float vX, float vY);
	PhysicComponent();
	PhysicComponent(bool dismis);
	float vx;
	float vy;
	float vr;
	float friction;
	float bounce;
	bool isMoving;
	bool dismissWhenCollideWithWall;

};

class WallSensorComponent : public artemis::Component{

public:
	WallSensorComponent();


	bool onFloor ;
	bool onHorizontalSurface ;
	bool onVerticalSurface ;
	
	float wallAngle ;

	bool onAnysurface();
};


class BoundComponent : public artemis::Component{

public :
	BoundComponent(float width, float height);
	BoundComponent(float x1, float y1, float x2, float y2);
	float getWidth();
	float getHeight();
	float getCenterX();
	float getCenterY();

	float x1;
	float x2;
	float y1;
	float y2;
};


class GameStateComponent: public artemis::Component{

public:
	GameStateComponent();
	void setGameState(R::GameState gameState);
	R::GameState gameState;
	float time_on_state;


};

class CharacterInfoComponent : public artemis::Component{
public :

	CharacterInfoComponent();
	bool isMainCharacter;
	std::string avatar;
	std::string name;

	float MAX_BLOOD;
	float MAX_POWER;
	float blood;
	float power;

	float NORMAL_SKILL_POWER;
	float SPECIAL_SKILL_POWER;

	// kiểm tra xem còn đủ mana để ra chưởng hay không
	bool notifyMana;
	void notifyNotEnoughMana();
	bool hasManaForSkill(float skillPower);

};

class SkeletonComponent : public artemis::Component{
public :
	SkeletonComponent();

	spine::SkeletonAnimation* skeleton;
	Node* node;

	std::string dataFile;
	std::string atlasFile;
	std::string animation;
	std::string skin;
	

	
	float timeScale;
	bool isCreated;
	bool loop_animation;
	bool scaleX;
	bool changeAnimation;
	bool changeSkin;
	bool resetSkeletonData;
	bool visible;
};
class DecisionComponent : public artemis::Component{
public :
	DecisionComponent();
	void setActive(bool active);

	DecisionBase* decisionBase;
	//thời gian mà entity đã suy nghĩ.
	float thinkingTime;
	// thời gian ra quyết định.
	float DECISION_TIME;

};
class StateComponent : public artemis::Component{
public:
	
	StateComponent();

	void setState(R::CharacterState newState);

	CharacterBase* characterBase;
	R::Direction direction;
	R::CharacterState state;
	R::Attack attack;
	R::Defense defense;

	int totalHit;
	int totalAttack;
	float time_on_state;
	int trungdonlientiep;
	int hitDetected;
	bool doneAction;
	bool jump;
};

class CharacterTypeComponent : public artemis::Component{
public :
	CharacterTypeComponent();
	CharacterTypeComponent(R::CharacterType name);
	R::CharacterType type;

};
class AttackComponent : public artemis::Component{
public :
	AttackComponent();
	R::Attack type;
	R::CharacterType whoAttack;
	float powerOfAttack;
	float manaOfAttack;
	// còn hiệu lực hay ko
	bool expire;
	bool isSpecialSkill;
	// for range attack
	float minX;
	float maxX;
	float minY;
	float maxY;
};

class RemoveableComponent : public artemis::Component{
public:
	RemoveableComponent();
	bool haveToRemove;
};

class DelayComponent : public artemis::Component{
public :
	DelayComponent();
	float timeAlive;
	float timeDelay;
	std::function<void()> callBack;
	void setCallBack(const std::function<void()> &callBack);
};

class CollisionComponent : public artemis::Component{
public :
	CollisionComponent();



};
class SkeletonBound{
public :
	SkeletonBound(std::string boundingName, std::string boneName, std::string slotName);
	std::string boundingbox;
	std::string boneName;
	std::string slotName;

};
class CatFollowComponent : public artemis::Component{
public:
	CatFollowComponent();
	void setState(R::CatFollowState newState);

	R::CatFollowState state;
	float timeOnState;
	float nextTimeAttack;
};

class BomComponent : public artemis::Component{
public:
	BomComponent();
	bool expire;
	float powerOfAttack;
};

class CameraFollowComponent : public artemis::Component{
public:
	CameraFollowComponent();
	R::CameraState state;
	float timeOnState;
	void setState(R::CameraState newState);
};
class CharacterUIComponent : public artemis::Component{
public:
	CharacterUIComponent();

	R::CharacterUIState state;
	float timeOnState;
	void setState(R::CharacterUIState newState);
};

class IntroduceComponent : public artemis::Component{
public:
	IntroduceComponent();

	R::IntroduceStep step;
	float timeOnState;
	bool isDoneStep;
	bool isDoneAll;
	void setStep(R::IntroduceStep newStep);
};


class SkillComponent : public artemis::Component{
public:
	SkillComponent(KameKameHa* kamekameha);
	~SkillComponent();
	KameKameHa* kamekameha;
};