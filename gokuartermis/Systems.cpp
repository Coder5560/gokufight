#include "Systems.h"
#include "gokuartermis/Components.h"

Systems::Systems()
{
}


Systems::~Systems()
{
}

GravitySystem::GravitySystem(){
	addComponentType<PosComponent>();
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
	}

}

bool GravitySystem::checkProcessing(){
	return true;
}

void GravitySystem::end(){

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
			physic->vy = physic->vy - (physic->vy*world->getDelta()*adjustedFriction);
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
		physic->vy = 0;
	}


}

bool AfterPhysicSystem::checkProcessing(){
	return true;
}

void AfterPhysicSystem::end(){

}




WallSensorSystem::WallSensorSystem(){
	addComponentType<PhysicComponent>();
	addComponentType<PosComponent>();
	addComponentType<WallSensorComponent>();
	addComponentType<BoundComponent>();

	GROUND = 100;
	WALL_LEFT = 20;
	WALL_RIGHT = 780;
	WALL_UP = 580;



}
void WallSensorSystem::initialize(){
	pom.init(*world);
	pym.init(*world);
	wm.init(*world);
	bm.init(*world);

}
void WallSensorSystem::begin(){

}

void WallSensorSystem::processEntity(artemis::Entity &e){
	PosComponent* position = (PosComponent*)(pom.get(e));
	PhysicComponent* physic = (PhysicComponent*)(pym.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));
	BoundComponent* bound = (BoundComponent*)(bm.get(e));

	if ((physic->vx != 0 || physic->vy != 0)){
		
		float px = position->x + physic->vx*world->getDelta();
		float py = position->y + physic->vy*world->getDelta();

		bool collideLeft = checkCollide(px + bound->x1, py + bound->getCenterY());
		bool collideRight = checkCollide(px + bound->x2, py+ bound->getCenterY());
		bool collideUp = checkCollide(px + bound->getCenterX(), py + bound->y2);
		bool collideDown = checkCollide(px + bound->getCenterX(), py + bound->y1);

		if ((physic->vx > 0 && collideRight)|| (physic->vx<0 || collideLeft)){
			physic->vx = physic->bounce>0 ? -physic->vx*physic->bounce : 0;
		}

		if ((physic->vy > 0 && collideUp) || (physic->vy<0 || collideDown)){
			physic->vy = physic->bounce>0 ? -physic->vy*physic->bounce : 0;
			position->y = GROUND;
		}

	}

	if (physic->vy == 0 && checkCollide(position->x + bound->getCenterX(), position->y + bound->y1)){
		wallSensor->onFloor = true;
	}

}
bool WallSensorSystem::checkCollide(float x, float y){
	if (y < GROUND || y > WALL_UP || x < WALL_LEFT || x > WALL_RIGHT){
		return true;
	}
	
	return false;
}

bool WallSensorSystem::checkProcessing(){
	return true;
}

void WallSensorSystem::end(){

}