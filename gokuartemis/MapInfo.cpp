#include "MapInfo.h"
#include "RenderLayer.h"
#include "R.h"

MapInfo::MapInfo()
{
	GROUND = 80;
	WALL_LEFT = 10;
	WALL_RIGHT = R::Constants::MAX_SCREEN_WIDTH/RenderLayer::getInstance()->getGameLayer()->getScale() -10;
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
