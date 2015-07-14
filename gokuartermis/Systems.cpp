#include "Systems.h"
#include "gokuartermis/Components.h"

Systems::Systems()
{
}


Systems::~Systems()
{
}

GravitySystem::GravitySystem(){
	GRAVITY_FACTOR = 50;
	addComponentType<PhysicComponent>();
	addComponentType<GravityComponent>();
}
void GravitySystem::initialize(){
	pm.init(*world);
	gm.init(*world);
}

void GravitySystem::begin(){

}

void GravitySystem::processEntity(artemis::Entity &e){
	PhysicComponent* physic = (PhysicComponent*)(pm.get(e));
	GravityComponent* gravity = (GravityComponent*)(gm.get(e));
	if (gravity->enable){
		physic->vx += gravity->gravityX*GRAVITY_FACTOR*world->getDelta();
		physic->vy += gravity->gravityY*GRAVITY_FACTOR*world->getDelta();
		//CCLOG("From Gravity System : %f - %f", physic->vy, gravity->gravityY*GRAVITY_FACTOR*world->getDelta());
	}
}

bool GravitySystem::checkProcessing(){
	return true;
}

void GravitySystem::end(){

}


PhysicSystem::PhysicSystem(){
	addComponentType<PhysicComponent>();
}
void PhysicSystem::initialize(){
	physicMapper.init(*world);
}

void PhysicSystem::processEntity(artemis::Entity &e){
}
void PhysicSystem::push(artemis::Entity &entity, float rotation, float force){
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(entity));
	if (physic){
		float radianAngle = rotation*M_PI/180;
		float plusVx = force*cos(radianAngle);
		float plusVy = force*sin(radianAngle);
		physic->vx += plusVx;
		physic->vy += plusVy;
	}
}

void PhysicSystem::stopPhysic(artemis::Entity& e){
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic){
		physic->vx = 0;
		physic->vy = 0;
	}
}

void PhysicSystem::clampVelocity(artemis::Entity& e,float minSpeed, float maxSpeed){
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic){
		Vec2 vTmp = Vec2(physic->vx, physic->vy);
		float length2= vTmp.x*vTmp.x + vTmp.y*vTmp.y;
		float angle = vTmp.getAngle();
		float max2 = maxSpeed*maxSpeed;
		float min2 = minSpeed*minSpeed;

		if (length2 > max2){
			physic->vx = maxSpeed*cos(angle);
			physic->vy = maxSpeed*sin(angle);
		}

		if (length2 < min2){
			physic->vx = minSpeed*cos(angle);
			physic->vy = minSpeed*sin(angle);
		}	
	}
}






WallSensorSystem::WallSensorSystem(){

	addComponentType<PosComponent>();
	addComponentType<WallSensorComponent>();
	addComponentType<BoundComponent>();



}
void WallSensorSystem::initialize(){
	mapInfo = new MapInfo();
	pom.init(*world);
	wm.init(*world);
	bm.init(*world);

}
void WallSensorSystem::begin(){

}

void WallSensorSystem::processEntity(artemis::Entity &e){
	PosComponent* position = (PosComponent*)(pom.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));
	BoundComponent* bound = (BoundComponent*)(bm.get(e));

	bool onFloor = mapInfo->checkCollide(position->x + bound->getCenterX(), position->y + bound->y1 - 1);
	bool onCelling = mapInfo->checkCollide(position->x + bound->getCenterX(), position->y + bound->y2 + 1);
	bool onWallLeft = mapInfo->checkCollide(position->x + bound->x1 - 1, position->y + bound->getCenterY());
	bool onWallRight = mapInfo->checkCollide(position->x + bound->x2 + 1, position->y + bound->getCenterY());

	wallSensor->onVerticalSurface = onWallLeft || onWallRight;
	wallSensor->onFloor = onFloor;
	wallSensor->onHorizontalSurface = onCelling || onFloor;
	wallSensor->wallAngle = onFloor ? 90 : onCelling ? -90 : onWallRight ? 0 : onWallLeft ? 180 : 90;


}


bool WallSensorSystem::checkProcessing(){
	return true;
}

void WallSensorSystem::end(){

}






AfterPhysicSystem::AfterPhysicSystem(){
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
	addComponentType<WallSensorComponent>();

}
void AfterPhysicSystem::initialize(){
	psm.init(*world);
	pym.init(*world);
	wm.init(*world);
}
void AfterPhysicSystem::begin(){

}

