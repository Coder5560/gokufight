#include "Systems.h"
#include "gokuartemis/Components.h"
#include "RenderLayer.h"
#include "RenderLayer.h"
#include "ui/UIText.h"
#include "ECSWorld.h"
Systems::Systems() {}

Systems::~Systems() {}

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
	PhysicComponent* physic =pm.get(e);
	GravityComponent* gravity = gm.get(e);
	if (gravity->enable) {
		physic->vx += gravity->gravityX * GRAVITY_FACTOR * world->getDelta();
		physic->vy += gravity->gravityY * GRAVITY_FACTOR * world->getDelta();
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
		if (abs(physic->vx) > 1) {
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
		if (abs(physic->vy) > .5) {}
		else { physic->vy = 0; }

		if (abs(physic->vr) > 1) {
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

bool MotionSystem::checkProcessing() { return true; }

void MotionSystem::end() {}

GameStateSystem::GameStateSystem() {
	time_stay_on_state = 0;
	addComponentType<GameStateComponent>();
}
void GameStateSystem::initialize() {
	gameStateMapper.init(*world);
}
void GameStateSystem::begin() {
}

void GameStateSystem::processEntity(artemis::Entity &e) {
	GameStateComponent* gameState = gameStateMapper.get(e);
	if (gameState->time_on_state == 0){
		if (gameState->gameState == R::GameState::PREPARE){ switchToAppear(); }
		if (gameState->gameState == R::GameState::FIGHTING){ switchToFighting(); }
		if (gameState->gameState == R::GameState::WIN){ switchToWin(); }
		if (gameState->gameState == R::GameState::LOSE){ switchToLose(); }
	}
	else{
		if (gameState->gameState == R::GameState::PREPARE){
			artemis::Entity &goku = world->getTagManager()->getEntity("goku");
			WallSensorComponent* wallSensor = (WallSensorComponent*)goku.getComponent<WallSensorComponent>();
			StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
			if (wallSensor->onFloor && stateComponent->state == R::CharacterState::STAND && stateComponent->time_on_state > .1){
				gameState->setGameState(R::GameState::FIGHTING);
				return;
			}
			else if (wallSensor->onFloor&& stateComponent->state != R::CharacterState::STAND){
				stateComponent->direction = R::Direction::RIGHT;
				stateComponent->setState(R::CharacterState::STAND);
			}
		}

		if (gameState->gameState == R::GameState::FIGHTING){
			artemis::Entity &goku = world->getTagManager()->getEntity("goku");
			artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");

			CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
			CharacterInfoComponent* enemyInfo = (CharacterInfoComponent*)enemy.getComponent<CharacterInfoComponent>();

			if (gokuInfo->blood <= 0){
				gameState->setGameState(R::GameState::LOSE);
				return;
			}

			if (enemyInfo->blood <= 0){
				gameState->setGameState(R::GameState::WIN);
				return;
			}
		}
	}


	gameState->time_on_state += world->getDelta();
}
bool GameStateSystem::checkProcessing() {
	return true;
}
void GameStateSystem::end() {

}
void GameStateSystem::switchToWin(){

	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
	gokuState->setState(R::CharacterState::WIN);

	ui::Text* text = ui::Text::create("YOU WIN", "fonts/Marker Felt.ttf", 40);
	text->setColor(Color3B::RED);
	Node* node = RenderLayer::getInstance()->createHudNode();
	node->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	text->setAnchorPoint(Vec2(.5, .5));
	text->setPosition(node->getContentSize() / 2);
	auto fadeIn = FadeIn::create(1);

	auto fadeOut = FadeOut::create(1);
	text->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, nullptr)));

	node->addChild(text);
	node->setTag(200);
	node->setCameraMask((unsigned short)CameraFlag::USER1);



}
void GameStateSystem::switchToLose(){
	ui::Text* text = ui::Text::create("YOU LOSE", "fonts/Marker Felt.ttf", 40);
	text->setColor(Color3B::RED);
	Node* node = RenderLayer::getInstance()->createHudNode();
	node->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	text->setAnchorPoint(Vec2(.5, .5));
	text->setPosition(node->getContentSize() / 2);
	auto fadeIn = FadeIn::create(1);

	auto fadeOut = FadeOut::create(1);
	text->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, nullptr)));

	node->addChild(text);
	node->setTag(200);
	node->setCameraMask((unsigned short)CameraFlag::USER1);
}
void GameStateSystem::switchToAppear(){

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	artemis::Entity& enemy = world->getTagManager()->getEntity("enemy");

	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* enemyPosition = (PosComponent*)enemy.getComponent<PosComponent>();
	float worldWidth = R::Constants::MAX_SCREEN_WIDTH;
	float worldHeight = R::Constants::HEIGHT_SCREEN;

	gokuPosition->x = R::Constants::MAX_SCREEN_WIDTH / 2 - worldWidth / 8-40;
	gokuPosition->y = 3 * worldHeight / 4;
	enemyPosition->x = R::Constants::MAX_SCREEN_WIDTH / 2 + worldWidth / 8-40;
	enemyPosition->y = 3 * worldHeight / 4;


	SkeletonComponent* gokuSkeleton = (SkeletonComponent*)goku.getComponent<SkeletonComponent>();
	SkeletonComponent* enemySkeleton = (SkeletonComponent*)enemy.getComponent<SkeletonComponent>();

	gokuSkeleton->node->setVisible(true);
	enemySkeleton->node->setVisible(true);
	enemySkeleton->node->setScaleX(-1);


	PhysicComponent* gokuPhysic = (PhysicComponent*)goku.getComponent<PhysicComponent>();
	PhysicComponent* enemyPhysic = (PhysicComponent*)enemy.getComponent<PhysicComponent>();

	gokuPhysic->vy = -200;
	enemyPhysic->vy = -300;

	auto defaulcamera = Camera::getDefaultCamera();
	defaulcamera->setPositionX(R::Constants::MAX_SCREEN_WIDTH / 2);

}
void GameStateSystem::switchToReady(){}
void GameStateSystem::switchToFighting(){
	Node* node = RenderLayer::getInstance()->getHudLayer()->getChildByTag(200);
	if (node){
		node->removeFromParentAndCleanup(true);
	}
	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	stateComponent->direction = R::Direction::RIGHT;
	stateComponent->setState(R::CharacterState::START);

}
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

		if ((physic->vx > 0 && collideCenterRight)|| (physic->vx < 0 && collideCenterLeft)) {
			if (physic->dismissWhenCollideWithWall){ EntityUtils::getInstance()->removeEntity(e); return; }
			physic->vx =(physic->bounce > 0) ? -physic->vx * physic->bounce : 0;
		}

		if (physic->vy < 0 && collideCenterDown) {
			if (physic->dismissWhenCollideWithWall){ EntityUtils::getInstance()->removeEntity(e); return; }
			physic->vy =(physic->bounce > 0) ? -physic->vy * physic->bounce : 0;

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
	GameStateComponent* gameState = (GameStateComponent*)world->getTagManager()->getEntity("gameState").getComponent<GameStateComponent>();
	if (gameState->gameState == R::GameState::NONE){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			gameState->setGameState(R::GameState::PREPARE);
		}
		return;
	}

	if (gameState->gameState == R::GameState::WIN || gameState->gameState == R::GameState::LOSE){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			RenderLayer::getInstance()->getHudLayer()->removeAllChildren();
			RenderLayer::getInstance()->getGameLayer()->removeAllChildren();
			ECSWorld::getInstance()->createWorld(R::Match_Type::GOKU_GIRAN);
		}
		return;
	}


	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	if (gameState->gameState == R::GameState::FIGHTING){

		CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
		switch (event){
		case GameHud::EventType::BEGIN:

			if (touchType == GameHud::TouchType::TAP){
				if (stateComponent->state != R::CharacterState::ATTACK){
					stateComponent->setState(R::CharacterState::ATTACK);
					stateComponent->attack = R::Attack::GOKU_BEAT1;
				}
			}
			else if (touchType == GameHud::TouchType::LONG_PRESS){
				if (stateComponent->state != R::CharacterState::ATTACK){
					stateComponent->setState(R::CharacterState::ATTACK);
					stateComponent->attack = R::Attack::GOKU_PUNCH1;
				}

			}
			else if (touchType == GameHud::TouchType::LEFT){

				stateComponent->setState(R::CharacterState::LEFT);
				stateComponent->direction = R::Direction::LEFT;



			}
			else if (touchType == GameHud::TouchType::RIGHT){

				stateComponent->setState(R::CharacterState::RIGHT);
				stateComponent->direction = R::Direction::RIGHT;
			}
			else if (touchType == GameHud::TouchType::TOP){
				stateComponent->setState(R::CharacterState::JUMP);
			}
			else if (touchType == GameHud::TouchType::TOP_LEFT){
				stateComponent->direction = R::Direction::TOP_LEFT;
				stateComponent->setState(R::CharacterState::JUMP);
			}
			else if (touchType == GameHud::TouchType::TOP_RIGHT){
				stateComponent->direction = R::Direction::TOP_RIGHT;
				stateComponent->setState(R::CharacterState::JUMP);
			}
			else if (touchType == GameHud::TouchType::BOTTOM_LEFT){
				if (stateComponent->state != R::CharacterState::ATTACK){
					stateComponent->attack = R::Attack::GOKU_KICK1;
					stateComponent->setState(R::CharacterState::ATTACK);
					stateComponent->direction = R::Direction::LEFT;
				}
			}
			else if (touchType == GameHud::TouchType::BOTTOM_RIGHT){
				if (stateComponent->state != R::CharacterState::ATTACK){
					stateComponent->attack = R::Attack::GOKU_KICK1;
					stateComponent->setState(R::CharacterState::ATTACK);
					stateComponent->direction = R::Direction::RIGHT;
				}

			}
			else if (touchType == GameHud::TouchType::BOTTOM){
				if (stateComponent->state != R::CharacterState::ATTACK){
					stateComponent->attack = R::Attack::GOKU_KICK1;
					stateComponent->setState(R::CharacterState::ATTACK);
					stateComponent->direction = R::Direction::AUTO;
				}
			}

			break;

		case GameHud::EventType::HOLD:
			if (touchType == GameHud::TouchType::LEFT){
				stateComponent->setState(R::CharacterState::WALK_LEFT);
				stateComponent->direction = R::Direction::LEFT;
			}
			else if (touchType == GameHud::TouchType::RIGHT){
				stateComponent->setState(R::CharacterState::WALK_RIGHT);
				stateComponent->direction = R::Direction::RIGHT;
			}
			break;

		case GameHud::EventType::END:

			if (stateComponent->state != R::CharacterState::JUMP){
				if (touchType == GameHud::TouchType::LEFT){
					stateComponent->setState(R::CharacterState::STAND);
				}
				else if (touchType == GameHud::TouchType::RIGHT){
					stateComponent->setState(R::CharacterState::STAND);
				}
			}
			break;
		default:
			break;
		}


	}
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

	NodeInfo* node = new NodeInfo();
	node->createNode(characterInfo);
	renderObjects.insert(std::pair<std::string, NodeInfo*>(characterInfo->avatar, node));
}
void UICharacterSystem::processNodeForCharacter(CharacterInfoComponent* characterInfo){
	if (renderObjects.count(characterInfo->avatar) != 0){
		renderObjects[characterInfo->avatar]->process(characterInfo);
	}


}

