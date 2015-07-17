#include "Systems.h"
#include "gokuartermis/Components.h"
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

}

bool WallSensorSystem::checkProcessing() {
	return true;
}

void WallSensorSystem::end() {

}

AfterPhysicSystem::AfterPhysicSystem() {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
	addComponentType<WallSensorComponent>();

}
void AfterPhysicSystem::initialize() {
	psm.init(*world);
	pym.init(*world);
	wm.init(*world);
}
void AfterPhysicSystem::begin() {

}

void AfterPhysicSystem::processEntity(artemis::Entity &e) {
	PhysicComponent* physic = (PhysicComponent*)(pym.get(e));
	PosComponent* position = (PosComponent*)(psm.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));

	position->x += physic->vx * world->getDelta();
	position->y += physic->vy * world->getDelta();

	if (physic->vr != 0) {
		//entityrotation = physic->vr*world->getDelta();
	}

	if (physic->friction != 0) {
		float adjustedFriction = physic->friction
			* (wallSensor->onFloor ? .5 : 1);
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

bool AfterPhysicSystem::checkProcessing() {
	return true;
}

void AfterPhysicSystem::end() {

}

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
}
bool GameStateSystem::checkProcessing() {
	return true;
}
void GameStateSystem::end() {
	time_stay_on_state += world->getDelta();
}
void GameStateSystem::switchToState(GameStateComponent::GameState state,
	artemis::Entity* e) {
	time_stay_on_state = 0;
	GameStateComponent* gameState = (GameStateComponent*)(gameStateMapper.get(
		*e));
	gameState->gameState = state;
}

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
	std::string str =
		(event == GameHud::EventType::BEGIN) ?
		"begin" :
		(event == GameHud::EventType::HOLD ? "Hold" : "end");

	artemis::Entity& e = world->getTagManager()->getEntity("goku");
	GokuProcessingSystem* gokuSystem =
		(GokuProcessingSystem*)(world->getSystemManager()->getSystem<
		GokuProcessingSystem>());

	if (event == GameHud::EventType::END
		&& touchType == GameHud::TouchType::NONE) {
		gokuSystem->actionStand();
	}

	// sự kiện người dùng release
	if (event == GameHud::EventType::END
		&& touchType == GameHud::TouchType::LEFT) {
		gokuSystem->actionStand();

	}
	if (event == GameHud::EventType::END
		&& touchType == GameHud::TouchType::RIGHT) {
		gokuSystem->actionStand();

	}
	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::A) {
		gokuSystem->actionKick1(R::AUTO);
	}

	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::LEFT_A) {
		gokuSystem->actionKick1(R::LEFT);
	}

	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::B) {
		gokuSystem->actionPunch1(R::AUTO);
	}

	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::LEFT_B) {
		gokuSystem->actionPunch1(R::LEFT);
	}

	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::X) {
		gokuSystem->actionBeat1(R::AUTO);
	}
	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::LEFT_X) {
		gokuSystem->actionBeat1(R::LEFT);
	}

	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::UP) {
		gokuSystem->actionJump1(R::AUTO);

	}

	if (event == GameHud::EventType::HOLD
		&& touchType == GameHud::TouchType::LEFT) {
		gokuSystem->actionMoveOn(R::LEFT);

	}
	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::LEFT) {
		gokuSystem->actionMove(R::LEFT);

	}
	if (event == GameHud::EventType::HOLD
		&& touchType == GameHud::TouchType::RIGHT) {
		gokuSystem->actionMoveOn(R::RIGHT);

	}
	if (event == GameHud::EventType::BEGIN
		&& touchType == GameHud::TouchType::RIGHT) {
		gokuSystem->actionMove(R::RIGHT);
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

GokuProcessingSystem::GokuProcessingSystem() :
goku(nullptr), physicSystem(nullptr) {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();

}
void GokuProcessingSystem::initialize() {
	posMapper.init(*world);
	physicMapper.init(*world);
	physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<
		PhysicSystem>());

	// create CharacterInfoComponent
	CharacterInfoComponent* gokuInfo = new CharacterInfoComponent();
	gokuInfo->MAX_BLOOD = 100;
	gokuInfo->MAX_POWER = 60;
	gokuInfo->blood = 100;
	gokuInfo->power = 100;
	gokuInfo->skill_a_power = 20;
	gokuInfo->skill_b_power = 20;
	gokuInfo->skill_x_power = 20;




	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/skeleton.json",
		"spine/skeleton.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	skeletonAnimation->setScale(.4);
	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);

	SkeletonComponent* gokuSkeleton = new SkeletonComponent();
	gokuSkeleton->skeleton = skeletonAnimation;
	gokuSkeleton->node = node;
	gokuSkeleton->isCreated = true;

	auto slot = skeletonAnimation->findSlot("strick");


	spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
	if (attachment){
		CCLOG("%f - %f - %f - %f ", attachment->x, attachment->y, attachment->width, attachment->rotation);

	}










	// tạo những component dùng chung cho nhiều entity
	GameStateComponent* gameStateComponent = new GameStateComponent();

	// Thêm một cái entity cho nó xôm
	goku = &(world->getEntityManager()->create());

	goku->addComponent(new PosComponent(120, 400));
	goku->addComponent(new BoundComponent(0, 0, 60, 60));
	goku->addComponent(new WallSensorComponent());
	goku->addComponent(new GravityComponent());
	goku->addComponent(new PhysicComponent());
	goku->addComponent(gokuSkeleton);
	goku->addComponent(gameStateComponent);
	goku->addComponent(gokuInfo);
	goku->setTag("goku");
	goku->refresh();

	((PhysicComponent*)(goku->getComponent<PhysicComponent>()))->bounce = 0;

}

