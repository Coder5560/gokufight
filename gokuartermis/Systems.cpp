#include "Systems.h"
#include "gokuartermis/Components.h"
#include "RenderLayer.h"
#include "gokuartermis/CharacterSystem.h"
#include "RenderLayer.h"
Systems::Systems() {
}

Systems::~Systems() {
}

GravitySystem::GravitySystem() {
	GRAVITY_FACTOR = 50;
	addComponentType<PhysicComponent>();
	addComponentType<GravityComponent>();
}
void GravitySystem::initialize() {
	pm.init(*world);
	gm.init(*world);
}

void GravitySystem::begin() {

}

void GravitySystem::processEntity(artemis::Entity &e) {
	PhysicComponent* physic = (PhysicComponent*)(pm.get(e));
	GravityComponent* gravity = (GravityComponent*)(gm.get(e));
	if (gravity->enable) {
		physic->vx += gravity->gravityX * GRAVITY_FACTOR * world->getDelta();
		physic->vy += gravity->gravityY * GRAVITY_FACTOR * world->getDelta();
		//CCLOG("From Gravity System : %f - %f", physic->vy, gravity->gravityY*GRAVITY_FACTOR*world->getDelta());
	}
}

bool GravitySystem::checkProcessing() {
	return true;
}

void GravitySystem::end() {

}

PhysicSystem::PhysicSystem() {
	addComponentType<PhysicComponent>();
}
void PhysicSystem::initialize() {
	physicMapper.init(*world);
}

void PhysicSystem::processEntity(artemis::Entity &e) {
}
void PhysicSystem::push(artemis::Entity &entity, float rotation, float force) {
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(
		entity));
	if (physic) {
		float radianAngle = rotation * M_PI / 180;
		float plusVx = force * cos(radianAngle);
		float plusVy = force * sin(radianAngle);
		physic->vx += plusVx;
		physic->vy += plusVy;
		//	CCLOG("Debug from PhysicSystem : %f - %f", physic->vx, physic->vy);
	}

}

void PhysicSystem::stopPhysic(artemis::Entity& e) {
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic) {
		physic->vx = 0;
		physic->vy = 0;
	}
}

void PhysicSystem::clampVelocity(artemis::Entity& e, float minSpeed,
	float maxSpeed) {
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic) {
		Vec2 vTmp = Vec2(physic->vx, physic->vy);
		float length2 = vTmp.x * vTmp.x + vTmp.y * vTmp.y;
		float angle = vTmp.getAngle();
		float max2 = maxSpeed * maxSpeed;
		float min2 = minSpeed * minSpeed;

		if (length2 > max2) {
			physic->vx = maxSpeed * cos(angle);
			physic->vy = maxSpeed * sin(angle);
		}

		if (length2 < min2) {
			physic->vx = minSpeed * cos(angle);
			physic->vy = minSpeed * sin(angle);
		}
	}
}

WallSensorSystem::WallSensorSystem() :
mapInfo(nullptr) {

	addComponentType<PosComponent>();
	addComponentType<WallSensorComponent>();
	addComponentType<BoundComponent>();
	addComponentType<PhysicComponent>();

}
void WallSensorSystem::initialize() {
	mapInfo = new MapInfo();
	pom.init(*world);
	wm.init(*world);
	bm.init(*world);
	physicMapper.init(*world);

}
void WallSensorSystem::begin() {

}