void AfterPhysicSystem::processEntity(artemis::Entity &e){
	PhysicComponent* physic = (PhysicComponent*)(pym.get(e));
	PosComponent* position = (PosComponent*)(psm.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));
	
	position->x += physic->vx*world->getDelta();
	position->y += physic->vy*world->getDelta();

	if (physic->vr != 0){
		//entityrotation = physic->vr*world->getDelta();
	}


	if (physic->friction != 0){
		float adjustedFriction = physic->friction * (wallSensor->onFloor ? 0.25 : 1);
		if (abs(physic->vx) > 0.005){
			physic->vx = physic->vx - (physic->vx*world->getDelta()*adjustedFriction);
		}
		else{
			physic->vx = 0;
		}
		if (abs(physic->vy) > 0.005){
			//physic->vy -= (physic->vy*world->getDelta()*adjustedFriction);
		}
		else{
			physic->vy = 0;
		}
		if (abs(physic->vr) > 0.005){
			physic->vr = physic->vr - (physic->vr*world->getDelta()*adjustedFriction);
		}
		else{
			physic->vr = 0;
		}
	}
	if (wallSensor->onFloor){
		//CCLOG("On Floor");
		physic->vy = 0;
	}
}

bool AfterPhysicSystem::checkProcessing(){
	return true;
}

void AfterPhysicSystem::end(){

}



GameStateSystem::GameStateSystem(){
	time_stay_on_state = 0;
	addComponentType<GameStateComponent>();
}
void GameStateSystem::initialize(){
	gameStateMapper.init(*world);
}
void GameStateSystem::begin(){
}

void GameStateSystem::processEntity(artemis::Entity &e){
}
bool GameStateSystem::checkProcessing(){
	return true;
}
void GameStateSystem::end(){
	time_stay_on_state += world->getDelta();
}
void GameStateSystem::switchToState(GameStateComponent::GameState state, artemis::Entity* e){
	time_stay_on_state = 0;
	GameStateComponent* gameState = (GameStateComponent*)(gameStateMapper.get(*e));
	gameState->gameState = state;
}




MapCollisionSystem::MapCollisionSystem(){
	mapInfo = new MapInfo();
	addComponentType<PhysicComponent>();
	addComponentType<PosComponent>();
	addComponentType<BoundComponent>();
}
void MapCollisionSystem::initialize(){

	physicMapper.init(*world);
	posMapper.init(*world);
	boundMapper.init(*world);

}
void MapCollisionSystem::begin(){

}

void MapCollisionSystem::processEntity(artemis::Entity &e){
	
	PhysicComponent* physic = (PhysicComponent*)(physicMapper.get(e));
	PosComponent* position = (PosComponent*)(posMapper.get(e));
	BoundComponent* bound = (BoundComponent*)(boundMapper.get(e));

	// no match require here, just check collision base on current infomation

//	CCLOG("process mapcollision : %f - %f ", physic->vx, physic->vy);

	if (physic->vx != 0 || physic->vy != 0)
	{
	
		float px = position->x + physic->vx*world->getDelta();
		float py = position->y + physic->vy*world->getDelta();

		bool collideCenterRight = mapInfo->checkCollide(px + bound->x2, py + bound->getCenterY());
		bool collideCenterLeft = mapInfo->checkCollide(px + bound->x1, py + bound->getCenterY());
		bool collideCenterUp = mapInfo->checkCollide(px + bound->getCenterX(), py + bound->y2);
		bool collideCenterDown = mapInfo->checkCollide(px + bound->getCenterX(), py + bound->y1);

		if ((physic->vx > 0 && collideCenterRight) || (physic->vx < 0 && collideCenterLeft))
		{
			physic->vx = (physic->bounce >0) ? -physic->vx * physic->bounce : 0;
		}

		if (physic->vy < 0 && collideCenterDown)
		{
			physic->vy = (physic->bounce >0) ? -physic->vy * physic->bounce : 0;
		//	CCLOG("bounce %f - %f", physic->vy,py + bound->y1);
		}
	}
}
bool MapCollisionSystem::checkProcessing(){
	return true;
}
void MapCollisionSystem::end(){

}

InputSystem::InputSystem(){

}
void InputSystem::initialize(){

}
void InputSystem::notifyInput(GameHud::EventType event, GameHud::TouchType touchType){
	artemis::Entity& e = world->getTagManager()->getEntity("goku");
	PhysicSystem* physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<PhysicSystem>());
	
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::UP){

		physicSystem->push(e,90, 400);
	}

}
void InputSystem::processEntity(artemis::Entity &e){


}