void GokuProcessingSystem::processEntity(artemis::Entity &e) {

}
void GokuProcessingSystem::end() {

	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	WallSensorComponent* wallSensor = (WallSensorComponent*)goku->getComponent<WallSensorComponent>();
	PhysicComponent* physic = (PhysicComponent*)goku->getComponent<PhysicComponent>();
	if (wallSensor->onFloor && gokuInfo->state == R::CharacterState::JUMP1 && gokuInfo->timeOnState != 0){
		actionStand();
		gokuInfo->state = R::CharacterState::STAND;
	}
	gokuInfo->timeOnState += world->getDelta();
}
void GokuProcessingSystem::actionStand() {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		//	CCLOG("Stand...........");
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<
			CharacterInfoComponent>();

		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Stand", true);
		skeletonAnimation->setToSetupPose();
		skeletonAnimation->setCompleteListener(nullptr);
		skeletonAnimation->setTimeScale(1);
		physicSystem->stopPhysic(*goku);
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving =
			false;
		gokuInfo->timeOnState = 0;
		gokuInfo->state = R::CharacterState::STAND;
	}

}
void GokuProcessingSystem::actionStandUp() {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Stand up", true);
		physicSystem->stopPhysic(*goku);
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving =
			true;
	}
}
void GokuProcessingSystem::actionDie(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionMove(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		if (direction == R::Direction::RIGHT) {
			physicSystem->push(*goku, 0, 160);
			physicSystem->clampVelocity(*goku, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
		}
		else if (direction == R::Direction::LEFT) {
			physicSystem->push(*goku, 180, 160);
			physicSystem->clampVelocity(*goku, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
	}
}

void GokuProcessingSystem::actionMoveOn(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving =
			true;
		// xử lý action
		if (direction == R::Direction::RIGHT) {
			physicSystem->push(*goku, 0, 160);
			physicSystem->clampVelocity(*goku, 0, 160);
			gokuInfo->state = R::CharacterState::WALK_RIGHT;
		}
		else if (direction == R::Direction::LEFT) {
			physicSystem->push(*goku, 180, 160);
			physicSystem->clampVelocity(*goku, 180, 160);
			gokuInfo->state = R::CharacterState::WALK_LEFT;
		}
	}
}

void GokuProcessingSystem::actionRun(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionStart(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionTrungDon(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;

		if (direction == R::Direction::LEFT) {
			physicSystem->push(*goku, 180, 160);
			physicSystem->clampVelocity(*goku, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x -= 20;
				actionStand();

			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			physicSystem->push(*goku, 0, 160);
			physicSystem->clampVelocity(*goku, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				actionStand();
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x += 20;
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {

			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x -= node->getScaleX() == 1 ? 20 : -20;
				actionStand();

			});
		}

	}
}
void GokuProcessingSystem::actionVictory() {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionBeat1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && (gokuInfo->timeOnState > .2f || gokuInfo->state == R::STAND);
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Beat1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener(
			[=](int trackIndex, int loopCount) {
			skeletonAnimation->setTimeScale(1);
			actionStand();

		});
		gokuInfo->timeRadon = .3f;
		gokuInfo->timeOnState = 0;
		gokuInfo->state = R::CharacterState::BEAT1;
	}
}
void GokuProcessingSystem::actionBeat2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionBeat3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionJump1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		physicSystem->push(*goku, 90, 300);
		physicSystem->clampVelocity(*goku, 0, 300);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump1", false);
		skeletonAnimation->setCompleteListener(nullptr);
		gokuInfo->state = R::CharacterState::JUMP1;
		gokuInfo->timeOnState = 0;

	}
}
void GokuProcessingSystem::actionJump2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionJump3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionKick1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		physicSystem->stopPhysic(*goku);
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Kick1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			actionStand();
		});
		gokuInfo->timeOnState = 0;
		gokuInfo->timeRadon = .3;
		gokuInfo->state = R::CharacterState::KICK1;
	}
}
void GokuProcessingSystem::actionKick2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionKick3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionPunch1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && (gokuInfo->timeOnState > .2f || gokuInfo->state == R::STAND);
	if (!dudieukien) {
		return;
	}
	else {
		physicSystem->stopPhysic(*goku);
		SkeletonComponent* skeleton = (SkeletonComponent*)goku->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		//skeletonAnimation->addAnimation(1, "Stand", true,.1f);
		skeletonAnimation->setCompleteListener(
			[=](int trackIndex, int loopCount) {
			actionStand();
		});
		gokuInfo->timeOnState = 0;
		gokuInfo->timeRadon = 1;
		gokuInfo->state = R::CharacterState::PUNCH1;
	}
}
void GokuProcessingSystem::actionPunch2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionPunch3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}

