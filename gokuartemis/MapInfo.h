#pragma once
#include "cocos2d.h"
USING_NS_CC;
class MapInfo
{
public:
	MapInfo();
	~MapInfo();
	bool checkCollide(float x, float y);
	
	float GROUND;
	float WALL_LEFT;
	float WALL_RIGHT;
	float WALL_UP;


};

