#pragma once
#include "artemis/Artemis.h"
#include "GameHud.h"
#include "spine/spine-cocos2dx.h"
#include "R.h"
#include "characters/CharacterBase.h"
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
	float vx;
	float vy;
	float vr;
	float friction;
	float bounce;
	bool isMoving;

};

class WallSensorComponent : public artemis::Component{

public:
	WallSensorComponent();
	bool onFloor = false;
	bool onHorizontalSurface = false;
	bool onVerticalSurface = false;
	
	float wallAngle = 0;

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
	enum GameState{
		// đây là bước chuẩn bị vào game, mọi thao tác trong game chưa hoạt động, các sự kiện diễn ra trên nền UI.
		PREPARE,
		// Chuẩn bị vào trận đấu
		ANIMATING_TO_FIGHT,
		// Hoạt cảnh thắng
		WIN_EFFECT,
		// Hoạt cảnh thua
		LOSE_EFFECT,
		// Hoạt cảnh chọn người chơi
		CHOSE_PLAYER,
		// Hoạt cảnh lúc chiến đấu.
		FIGHTING,
	};
	GameState gameState;
};

class CharacterInfoComponent : public artemis::Component{
public :

	CharacterInfoComponent();
	R::CharacterState state;
	std::string tag;
	bool isMainCharacter;
	float timeOnState;
	float timeRadon;

	float MAX_BLOOD;
	float MAX_POWER;
	float blood;
	float power;
	
	float skill_a_power;
	float skill_b_power;
	float skill_x_power;

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
	float time_on_state;

	bool customAnimation;
	std::vector<std::string> animations;

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
	R::CharacterType whoAttack;
	bool expire;
	float powerOfAttack;
	float timeAttack;
	float collisionPointX;
	float collisionPointY;
};