void WallSensorSystem::processEntity(artemis::Entity &e) {
	PosComponent* position = (PosComponent*)(pom.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));
	BoundComponent* bound = (BoundComponent*)(bm.get(e));
	PhysicComponent* physic = (PhysicComponent*)(physicMapper.get(e));

	float px = position->x + world->getDelta() * physic->vx;
	float py = position->y + world->getDelta() * physic->vy;
	//float px = position->x;
	//float py = position->y;

	bool onFloor = mapInfo->checkCollide(px + bound->getCenterX(),
		py + bound->y1 - 1);
	bool onCelling = mapInfo->checkCollide(px + bound->getCenterX(),
		py + bound->y2 + 1);
	bool onWallLeft = mapInfo->checkCollide(px + bound->x1 - 1,
		py + bound->getCenterY());
	bool onWallRight = mapInfo->checkCollide(px + bound->x2 + 1,
		py + bound->getCenterY());

	wallSensor->onVerticalSurface = onWallLeft || onWallRight;
	wallSensor->onFloor = onFloor;
	wallSensor->onHorizontalSurface = onCelling || onFloor;
	wallSensor->wallAngle = onFloor ? 90 : onCelling ? -90 : onWallRight ? 0 :
		onWallLeft ? 180 : 90;


	if (physic->vr != 0) {
		//entityrotation = physic->vr*world->getDelta();
	}

	if (physic->friction != 0) {
		float adjustedFriction = physic->friction
			* (wallSensor->onFloor ? .5 : .2);
		if (abs(physic->vx) > 2) {
			if (physic->isMoving) {
			}
			else {
				physic->vx = physic->vx
					- (physic->vx * world->getDelta() * adjustedFriction);
			}
		}
		else {
			physic->vx = 0;
		}
		if (abs(physic->vy) > 2) {
			//physic->vy -= (physic->vy*world->getDelta()*adjustedFriction);
		}
		else {
			physic->vy = 0;
		}
		if (abs(physic->vr) > 2) {
			physic->vr = physic->vr
				- (physic->vr * world->getDelta() * adjustedFriction);
		}
		else {
			physic->vr = 0;
		}
	}
	if (wallSensor->onFloor) {
		//	CCLOG("On Floor");
		physic->vy = 0;
	}
	
}

bool WallSensorSystem::checkProcessing() {
	return true;
}

void WallSensorSystem::end() {

}

MotionSystem::MotionSystem() {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();


}
void MotionSystem::initialize() {
	psm.init(*world);
	pym.init(*world);
}
void MotionSystem::begin() {

}

void MotionSystem::processEntity(artemis::Entity &e) {
	PhysicComponent* physic = (PhysicComponent*)(pym.get(e));
	PosComponent* position = (PosComponent*)(psm.get(e));
	position->x += physic->vx * world->getDelta();
	position->y += physic->vy * world->getDelta();
	
}

bool MotionSystem::checkProcessing() {	return true;}

void MotionSystem::end() {}

GameStateSystem::GameStateSystem() {
	time_stay_on_state = 0;
	addComponentType<GameStateComponent>();
}
void GameStateSystem::initialize() {
	gameStateMapper.init(*world);

	artemis::Entity& entity = (world->getEntityManager()->create());
	entity.addComponent(new GameStateComponent());
	entity.setTag("gameStateEntity");
	entity.refresh();

}
void GameStateSystem::begin() {
}

void GameStateSystem::processEntity(artemis::Entity &e) {
}
bool GameStateSystem::checkProcessing() {
	return true;
}
void GameStateSystem::end() {
	time_stay_on_state += world->getDelta();
}
void GameStateSystem::switchToWin(){}
void GameStateSystem::switchToLose(){}
void GameStateSystem::switchToReady(){}
void GameStateSystem::switchToFighting(){}
void GameStateSystem::switchToPause(){}
void GameStateSystem::switchToResume(){}




MapCollisionSystem::MapCollisionSystem() {
	mapInfo = new MapInfo();
	addComponentType<PhysicComponent>();
	addComponentType<PosComponent>();
	addComponentType<BoundComponent>();
}
void MapCollisionSystem::initialize() {

	physicMapper.init(*world);
	posMapper.init(*world);
	boundMapper.init(*world);

}
void MapCollisionSystem::begin() {

}

void MapCollisionSystem::processEntity(artemis::Entity &e) {

	PhysicComponent* physic = (PhysicComponent*)(physicMapper.get(e));
	PosComponent* position = (PosComponent*)(posMapper.get(e));
	BoundComponent* bound = (BoundComponent*)(boundMapper.get(e));

	// no match require here, just check collision base on current infomation

	//	CCLOG("process mapcollision : %f - %f ", physic->vx, physic->vy);

	if (physic->vx != 0 || physic->vy != 0) {

		float px = position->x + physic->vx * world->getDelta();
		float py = position->y + physic->vy * world->getDelta();

		bool collideCenterRight = mapInfo->checkCollide(px + bound->x2,
			py + bound->getCenterY());
		bool collideCenterLeft = mapInfo->checkCollide(px + bound->x1,
			py + bound->getCenterY());
		bool collideCenterUp = mapInfo->checkCollide(px + bound->getCenterX(),
			py + bound->y2);
		bool collideCenterDown = mapInfo->checkCollide(px + bound->getCenterX(),
			py + bound->y1);

		if ((physic->vx > 0 && collideCenterRight)
			|| (physic->vx < 0 && collideCenterLeft)) {
			physic->vx =
				(physic->bounce > 0) ? -physic->vx * physic->bounce : 0;
		}

		if (physic->vy < 0 && collideCenterDown) {
			physic->vy =
				(physic->bounce > 0) ? -physic->vy * physic->bounce : 0;

		}
	}
}
bool MapCollisionSystem::checkProcessing() {
	return true;
}
void MapCollisionSystem::end() {

}