CharacterProcessingSystem::CharacterProcessingSystem() :
node(nullptr), skeletonAnimation(nullptr), character(nullptr), initSystem(
false), physicSystem(nullptr) {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
}

CharacterProcessingSystem::~CharacterProcessingSystem() {
}
void CharacterProcessingSystem::initialize() {
	posMapper.init(*world);
	physicMapper.init(*world);
	initSystem = false;
	character = nullptr;
	skeletonAnimation = nullptr;

}

void CharacterProcessingSystem::initSystemInformation() {
	if (initSystem)
		return;
}

void CharacterProcessingSystem::begin() {
	if (!initSystem) {
		CCLOG("Init System");
		initSystemInformation();
		initSystem = true;
	}
}

void CharacterProcessingSystem::processEntity(artemis::Entity &e) {
}

void CharacterProcessingSystem::actionStand() {
}
void CharacterProcessingSystem::actionStandUp() {
}
void CharacterProcessingSystem::actionDie(R::Direction direction) {
}
void CharacterProcessingSystem::actionMove(R::Direction direction) {
}
void CharacterProcessingSystem::actionMoveOn(R::Direction direction) {
}
void CharacterProcessingSystem::actionRun(R::Direction direction) {
}
void CharacterProcessingSystem::actionStart(R::Direction direction) {
}
void CharacterProcessingSystem::actionTrungDon(R::Direction direction) {
}
void CharacterProcessingSystem::actionVictory() {
}
void CharacterProcessingSystem::actionBeat(R::Direction direction) {
}
void CharacterProcessingSystem::actionJump(R::Direction direction) {
}
void CharacterProcessingSystem::actionKick(R::Direction direction) {
}
void CharacterProcessingSystem::actionPunch(R::Direction direction) {
}
void CharacterProcessingSystem::actionThink() {
}
void CharacterProcessingSystem::actionSpecialSkill(R::Direction direction) {
}

YamchaCharacter::YamchaCharacter(std::string tag, std::string skeletonDataFile,
	std::string atlasFile) {
	CharacterProcessingSystem();
	this->tag = tag;
	this->skeletonDatafile = skeletonDataFile;
	this->skeletonAtlas = atlasFile;
}

void YamchaCharacter::initialize() {
	posMapper.init(*world);
	physicMapper.init(*world);
	initSystem = false;
	character = nullptr;
	skeletonAnimation = nullptr;
}

