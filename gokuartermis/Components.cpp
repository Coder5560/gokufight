#include "Components.h"
#include "cocos2d.h"

USING_NS_CC;

Components::Components(){}


Components::~Components(){}

PosComponent::PosComponent() :x(0), y(0){}
PosComponent::PosComponent(float posX, float posY){
	this->x = posX;
	this->y = posY;
}

GravityComponent::GravityComponent(){
	DEFAULT_GRAVITY_Y = -9.8f;
	enable = true;
	gravityX = 0;
	this->gravityY = DEFAULT_GRAVITY_Y;

}
GravityComponent::GravityComponent(float gravityY){
	DEFAULT_GRAVITY_Y = -9.8f;
	enable = true;
	gravityX = 0;
	this->gravityY = gravityY;

}
PhysicComponent::PhysicComponent(bool dismis){
	this->dismissWhenCollideWithWall = dismis;
	vx = 0;
	vy = 0;
	vr = 0;
	friction = 6;
	bounce = .5;
	isMoving = false;
}

PhysicComponent::PhysicComponent(){
	vx = 0;
	vy = 0;
	vr = 0;
	friction = 6;
	bounce = .5;
	isMoving = false;
	this->dismissWhenCollideWithWall = false;
}
PhysicComponent::PhysicComponent(float vX, float vY){
	this->vx = vX;
	this->vy = vY;
	vr = 0;
	friction = 6;
	bounce = .5;
	isMoving = false;
	this->dismissWhenCollideWithWall = false;
}
WallSensorComponent::WallSensorComponent() : onFloor(false), onVerticalSurface(false), onHorizontalSurface(false), wallAngle(0){}


bool WallSensorComponent::onAnysurface(){
	return onHorizontalSurface || onVerticalSurface;
}


BoundComponent::BoundComponent(float width, float height){
	this->x1 = 0;
	this->x2 = width;
	this->y1 = 0;
	this->y2 = height;
}
BoundComponent::BoundComponent(float x1, float y1, float x2, float y2){
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}
float BoundComponent::getWidth(){
	return abs(x2 - x1);
}
float BoundComponent::getHeight(){
	return abs(y2 - y1);
}
float BoundComponent::getCenterX(){
	return x1 + (x2 - x1) / 2;
}
float BoundComponent::getCenterY(){
	return y1 + (y2 - y1) / 2;
}


GameStateComponent::GameStateComponent() : gameState(R::GameState::NONE), time_on_state(0) {}
void GameStateComponent::setGameState(R::GameState gameState){ this->gameState = gameState; time_on_state = 0; }

SkeletonComponent::SkeletonComponent(){
	skeleton = nullptr;
	node = nullptr;

	dataFile = "";
	atlasFile = "";
	animation = "";
	skin = "";

	timeScale = 1;
	isCreated = false;
	loop_animation = false;
	scaleX = 1;
	changeAnimation = false;
	changeSkin = false;
	resetSkeletonData = false;
	visible = true;
}

CharacterInfoComponent::CharacterInfoComponent() :notifyMana(false), isMainCharacter(false), MAX_BLOOD(-1), MAX_POWER(-1), blood(-1), power(-1), avatar("") {}
bool CharacterInfoComponent::hasManaForSkill(float skillMana){ return (power - skillMana) >= 0; }
void CharacterInfoComponent::notifyNotEnoughMana(){ notifyMana = true; }

DecisionComponent::DecisionComponent() : thinkingTime(0), DECISION_TIME(2), decisionBase(nullptr){}

StateComponent::StateComponent() : attack(R::Attack::NONE), defense(R::Defense::NONE), characterBase(nullptr), state(R::CharacterState::STAND), time_on_state(1), direction(R::Direction::AUTO){}

void StateComponent::setState(R::CharacterState newState){
	this->state = newState;
	time_on_state = 0;
}

AttackComponent::AttackComponent() :whoAttack(R::CharacterType::NONAME), expire(false), manaOfAttack(0), powerOfAttack(0), minX(0), maxX(0), minY(0){}

CharacterTypeComponent::CharacterTypeComponent() : type(R::CharacterType::NONAME){}

CharacterTypeComponent::CharacterTypeComponent(R::CharacterType name) : type(name){}

RemoveableComponent::RemoveableComponent() : haveToRemove(false){}

DelayComponent::DelayComponent() : timeAlive(0), timeDelay(10),callBack(nullptr){ }
void DelayComponent::setCallBack(const std::function<void()> &callBack){
	this->callBack = callBack;
}