InputSystem::InputSystem() {

}
void InputSystem::initialize() {

}
void InputSystem::notifyInput(GameHud::EventType event,
	GameHud::TouchType touchType) {
	GameStateComponent* gameState = (GameStateComponent*)world->getTagManager()->getEntity("gameStateEntity").getComponent<GameStateComponent>();
	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	//	if (gameState->gameState == GameStateComponent::GameState::FIGHTING){

	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
	


	switch (event){
	case GameHud::EventType::BEGIN:
		if (touchType == GameHud::TouchType::TAP){
			CCLOG("Beat");
		//	gokuSystem->actionBeat1(R::Direction::AUTO);
			
			//stateComponent->customAnimation = true;
			//stateComponent->animations.clear();
			//stateComponent->animations.push_back("Beat1");
			stateComponent->setState(R::CharacterState::BEAT);
			stateComponent->characterBase->changeState(goku);

		}
		else if (touchType == GameHud::TouchType::LONG_PRESS){
			CCLOG("Punch");
//gokuSystem->actionPunch1(R::Direction::AUTO);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Punch1");
			stateComponent->setState(R::CharacterState::PUNCH);
		}
		else if (touchType == GameHud::TouchType::LEFT){
			CCLOG("LEFT");
		//	gokuSystem->actionMove(R::Direction::LEFT);

			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Move");
			stateComponent->setState(R::CharacterState::LEFT);
			stateComponent->direction = R::Direction::LEFT;



		}
		else if (touchType == GameHud::TouchType::RIGHT){
			CCLOG("RIGHT");
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Move");
			stateComponent->setState(R::CharacterState::RIGHT);
			stateComponent->direction = R::Direction::RIGHT;

//gokuSystem->actionMove(R::Direction::RIGHT);

		}
		else if (touchType == GameHud::TouchType::TOP){
			CCLOG("JUMP");
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Jump1");
			stateComponent->setState(R::CharacterState::JUMP);
		//	gokuSystem->actionJump1(R::Direction::AUTO);
		}
		else if (touchType == GameHud::TouchType::TOP_LEFT){
			CCLOG("JUMP_LEFT");
		//	gokuSystem->actionJump1(R::Direction::TOP_LEFT);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Jump1");
			stateComponent->direction = R::Direction::TOP_LEFT;
			stateComponent->setState(R::CharacterState::JUMP);
			

		}
		else if (touchType == GameHud::TouchType::TOP_RIGHT){
			CCLOG("JUMP_RIGHT");
		//	gokuSystem->actionJump1(R::Direction::TOP_RIGHT);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Jump1");
			stateComponent->direction = R::Direction::TOP_RIGHT;
			stateComponent->setState(R::CharacterState::JUMP);

		}
		else if (touchType == GameHud::TouchType::BOTTOM_LEFT){
		//	gokuSystem->actionKick1(R::Direction::LEFT);
			CCLOG("KICK_LEFT");
		}
		else if (touchType == GameHud::TouchType::BOTTOM_RIGHT){
		//	gokuSystem->actionKick1(R::Direction::RIGHT);
			CCLOG("KICK_RIGHT");
		}
		else if (touchType == GameHud::TouchType::BOTTOM){
		//	gokuSystem->actionKick1(R::Direction::AUTO);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Kick1");
			stateComponent->setState(R::CharacterState::KICK);
			stateComponent->direction = R::Direction::AUTO;
			CCLOG("KICK");
		}

		break;

	case GameHud::EventType::HOLD:

		if (touchType == GameHud::TouchType::LEFT){
		//	gokuSystem->actionMoveOn(R::Direction::LEFT);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Move");
			stateComponent->setState(R::CharacterState::WALK_LEFT);
			stateComponent->direction = R::Direction::LEFT;
			CCLOG("MOVE on LEFT");
		}
		else if (touchType == GameHud::TouchType::RIGHT){
			CCLOG("MOVE on RIGHT");
		//	gokuSystem->actionMoveOn(R::Direction::RIGHT);
			stateComponent->customAnimation = true;
			stateComponent->animations.clear();
			stateComponent->animations.push_back("Move");
			stateComponent->setState(R::CharacterState::WALK_RIGHT);
			stateComponent->direction = R::Direction::RIGHT;
		}
		break;

	case GameHud::EventType::END:

		if (characterInfo->state != R::CharacterState::JUMP){
			if (touchType == GameHud::TouchType::LEFT){
		//		gokuSystem->actionStand();
				stateComponent->customAnimation = true;
				stateComponent->animations.clear();
				stateComponent->animations.push_back("Stand");
				stateComponent->setState(R::CharacterState::STAND);
			}
			else if (touchType == GameHud::TouchType::RIGHT){
		//		gokuSystem->actionStand();
				stateComponent->customAnimation = true;
				stateComponent->animations.clear();
				stateComponent->animations.push_back("Stand");
				stateComponent->setState(R::CharacterState::STAND);
			}
		}
		break;
	default:
		break;
	}


	//}
}
void InputSystem::processEntity(artemis::Entity &e) {

}