void YamchaCharacter::initSystemInformation() {
	//skeletonAnimation->setAnimation(0, "Stand", true);
	//skeletonAnimation->setSkin("goku");

	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/skeleton.json",
		"spine/skeleton.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("yamcha");
	skeletonAnimation->setScale(.4);
	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);
	node->setScaleX(-1);

	SkeletonComponent* yamchaSkeleton = new SkeletonComponent();
	yamchaSkeleton->skeleton = skeletonAnimation;
	yamchaSkeleton->node = node;
	yamchaSkeleton->isCreated = true;


	physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<
		PhysicSystem>());
	character = &(world->getEntityManager()->create());
	character->setTag(tag);
	character->addComponent(new PosComponent(240, 420));
	character->addComponent(new PhysicComponent());
	character->addComponent(new GravityComponent());
	character->addComponent(new BoundComponent(0, 0, 60, 60));
	character->addComponent(new WallSensorComponent());
	character->addComponent(yamchaSkeleton);

	character->refresh();


}

void YamchaCharacter::begin() {
	if (!initSystem) {
		CCLOG("Init Yamcha");
		initSystemInformation();
		initSystem = true;
	}
}

void YamchaCharacter::processEntity(artemis::Entity &e) {

}

void YamchaCharacter::actionTrungDon(R::Direction direction){

	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		if (direction == R::Direction::LEFT) {
			physicSystem->push(*character, 180, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				PosComponent* pos = (PosComponent*)(character->getComponent<PosComponent>());
				pos->x -= 20;
				actionStand();

			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			physicSystem->push(*character, 0, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				actionStand();
				PosComponent* pos = (PosComponent*)(character->getComponent<PosComponent>());
				pos->x += 20;
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {

			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				PosComponent* pos = (PosComponent*)(character->getComponent<PosComponent>());
				pos->x -= node->getScaleX() == 1 ? 20 : -20;
				actionStand();

			});
		}

	}

}






CharacterCollisionSystem::CharacterCollisionSystem(){

}

void CharacterCollisionSystem::initialize(){
}

void CharacterCollisionSystem::processEntity(artemis::Entity &e){

}
void CharacterCollisionSystem::end(){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	artemis::Entity& yamcha = world->getTagManager()->getEntity("yamcha");
	
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)(goku.getComponent<CharacterInfoComponent>());
	CharacterInfoComponent* yamchaInfo = (CharacterInfoComponent*)(yamcha.getComponent<CharacterInfoComponent>());


	if (gokuInfo->state == R::CharacterState::PUNCH1 || gokuInfo->state == R::CharacterState::KICK1 || gokuInfo->state == R::CharacterState::BEAT1){
		CCLOG("CheckDon %f %f", gokuInfo->timeOnState , gokuInfo->timeRadon);
		if (gokuInfo->timeOnState > gokuInfo->timeRadon){
			if (isHit(&goku, &yamcha)){
				CCLOG("Trung Don ");
				PosComponent* position1 = (PosComponent*)goku.getComponent<PosComponent>();
				PosComponent* position2 = (PosComponent*)yamcha.getComponent<PosComponent>();
				
				YamchaCharacter* yamchaCharacter = (YamchaCharacter*)world->getSystemManager()->getSystem<YamchaCharacter>();
				yamchaCharacter->actionTrungDon(position1->x<position2->x ? R::Direction::RIGHT : R::Direction::LEFT);
				gokuInfo->state = R::CharacterState::STAND;
			}
		}
	}



}

bool CharacterCollisionSystem::isHit(artemis::Entity* e1, artemis::Entity* e2){
	BoundComponent* bound1 = (BoundComponent*)e1->getComponent<BoundComponent>();
	BoundComponent* bound2 = (BoundComponent*)e2->getComponent<BoundComponent>();
	PosComponent* position1 = (PosComponent*)e1->getComponent<PosComponent>();
	PosComponent* position2 = (PosComponent*)e2->getComponent<PosComponent>();
	SkeletonComponent* skeleton1 = (SkeletonComponent*)e1->getComponent<SkeletonComponent>();
	SkeletonComponent* skeleton2 = (SkeletonComponent*)e2->getComponent<SkeletonComponent>();

	bool isLeft1 = skeleton1->node->getScaleX() > 0;
	bool isLeft2 = skeleton1->node->getScaleX() > 0;
	bool is1Left2 = position1->x < position2->x;
	bool canHit = (is1Left2 && isLeft1) || (!is1Left2 && !isLeft1);


	Vec2 centerE1 = Vec2(position1->x + bound1->getCenterX(), position1->y + bound1->getCenterY());
	Vec2 centerE2 = Vec2(position2->x + bound1->getCenterX(), position2->y + bound2->getCenterY());

	float distance = centerE1.distance(centerE2);





	if (canHit&& distance < bound1->getWidth() / 2 + bound2->getWidth() / 2){
		return true;
	}
	else{
		return false;
	}
	
	return false;

}