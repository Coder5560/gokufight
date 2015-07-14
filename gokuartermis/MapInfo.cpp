#include "MapInfo.h"


MapInfo::MapInfo()
{
	GROUND = 100;
	WALL_LEFT = 20;
	WALL_RIGHT = Director::getInstance()->getVisibleSize().width - 20;
	WALL_UP = Director::getInstance()->getVisibleSize().height - 20;
}


MapInfo::~MapInfo()
{
}
bool MapInfo::checkCollide(float x, float y){
	if (y <= GROUND || x <= WALL_LEFT || x >= WALL_RIGHT){
		return true;
	}
	return false;
}