SkeletonSystem::SkeletonSystem() {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
	addComponentType<BoundComponent>();
	addComponentType<SkeletonComponent>();
}
void SkeletonSystem::initialize() {
	physicMapper.init(*world);
	boundMapper.init(*world);
	skeletonMapper.init(*world);
	positionMapper.init(*world);
}

void SkeletonSystem::begin() {

}

void SkeletonSystem::processEntity(artemis::Entity &e) {
	SkeletonComponent* skeleton = (SkeletonComponent*)skeletonMapper.get(e);
	PosComponent* position = (PosComponent*)positionMapper.get(e);
	if (skeleton) {
		if (!skeleton->isCreated)
			return;
		skeleton->node->setPosition(Vec2(position->x, position->y));
	}
}

bool SkeletonSystem::checkProcessing() {
	return true;
}

void SkeletonSystem::end() {

}



UICharacterSystem::UICharacterSystem() {
	addComponentType<CharacterInfoComponent>();
}
void UICharacterSystem::initialize(){
	characterInfoMapper.init(*world);
}
void UICharacterSystem::createNodeForCharacter(CharacterInfoComponent* characterInfo){
	std::string str = "Character :" + characterInfo->tag;
	CCLOG(str.c_str());
	NodeInfo* node = new NodeInfo();
	node->createNode(characterInfo);
	renderObjects.insert(std::pair<std::string, NodeInfo*>(characterInfo->tag, node));
}
void UICharacterSystem::processNodeForCharacter(CharacterInfoComponent* characterInfo){
	if (renderObjects.count(characterInfo->tag) != 0){
		renderObjects[characterInfo->tag]->process(characterInfo);
	}


}

void UICharacterSystem::processEntity(artemis::Entity &e){
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)characterInfoMapper.get(e);
	if (renderObjects.count(characterInfo->tag) == 0){
		createNodeForCharacter(characterInfo);
	}
	else{
		processNodeForCharacter(characterInfo);
	}
}


DebugSystem::DebugSystem(){
	addComponentType<PosComponent>();
	addComponentType<BoundComponent>();

	
	
}
void DebugSystem::initialize(){
	posMapper.init(*world);
	boundMapper.init(*world);
	Node* node = RenderLayer::getInstance()->createHudNode();
	node->setTag(100);
}
void DebugSystem::begin(){
	Node* node = RenderLayer::getInstance()->getHudLayer()->getChildByTag(100);
	node->removeAllChildrenWithCleanup(true);
}
void DebugSystem::processEntity(artemis::Entity &e){
	PosComponent* position = posMapper.get(e);
	BoundComponent* bound = boundMapper.get(e);

	auto rectNode = DrawNode::create();

	Vec2 rectangle[4];
	rectangle[0] = Vec2(position->x + bound->x1, position->y + bound->y1);
	rectangle[1] = Vec2(position->x + bound->x2, position->y + bound->y1);
	rectangle[2] = Vec2(position->x + bound->x2, position->y + bound->y2);
	rectangle[3] = Vec2(position->x + bound->x1, position->y + bound->y2);

	Color4F white(1, 0, 0, 1);
	rectNode->drawLine(rectangle[0],rectangle[1],white);
	rectNode->drawLine(rectangle[1], rectangle[2], white);
	rectNode->drawLine(rectangle[2], rectangle[3], white);
	rectNode->drawLine(rectangle[3], rectangle[0], white);
	
	RenderLayer::getInstance()->getHudLayer()->getChildByTag(100)->addChild(rectNode);
}
