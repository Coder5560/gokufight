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
	PhysicComponent* physic = pm.get(e);
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
	if (!wallSensor)return;
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
	if (physic && position){
		position->x += physic->vx * world->getDelta();
		position->y += physic->vy * world->getDelta();
	}
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
		if (gameState->gameState == R::GameState::ANIMATING_TO_FIGHT){ switchToReady(); }
		if (gameState->gameState == R::GameState::FIGHTING){
			switchToFighting();
		}
		if (gameState->gameState == R::GameState::WIN){ switchToWin(); }
		if (gameState->gameState == R::GameState::LOSE){ switchToLose(); }
	}
	else{
		if (gameState->gameState == R::GameState::ANIMATING_TO_FIGHT){
			if (gameState->time_on_state > 1){
				gameState->setGameState(R::GameState::FIGHTING);
			}
			gameState->time_on_state += world->getDelta();
			return;
		}

		if (gameState->gameState == R::GameState::PREPARE){
			artemis::Entity &goku = world->getTagManager()->getEntity("goku");
			WallSensorComponent* wallSensor = (WallSensorComponent*)goku.getComponent<WallSensorComponent>();
			StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
			if (wallSensor->onFloor && stateComponent->state == R::CharacterState::STAND && stateComponent->time_on_state > .1){
				gameState->setGameState(R::GameState::ANIMATING_TO_FIGHT);
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
	float worldWidth = R::Constants::MAX_SCREEN_WIDTH / RenderLayer::getInstance()->getGameLayer()->getScale();
	float worldHeight = R::Constants::HEIGHT_SCREEN / 2;
	float viewWidth = R::Constants::WIDTH_SCREEN;

	gokuPosition->x = worldWidth / 2 - 140;
	gokuPosition->y = 3 * worldHeight / 4;
	enemyPosition->x = worldWidth / 2 + 40;
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
	defaulcamera->setPositionX(worldWidth / 2 + viewWidth / 2);

}
void GameStateSystem::switchToReady(){
	Node* node = RenderLayer::getInstance()->getHudLayer()->getChildByTag(200);
	if (node){
		node->removeFromParentAndCleanup(true);
	}
	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	stateComponent->direction = R::Direction::RIGHT;
	stateComponent->setState(R::CharacterState::START);


	artemis::Entity &character = world->getTagManager()->getEntity("enemy");
	PosComponent* characterPosition = (PosComponent*)character.getComponent<PosComponent>();
	StateComponent* characterstateComponent = (StateComponent*)character.getComponent<StateComponent>();
	characterstateComponent->direction = R::Direction::LEFT;
	characterstateComponent->setState(R::CharacterState::START);
	CharacterTypeComponent* characterType = (CharacterTypeComponent*)character.getComponent<CharacterTypeComponent>();

	if (characterType->type == R::CharacterType::TEGIAC){
		artemis::Entity &meo = world->getTagManager()->getEntity("meo");
		PosComponent* meoPosition = (PosComponent*)meo.getComponent<PosComponent>();
		meoPosition->x = characterPosition->x + 500;
		meoPosition->y = characterPosition->y / 2 + 100;
		CatFollowComponent* catFollow = (CatFollowComponent*)meo.getComponent<CatFollowComponent>();
		catFollow->setState(R::CatFollowState::FOLLOW_ENEMY);
	}


}
void GameStateSystem::switchToFighting(){


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

		if ((physic->vx > 0 && collideCenterRight) || (physic->vx < 0 && collideCenterLeft)) {
			if (physic->dismissWhenCollideWithWall){ EntityUtils::getInstance()->removeEntity(e); return; }
			physic->vx = (physic->bounce > 0) ? -physic->vx * physic->bounce : 0;
		}

		if (physic->vy < 0 && collideCenterDown) {
			if (physic->dismissWhenCollideWithWall){ EntityUtils::getInstance()->removeEntity(e); return; }
			physic->vy = (physic->bounce > 0) ? -physic->vy * physic->bounce : 0;

		}
	}
}
bool MapCollisionSystem::checkProcessing() {
	return true;
}
void MapCollisionSystem::end() {

}

InputSystem::InputSystem() {
	_timeLastTouch = 0;
	_tapCount = 0;
}
void InputSystem::initialize() {

}
void InputSystem::notifyInput(Touch* touch, GameHud::EventType event,
	GameHud::TouchType touchType) {
		if (touch && touch->getLocation().y > 500){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			Node* node = RenderLayer::getInstance()->getBackgroundLayer();
			if (node->getNumberOfRunningActions() > 0) return;
			bgIndex++;
			bgIndex = bgIndex % 7;
			FadeIn* fadeIn = FadeIn::create(.5f);
			FadeOut* fadeOut = FadeOut::create(.5f);
			CallFunc* callFun = CallFunc::create([=](){
				node->removeAllChildren();
				std::string str = "backgrounds/bg1.png";
				if (bgIndex == 0) str = "backgrounds/bg1.png";
				if (bgIndex == 1) str = "backgrounds/bg2.png";
				if (bgIndex == 2) str = "backgrounds/bg3.png";
				if (bgIndex == 3) str = "backgrounds/bg4.png";
				if (bgIndex == 4) str = "backgrounds/bg5.png";
				if (bgIndex == 5) str = "backgrounds/bg6.png";
				if (bgIndex == 6) str = "backgrounds/bg7.png";

				ui::ImageView* image = ui::ImageView::create(str);
				image->setContentSize(Size(R::Constants::MAX_SCREEN_WIDTH, R::Constants::HEIGHT_SCREEN));
				image->setAnchorPoint(Vec2(0, 0));
				node->addChild(image);
			});


			node->runAction(Sequence::create(fadeIn, callFun, fadeOut, nullptr));

		}
		return;
	}

	GameStateComponent* gameState = (GameStateComponent*)world->getTagManager()->getEntity("gameState").getComponent<GameStateComponent>();
	if (gameState->gameState == R::GameState::NONE){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			gameState->setGameState(R::GameState::PREPARE);
		}
		return;
	}

	if (gameState->gameState == R::GameState::WIN){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			ECSWorld::getInstance()->nextMatch();
		}
		return;
	}

	if (gameState->gameState == R::GameState::LOSE){
		if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::TAP){
			ECSWorld::getInstance()->resetCurrentMatch();

		}
		return;
	}



	artemis::Entity &goku = world->getTagManager()->getEntity("goku");


	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	if (gameState->gameState == R::GameState::FIGHTING){
		artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");
		PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
		PosComponent* enemyPosition = (PosComponent*)enemy.getComponent<PosComponent>();

		switch (event){
		case GameHud::EventType::BEGIN:

			if (touchType == GameHud::TouchType::TAP){
				_tapCount++;
				


				//if ((stateComponent->state == R::CharacterState::STAND || stateComponent->state == R::CharacterState::DEFENSE) && stateComponent->time_on_state > .1f){
				//	stateComponent->setState(R::CharacterState::ATTACK);
				//	float distance = abs(gokuPosition->x - enemyPosition->x);
				//	srand(time(NULL));
				//	int random = rand() % 8 + 1;
				//	//if (distance > 30){
				//	//	// hoac la punch2 hoac la beat2
				//	//	(random < 4) ? (stateComponent->attack = R::Attack::GOKU_BEAT2) : (stateComponent->attack = R::Attack::GOKU_PUNCH2);
				//	//}
				//	//else{
				//	//kick3, beat3,punch2,kick2,beat2,kick1
				//	if (random == 1)stateComponent->attack = R::Attack::GOKU_KICK3;
				//	else if (random == 2)stateComponent->attack = R::Attack::GOKU_BEAT3;
				//	else if (random == 3)stateComponent->attack = R::Attack::GOKU_PUNCH2;
				//	else if (random == 4)stateComponent->attack = R::Attack::GOKU_KICK2;
				//	else if (random == 5)stateComponent->attack = R::Attack::GOKU_BEAT2;
				//	else if (random == 6)stateComponent->attack = R::Attack::GOKU_KICK1;
				//	else stateComponent->attack = R::Attack::GOKU_BEAT1;
				//}
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
void InputSystem::begin(){
	if (_tapCount != 0){
		_timeLastTouch += world->getDelta();
	}
	if (_timeLastTouch >= .2f && _tapCount!=0){
		processInputTap(_tapCount);
	}

}
void InputSystem::processInputTap(int tapCount){
	CCLOG("Tap Count %d", tapCount);
	_timeLastTouch = 0;
	_tapCount = 0;

	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)goku.getComponent<StateComponent>();
	if ((stateComponent->state == R::CharacterState::STAND || stateComponent->state == R::CharacterState::DEFENSE) && stateComponent->time_on_state > .1f){
		stateComponent->setState(R::CharacterState::ATTACK);
			if (tapCount == 1){
			srand(time(NULL));
			int random = rand() % 100;
			if (random % 3 == 0)stateComponent->attack = R::Attack::GOKU_KICK1;
			else if (random % 3 == 1)stateComponent->attack = R::Attack::GOKU_KICK2;
			else stateComponent->attack = R::Attack::GOKU_KICK3;
		}
		if (tapCount == 2){
			srand(time(NULL));
			int random = rand() % 4 + 1;
			if (random == 1)stateComponent->attack = R::Attack::GOKU_BEAT1;
			else if (random == 2)stateComponent->attack = R::Attack::GOKU_BEAT3;
			else if (random == 3)stateComponent->attack = R::Attack::GOKU_PUNCH2;
			else if (random == 4)stateComponent->attack = R::Attack::GOKU_BEAT2;
			else stateComponent->attack = R::Attack::GOKU_BEAT1;
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
	if (position){}
	else{ return; }
	auto rectNode = DrawNode::create();

	Vec2 rectangle[4];
	rectangle[0] = Vec2(position->x + bound->x1, position->y + bound->y1);
	rectangle[1] = Vec2(position->x + bound->x2, position->y + bound->y1);
	rectangle[2] = Vec2(position->x + bound->x2, position->y + bound->y2);
	rectangle[3] = Vec2(position->x + bound->x1, position->y + bound->y2);

	Color4F white(1, 0, 0, 1);
	rectNode->drawLine(rectangle[0] * 2, rectangle[1] * 2, white);
	rectNode->drawLine(rectangle[1] * 2, rectangle[2] * 2, white);
	rectNode->drawLine(rectangle[2] * 2, rectangle[3] * 2, white);
	rectNode->drawLine(rectangle[3] * 2, rectangle[0] * 2, white);

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
		e.remove();
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
		e.remove();
	}
}



SkeletonCollisonSystem::SkeletonCollisonSystem() :collisionPoint(Vec2::ZERO){}
void SkeletonCollisonSystem::initialize(){
}
void SkeletonCollisonSystem::processEntity(artemis::Entity &e){
	if (((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type == R::CharacterType::GOKU){

	}
	else{

	}
}
bool SkeletonCollisonSystem::checkCollision(artemis::Entity &attacker, artemis::Entity &defenser){
	bool isCollision = false;

	return isCollision;
}


CatFollowGokuSystem::CatFollowGokuSystem() : prepareAttack(false), readyToAttack(false){
	addComponentType<PosComponent>();
	addComponentType<SkeletonComponent>();
	addComponentType<CatFollowComponent>();
}
void CatFollowGokuSystem::initialize(){
	positionMapper.init(*world);
	skeletonMapper.init(*world);
	catFollowMapper.init(*world);
}
void CatFollowGokuSystem::processEntity(artemis::Entity &e){
	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");
	PosComponent* enemyPosition = (PosComponent*)enemy.getComponent<PosComponent>();

	PosComponent* meoPosition = positionMapper.get(e);
	SkeletonComponent* meoAnimation = skeletonMapper.get(e);
	CatFollowComponent* catFollow = catFollowMapper.get(e);
	catFollow->timeOnState += world->getDelta();

	GameStateComponent* gameState = (GameStateComponent*)(world->getTagManager()->getEntity("gameState").getComponent<GameStateComponent>());
	if (gameState->gameState != R::GameState::FIGHTING){
		return;
	}
	if (catFollow->state == R::CatFollowState::NONE){
		return;
	}
	else if (catFollow->state == R::CatFollowState::ATTACK_GOKU){

		if (!prepareAttack){
			// create attack
			meoAnimation->skeleton->setAnimation(0, "throw", false);
			meoAnimation->skeleton->setCompleteListener([=](int trackID, int loopcount){
				if (!readyToAttack){
					readyToAttack = true;
				}
			});
			prepareAttack = true;
		}


		if (readyToAttack){
			meoAnimation->skeleton->setAnimation(0, "stand", true);
			meoAnimation->skeleton->setCompleteListener(nullptr);
			createBomAtack(meoPosition, meoAnimation->node->getScaleX()<0);
			catFollow->setState(R::CatFollowState::FOLLOW_ENEMY);
			readyToAttack = false;
			prepareAttack = false;
		}

	}
	else if (catFollow->state == R::CatFollowState::FOLLOW_ENEMY){
		float ratio = .98f;
		if (abs(enemyPosition->x - meoPosition->x) > 10){
			meoPosition->x = ratio* meoPosition->x + (1 - ratio)* enemyPosition->x;
			meoPosition->y = 200;
			meoAnimation->node->setPosition(Vec2(meoPosition->x, meoPosition->y));
			if (gokuPosition->x > meoPosition->x){
				meoAnimation->node->setScaleX(1);
			}
			else{
				meoAnimation->node->setScaleX(-1);
			}
		}


		if (catFollow->timeOnState >= catFollow->nextTimeAttack){
			catFollow->setState(R::CatFollowState::FOLLOW_GOKU);
		}
	}
	else  if (catFollow->state == R::CatFollowState::FOLLOW_GOKU){
		if (abs(gokuPosition->x - meoPosition->x) > 10){
			float ratio = .98f;
			meoPosition->x = ratio* meoPosition->x + (1 - ratio)* gokuPosition->x;
			meoPosition->y = 200;
			meoAnimation->node->setPosition(Vec2(meoPosition->x, meoPosition->y));
			if (gokuPosition->x > meoPosition->x){
				meoAnimation->node->setScaleX(1);
			}
			else{
				meoAnimation->node->setScaleX(-1);
			}
		}
		else{
			if (catFollow->timeOnState >= catFollow->nextTimeAttack / 2){
				catFollow->setState(R::CatFollowState::ATTACK_GOKU);
			}
		}
	}
}
void CatFollowGokuSystem::createBomAtack(PosComponent* positionComponent, bool isLeftDirection){

	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/bomb.png";

	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/bomb.json",
		"spine/bomb.atlas");
	skeletonAnimation->setAnimation(0, "bomb", false);
	skeletonAnimation->setScale(.3);
	skeletonAnimation->setVisible(false);

	Sprite* ball = Sprite::create("textures/ball.png");
	ball->setScale(.5f);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(ball);
	node->setVisible(true);

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;


	Vec2 position = Vec2(positionComponent->x + (isLeftDirection ? -20 : 20), positionComponent->y);


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::BOMB));
	character.addComponent(new PosComponent(position.x, position.y));
	character.addComponent(characterSkeleton);
	character.addComponent(new BomComponent());
	character.setTag("bomb");

	character.refresh();
}


BombSystem::BombSystem(){
	addComponentType<PosComponent>();
	addComponentType<SkeletonComponent>();
	addComponentType<BomComponent>();
}
void BombSystem::initialize(){
	positionMapper.init(*world);
	skeletonMapper.init(*world);
	bombMapper.init(*world);
}
void BombSystem::processEntity(artemis::Entity &e){
	artemis::Entity &goku = world->getTagManager()->getEntity("goku");
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* bombPosition = positionMapper.get(e);
	BomComponent* bomComponent = bombMapper.get(e);
	MapInfo* map = new MapInfo();
	SkeletonComponent* skeletonComponent = skeletonMapper.get(e);
	if (!map->checkCollide(bombPosition->x, bombPosition->y - 20)){
		bombPosition->y -= 200 * world->getDelta();
		skeletonComponent->node->setPosition(Vec2(bombPosition->x, bombPosition->y));
	}
	else{
		if (!bomComponent->expire){
			bomComponent->expire = true;
			skeletonComponent->node->removeAllChildren();

			spine::SkeletonAnimation* animation =
				spine::SkeletonAnimation::createWithFile("spine/bomb.json",
				"spine/bomb.atlas", .3f);
			animation->setAnimation(0, "bomb", false);
			Node* node = RenderLayer::getInstance()->createGameNode();
			node->setPosition(bombPosition->x, bombPosition->y);
			node->setAnchorPoint(Vec2(.5, .5));
			node->ignoreAnchorPointForPosition(false);
			node->addChild(animation);

			DelayTime* delay = DelayTime::create(3);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			Sequence* sequence = Sequence::create(delay, removeSelf, nullptr);
			node->runAction(sequence);

			// check for attack goku
			Vec2 _pos1 = Vec2(gokuPosition->x, gokuPosition->y);
			if (node->getPosition().distanceSquared(_pos1) < 500){

				CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
				gokuInfo->blood -= bomComponent->powerOfAttack;
			}

			e.remove();
		}
	}
}




CameraFollowSystem::CameraFollowSystem(){
	addComponentType<CameraFollowComponent>();

}
void CameraFollowSystem::initialize(){
	cameraMapper.init(*world);
}
void CameraFollowSystem::processEntity(artemis::Entity &e){
	if (world->getTagManager()->isSubscribed("goku")){
		artemis::Entity &e = world->getTagManager()->getEntity("goku");
		PosComponent* pos = (PosComponent*)e.getComponent<PosComponent>();
		auto defaulcamera = Camera::getDefaultCamera();
		float cameraX = defaulcamera->getPositionX();


		float distance = 2 * pos->x - cameraX;
		float maxDistance = R::Constants::WIDTH_SCREEN / 4;

		if (distance > maxDistance){
			cameraX = 2 * pos->x - maxDistance;
		}

		if (distance < -maxDistance){
			cameraX = 2 * pos->x + maxDistance;
		}




		float minX = R::Constants::WIDTH_SCREEN / 2;
		float maxX = R::Constants::MAX_SCREEN_WIDTH - R::Constants::WIDTH_SCREEN / 2;
		cameraX = (cameraX < minX) ? minX : cameraX;
		cameraX = (cameraX >maxX) ? maxX : cameraX;
		defaulcamera->setPositionX(cameraX);
	}
}




SpecialSkillSystem::SpecialSkillSystem(){
	addComponentType<PosComponent>();
	addComponentType<SkeletonComponent>();
	addComponentType<CharacterTypeComponent>();
	addComponentType<StateComponent>();
	addComponentType<CharacterInfoComponent>();
}
void SpecialSkillSystem::initialize(){
	positionMapper.init(*world);
	skeletonMapper.init(*world);
	typeMapper.init(*world);
	stateMapper.init(*world);
	infoMapper.init(*world);
}
void SpecialSkillSystem::begin(){
	Layer* layer = RenderLayer::getInstance()->getGameLayer();
	if (!layer->getChildByName("drawnode")){
		Node* node = Node::create();
		node->setName("drawnode");
		node->setContentSize(layer->getContentSize());
		layer->addChild(node);
	}
	else{
		layer->getChildByName("drawnode")->removeAllChildren();
	}
}
void SpecialSkillSystem::processEntity(artemis::Entity &e){
	CharacterTypeComponent* type = typeMapper.get(e);

	//if (type->type == R::CharacterType::GOKU){
	//	processGoku(e);
	//}
	if (type->type == R::CharacterType::PICOLO){
		processPicolo(e);
	}

}
void SpecialSkillSystem::processPicolo(artemis::Entity &e){
	StateComponent* stateComponent = stateMapper.get(e);
	// trường hợp power1
	if (stateComponent->state == R::CharacterState::ATTACK && stateComponent->attack == R::Attack::PICOLO_POWER1){
		SkeletonComponent* skeleton = skeletonMapper.get(e);
		spine::SkeletonAnimation* animation = skeleton->skeleton;
		spBone* bone = animation->findBone("c-effect2");
		if (bone){
			float x = skeleton->node->getPositionX() + skeleton->node->getScaleX()* (bone->skeleton->x + bone->worldX);
			float y = skeleton->node->getPositionY() + skeleton->node->getScaleY()*(bone->skeleton->y + bone->worldY);
			CCDrawNode* drawnode = CCDrawNode::create();
			drawnode->drawDot(Vec2(x, y), 5, Color4F::BLUE);
			RenderLayer::getInstance()->getGameLayer()->getChildByName("drawnode")->addChild(drawnode);
			artemis::Entity& goku = world->getTagManager()->getEntity("goku");
			PosComponent* position = (PosComponent*)goku.getComponent<PosComponent>();
			BoundComponent* bound = (BoundComponent*)goku.getComponent<BoundComponent>();

			Rect rect1 = Rect(position->x + bound->x1, position->y + bound->y1, bound->getWidth(), bound->getHeight());
			Rect rect2 = Rect(x - 5, y - 5, 10, 10);
			if (rect1.intersectsCircle(Vec2(x, y), 10)){
				StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
				if (gokuState->state != R::CharacterState::DEFENSE){
					gokuState->setState(R::CharacterState::DEFENSE);
					gokuState->defense = R::Defense::TRUNG_DON_NGA;
					PosComponent* attackPosition = positionMapper.get(e);
					gokuState->direction = (attackPosition->x > position->x) ? R::Direction::LEFT : R::Direction::RIGHT;
				}
			}
		}
	}

	// trường hợp power2 

	if (stateComponent->state == R::CharacterState::ATTACK && stateComponent->attack == R::Attack::PICOLO_POWER2){
		SkeletonComponent* skeleton = skeletonMapper.get(e);
		spine::SkeletonAnimation* animation = skeleton->skeleton;
		spBone* bone = animation->findBone("circle-effect");
		if (bone){
			float x = skeleton->node->getPositionX() + skeleton->node->getScaleX()* (bone->skeleton->x + bone->worldX);
			float y = skeleton->node->getPositionY() + skeleton->node->getScaleY()*(bone->skeleton->y + bone->worldY);
			CCDrawNode* drawnode = CCDrawNode::create();
			drawnode->drawDot(Vec2(x, y), 5, Color4F::BLUE);
			RenderLayer::getInstance()->getGameLayer()->getChildByName("drawnode")->addChild(drawnode);
			artemis::Entity& goku = world->getTagManager()->getEntity("goku");
			PosComponent* position = (PosComponent*)goku.getComponent<PosComponent>();
			BoundComponent* bound = (BoundComponent*)goku.getComponent<BoundComponent>();

			Rect rect1 = Rect(position->x + bound->x1, position->y + bound->y1, bound->getWidth(), bound->getHeight());
			Rect rect2 = Rect(x - 5, y - 5, 10, 10);
			if (rect1.intersectsCircle(Vec2(x, y), 10)){
				StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
				if (gokuState->state != R::CharacterState::DEFENSE){
					gokuState->setState(R::CharacterState::DEFENSE);
					gokuState->defense = R::Defense::TRUNG_DON_NGA;
					PosComponent* attackPosition = positionMapper.get(e);
					gokuState->direction = (attackPosition->x > position->x) ? R::Direction::LEFT : R::Direction::RIGHT;
				}
			}
		}
	}

}

void SpecialSkillSystem::processGoku(artemis::Entity &e){
	StateComponent* stateComponent = stateMapper.get(e);
	// trường hợp power1
	if (stateComponent->state == R::CharacterState::ATTACK && (stateComponent->attack == R::Attack::GOKU_BEAT1 || stateComponent->attack == R::Attack::GOKU_BEAT2 || stateComponent->attack == R::Attack::GOKU_BEAT3 || stateComponent->attack == R::Attack::GOKU_PUNCH2)){
		SkeletonComponent* skeleton = skeletonMapper.get(e);
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;

		spBoundingBoxAttachment* attachment = (spBoundingBoxAttachment*)skeletonAnimation->getAttachment("strick", "stickbound");
		spBone* bone = skeletonAnimation->findBone("bone19");
		int i;
		float px, py;
		float* vertices = attachment->vertices;
		float x = skeleton->node->getPositionX() + bone->skeleton->x + bone->worldX, y = skeleton->node->getPositionY() + bone->skeleton->y + bone->worldY;
		float* worldVertices = new float[attachment->verticesCount];
		for (i = 0; i < attachment->verticesCount; i += 2) {
			px = vertices[i];
			py = vertices[i + 1];
			worldVertices[i] = px * bone->m00 + py * bone->m01 + x;
			worldVertices[i + 1] = px * bone->m10 + py * bone->m11 + y;
		}

		CCDrawNode* polygon = CCDrawNode::create();
		//creating red polygon with thin black border
		Vec2* vec = new Vec2[attachment->verticesCount / 2];


		for (int i = 0; i < attachment->verticesCount / 2; i++){
			vec[i] = Vec2(worldVertices[i * 2], worldVertices[i * 2 + 1]);
		}
		polygon->drawPolygon(vec, 4, ccc4f(1, 1, 0, 1), 1, ccc4f(1, 1, 0, 1));

	}


}




CharacterRenderSystem::CharacterRenderSystem(){
	addComponentType<CharacterUIComponent>();
	isCreated = false;
}

void CharacterRenderSystem::initialize(){
	characterUIMapper.init(*world);
}

void CharacterRenderSystem::begin(){
	if (!isCreated && world->getTagManager()->isSubscribed("characterrender")){
		artemis::Entity &goku = world->getTagManager()->getEntity("goku");
		artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");
		CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
		CharacterInfoComponent* enemyInfo = (CharacterInfoComponent*)enemy.getComponent<CharacterInfoComponent>();

		infoLeft = new PlayerInfoLeft(RenderLayer::getInstance()->createHudNode(), gokuInfo->name, gokuInfo->avatar);
		infoRight = new PlayerInfoRight(RenderLayer::getInstance()->createHudNode(), enemyInfo->name, enemyInfo->avatar);
		infoLeft->node->setPosition(Vec2(10,R::Constants::HEIGHT_SCREEN-120));
		infoRight->node->setPosition(Vec2(260, R::Constants::HEIGHT_SCREEN - 120));
		infoLeft->node->setCameraMask((unsigned short)CameraFlag::USER1);
		infoRight->node->setCameraMask((unsigned short)CameraFlag::USER1);
		isCreated = true;

		text = ui::Text::create("VS", "fonts/fonts/courbd.ttf",24);
		text->setPosition(Vec2(R::Constants::WIDTH_SCREEN/2,R::Constants::HEIGHT_SCREEN-110));
		text->setColor(Color3B::BLACK);
		text->enableOutline(Color4B::WHITE,2);
		RenderLayer::getInstance()->getHudLayer()->addChild(text);
		text->setCameraMask((unsigned short)CameraFlag::USER1);

	}
}

void CharacterRenderSystem::processEntity(artemis::Entity &e){
	if (isCreated){
		artemis::Entity &goku = world->getTagManager()->getEntity("goku");
		artemis::Entity &enemy = world->getTagManager()->getEntity("enemy");
		CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
		CharacterInfoComponent* enemyInfo = (CharacterInfoComponent*)enemy.getComponent<CharacterInfoComponent>();
		gokuInfo->power += .05;
		gokuInfo->power = (gokuInfo->power < gokuInfo->MAX_POWER) ? gokuInfo->power : gokuInfo->MAX_POWER;

		enemyInfo->power += .05;
		enemyInfo->power = (enemyInfo->power < enemyInfo->MAX_POWER) ? enemyInfo->power : enemyInfo->MAX_POWER;

		infoLeft->update(gokuInfo->blood/gokuInfo->MAX_BLOOD, gokuInfo->power/gokuInfo->MAX_POWER);
		infoRight->update(enemyInfo->blood / enemyInfo->MAX_BLOOD, enemyInfo->power / enemyInfo->MAX_POWER);
	}


}