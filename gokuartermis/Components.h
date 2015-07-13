#pragma once

#include "artemis/Artemis.h"
class PosComponent;
class GravityComponent;
class PhysicComponent;
class BoundComponent;
class WallSensorComponent;



class Components
{
public:
	Components();
	~Components();
};


class PosComponent : public artemis::Component{
public:
	PosComponent(float posX, float posY);
	float x;
	float y;
};

class GravityComponent : public artemis::Component{

public:
	GravityComponent();
	GravityComponent(float gravityY);
	const float DEFAULT_GRAVITY_Y = -9.8f;
	float gravityX;
	float gravityY;
	bool enable;

};


class PhysicComponent : public artemis::Component{
public:
	PhysicComponent(float vX, float vY);

	float vx;
	float vy;
	float vr;
	float friction;
	float bounce;

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