#include "Components.h"
#include "cocos2d.h"

USING_NS_CC;

Components::Components(){}


Components::~Components(){}

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
PhysicComponent::PhysicComponent(){
	vx = 0;
	vy = 0;
	vr = 0;
	friction = 6;
	bounce = .5;
}
PhysicComponent::PhysicComponent(float vX, float vY){
	this->vx = vX;
	this->vy = vY;
	vr = 0;
	friction = 6;
	bounce = .5;
}
WallSensorComponent::WallSensorComponent(){
	onFloor = false;
}
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


GameStateComponent::GameStateComponent() : gameState(GameState::PREPARE) {}