void UICharacterSystem::processEntity(artemis::Entity &e){
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)characterInfoMapper.get(e);
	if (renderObjects.count(characterInfo->avatar) == 0){
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
	rectNode->drawLine(rectangle[0], rectangle[1], white);
	rectNode->drawLine(rectangle[1], rectangle[2], white);
	rectNode->drawLine(rectangle[2], rectangle[3], white);
	rectNode->drawLine(rectangle[3], rectangle[0], white);

	RenderLayer::getInstance()->getHudLayer()->getChildByTag(100)->addChild(rectNode);
}


RemoveEntitySystem::RemoveEntitySystem(){
	addComponentType<RemoveableComponent>();
}
void RemoveEntitySystem::initialize(){
	removeEntityMapper.init(*world);
	
}

void RemoveEntitySystem::processEntity(artemis::Entity &e){
	RemoveableComponent* removeableComponent = removeEntityMapper.get(e);
	if (removeableComponent->haveToRemove){
		world->getEntityManager()->remove(e);
	}
}


DelaySystem::DelaySystem(){
	addComponentType<DelayComponent>();
}
void DelaySystem::initialize(){
	delayMapper.init(*world);

}

void DelaySystem::processEntity(artemis::Entity &e){
	DelayComponent* delayComponent = delayMapper.get(e);
	if (delayComponent->timeAlive > delayComponent->timeDelay){
		// remove and call the callBack
		delayComponent->callBack();
		world->getEntityManager()->remove(e);
	}
}