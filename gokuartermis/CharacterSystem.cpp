#include "CharacterSystem.h"

CharacterSystem::CharacterSystem() : characterType(R::CharacterType::NONAME), character(nullptr), initSystem(false), physicSystem(nullptr), animation(""), custom(false), animation1(""), animation2("") {
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
}

CharacterSystem::~CharacterSystem() {}
void CharacterSystem::initialize() {
	posMapper.init(*world);
	physicMapper.init(*world);
	initSystem = false;
	character = nullptr;
}

void CharacterSystem::initSystemInformation() {
	if (initSystem)
		return;
}

void CharacterSystem::begin() {
	if (!initSystem) {
		initSystemInformation();
		initSystem = true;
	}
}

void CharacterSystem::processEntity(artemis::Entity &e) {
}

void CharacterSystem::actionStand() {

	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		std::string strAnimation = custom ? animation : "Stand";
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<SkeletonComponent>();
		CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();

		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, strAnimation, true);
		skeletonAnimation->setToSetupPose();
		skeletonAnimation->setCompleteListener(nullptr);
		skeletonAnimation->setTimeScale(1);
		physicSystem->stopPhysic(*character);
		((PhysicComponent*)character->getComponent<PhysicComponent>())->isMoving = false;
		characterInfo->timeOnState = 0;
		characterInfo->state = R::CharacterState::STAND;
		// gán lại biến custom
		custom = false;
	}


}
void CharacterSystem::actionStandUp() {
}
void CharacterSystem::actionDie(R::Direction direction) {
}
void CharacterSystem::actionMove(R::Direction direction) {
	WallSensorComponent* wallSensor = (WallSensorComponent*)character->getComponent<WallSensorComponent>();

	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();

	//	bool dudieukien = wallSensor->onFloor && (characterInfo->state = R::CharacterState::STAND);
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		if (direction == R::Direction::RIGHT) {
			physicSystem->push(*character, 0, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
			characterInfo->state = R::CharacterState::WALK_RIGHT;
		}
		else if (direction == R::Direction::LEFT) {
			physicSystem->push(*character, 180, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
			characterInfo->state = R::CharacterState::WALK_LEFT;
		}
	}

}
void CharacterSystem::actionMoveOn(R::Direction direction) {
}
void CharacterSystem::actionRun(R::Direction direction) {
}
void CharacterSystem::actionStart(R::Direction direction) {
}
void CharacterSystem::actionTrungDon(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		std::string strAnimation = custom ? animation : "Trungdon";
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;

		if (direction == R::Direction::LEFT) {
			physicSystem->push(*character, 180, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, strAnimation, false);
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
			skeletonAnimation->setAnimation(0, strAnimation, false);
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
			skeletonAnimation->setAnimation(0, strAnimation, false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				PosComponent* pos = (PosComponent*)(character->getComponent<PosComponent>());
				pos->x -= node->getScaleX() == 1 ? 20 : -20;
				actionStand();
			});
		}
		// gán lại biến custom
		custom = false;
	}
}
void CharacterSystem::actionVictory() {
}
void CharacterSystem::actionBeat(R::Direction direction) {
}
void CharacterSystem::actionJump(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(character->getComponent<WallSensorComponent>());
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && (characterInfo->state == R::CharacterState::STAND || characterInfo->state == R::CharacterState::WALK_LEFT || characterInfo->state == R::CharacterState::WALK_RIGHT);
	if (!dudieukien) {
		return;
	}
	else {
		CCLOG("Jump Pass");
		std::string strAnimation = custom ? animation : "Jump";
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		float angle = direction == R::Direction::TOP ? 90 : (direction == R::Direction::TOP_LEFT ? 120 : (direction == R::Direction::TOP_RIGHT ? 60 : 90));
		float force = direction == R::Direction::TOP || direction == R::Direction::AUTO ? 300 : 400;
		// xử lý action
		physicSystem->push(*character, angle, force);
		physicSystem->clampVelocity(*character, 0, force);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, strAnimation, false);
		skeletonAnimation->setCompleteListener(nullptr);

		characterInfo->state = R::CharacterState::JUMP;
		characterInfo->timeOnState = 0;


		if (angle == 45 && node->getScaleX() != 1){
			node->setScaleX(1);
		}

		if (angle == 135 && node->getScaleX() != -1){
			node->setScaleX(-1);
		}
	}

}
void CharacterSystem::actionKick(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(character->getComponent<WallSensorComponent>());
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && characterInfo->state == R::CharacterState::STAND;
	if (!dudieukien) {
		return;
	}
	else {
		CCLOG("KICK Pass");
		std::string strAnimation = custom ? animation : "Kick";
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		physicSystem->stopPhysic(*character);
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, strAnimation, false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			actionStand();
		});
		characterInfo->timeOnState = 0;
		characterInfo->timeRadon = .3;
		characterInfo->state = R::CharacterState::KICK;

	}
}
void CharacterSystem::actionPunch(R::Direction direction) {
}
void CharacterSystem::actionThink() {
}
void CharacterSystem::actionSpecialSkill(R::Direction direction) {
}


GokuCharacter::GokuCharacter(){
	super();
	characterType = R::CharacterType::GOKU;
}
void GokuCharacter::initialize() {
	posMapper.init(*world);
	physicMapper.init(*world);
	physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<
		PhysicSystem>());

	// create CharacterInfoComponent
	CharacterInfoComponent* gokuInfo = new CharacterInfoComponent();
	gokuInfo->tag = "goku";
	gokuInfo->isMainCharacter = true;
	gokuInfo->MAX_BLOOD = 100;
	gokuInfo->MAX_POWER = 60;
	gokuInfo->blood = 100;
	gokuInfo->power = 100;
	gokuInfo->skill_a_power = 5;
	gokuInfo->skill_b_power = 5;
	gokuInfo->skill_x_power = 5;

	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Goku.json",
		"spine/Goku.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	skeletonAnimation->setScale(.8);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);

	SkeletonComponent* gokuSkeleton = new SkeletonComponent();
	gokuSkeleton->skeleton = skeletonAnimation;
	gokuSkeleton->node = node;
	gokuSkeleton->isCreated = true;

	character = &(world->getEntityManager()->create());

	character->addComponent(new PosComponent(120, 400));
	character->addComponent(new BoundComponent(0, 0, 60, 60));
	character->addComponent(new WallSensorComponent());
	character->addComponent(new GravityComponent());
	character->addComponent(new PhysicComponent());
	character->addComponent(gokuSkeleton);
	character->addComponent(gokuInfo);
	character->setTag(gokuInfo->tag);
	character->refresh();

	((PhysicComponent*)(character->getComponent<PhysicComponent>()))->bounce = 0;

}

void GokuCharacter::processEntity(artemis::Entity &e) {

}
void GokuCharacter::end() {
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();
	PosComponent* pos = (PosComponent*)character->getComponent<PosComponent>();
	WallSensorComponent* wallSensor = (WallSensorComponent*)character->getComponent<WallSensorComponent>();
	PhysicComponent* physic = (PhysicComponent*)character->getComponent<PhysicComponent>();
	if (wallSensor->onFloor && gokuInfo->state == R::CharacterState::JUMP && gokuInfo->timeOnState != 0){
		actionStand();
		gokuInfo->state = R::CharacterState::STAND;
	}
	gokuInfo->timeOnState += world->getDelta();
	auto camera = Camera::getDefaultCamera();
	auto winSize = Director::getInstance()->getWinSize();
	if (pos->x >= winSize.width / 2 && pos->x <= 800 - winSize.width / 2) {
		camera->setPositionX(pos->x);
	}
}

void GokuCharacter::actionStandUp() {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Stand up", true);
		physicSystem->stopPhysic(*character);
		((PhysicComponent*)character->getComponent<PhysicComponent>())->isMoving =
			true;
	}
	custom = false;
}
void GokuCharacter::actionDie(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
	custom = false;
}
void GokuCharacter::actionMove(R::Direction direction) {
	super::actionMove(direction);
	custom = false;
}

void GokuCharacter::actionMoveOn(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(character->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();

	bool dudieukien = wallSensor->onFloor && (gokuInfo->state == R::CharacterState::WALK_LEFT || gokuInfo->state == R::CharacterState::WALK_RIGHT);
	if (!dudieukien) {
		return;
	}
	else {
		((PhysicComponent*)character->getComponent<PhysicComponent>())->isMoving = true;

		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		if (direction == R::Direction::RIGHT) {
			physicSystem->push(*character, 0, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			gokuInfo->state = R::CharacterState::WALK_RIGHT;
			node->setScaleX(1);
		}
		else if (direction == R::Direction::LEFT) {
			physicSystem->push(*character, 180, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			gokuInfo->state = R::CharacterState::WALK_LEFT;
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			physicSystem->push(*character, node->getScaleX() > 0 ? 0 : 180, 160);
			physicSystem->clampVelocity(*character, 0, 160);
			gokuInfo->state = (node->getScaleX() > 0) ? R::CharacterState::WALK_RIGHT : R::CharacterState::WALK_LEFT;

		}
	}
	custom = false;
}

void GokuCharacter::actionRun(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionStart(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionTrungDon(R::Direction direction) {
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
	custom = false;
}
void GokuCharacter::actionVictory() {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionBeat1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(character->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && (gokuInfo->state == R::CharacterState::STAND || (gokuInfo->state == R::CharacterState::BEAT && gokuInfo->timeOnState > .2
		));
	if (!dudieukien) {
		return;
	}
	else {
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
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
		gokuInfo->state = R::CharacterState::BEAT;
	}
}
void GokuCharacter::actionBeat2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionBeat3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionJump1(R::Direction direction) {
	custom = true;
	animation = "Jump1";
	super::actionJump(direction);
	custom = false;
}
void GokuCharacter::actionJump2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionJump3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionKick1(R::Direction direction) {
	custom = true;
	animation = "Kick1";
	super::actionKick(direction);
	custom = false;
}
void GokuCharacter::actionKick2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionKick3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionPunch1(R::Direction direction) {
	WallSensorComponent* wallSensor =
		(WallSensorComponent*)(character->getComponent<WallSensorComponent>());
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)character->getComponent<CharacterInfoComponent>();
	bool dudieukien = wallSensor->onFloor && (gokuInfo->timeOnState > .2f || gokuInfo->state == R::CharacterState::STAND);
	if (!dudieukien) {
		return;
	}
	else {
		physicSystem->stopPhysic(*character);
		SkeletonComponent* skeleton = (SkeletonComponent*)character->getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		skeletonAnimation->setCompleteListener(
			[=](int trackIndex, int loopCount) {
			actionStand();
		});
		gokuInfo->timeOnState = 0;
		gokuInfo->timeRadon = 1;
		gokuInfo->state = R::CharacterState::PUNCH;
	}
}
void GokuCharacter::actionPunch2(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuCharacter::actionPunch3(R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}



YamchaCharacter::YamchaCharacter(std::string tag, std::string skeletonDataFile,
	std::string atlasFile) {
	super();
	this->characterType = R::CharacterType::YAMCHA;
	this->tag = tag;
	this->skeletonDatafile = skeletonDataFile;
	this->skeletonAtlas = atlasFile;
}

void YamchaCharacter::initialize() {
	super::initialize();
}

void YamchaCharacter::initSystemInformation() {

	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/skeleton.json",
		"spine/skeleton.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	skeletonAnimation->setScale(.8);
	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);
	node->setScaleX(-1);

	SkeletonComponent* yamchaSkeleton = new SkeletonComponent();
	yamchaSkeleton->skeleton = skeletonAnimation;
	yamchaSkeleton->node = node;
	yamchaSkeleton->isCreated = true;


	// create CharacterInfoComponent
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->tag = tag;
	characterInfo->isMainCharacter = false;
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 60;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	characterInfo->skill_a_power = 5;
	characterInfo->skill_b_power = 5;
	characterInfo->skill_x_power = 5;

	DecisionComponent* decisionComponent = new DecisionComponent();

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
	character->addComponent(characterInfo);
	character->addComponent(decisionComponent);
	character->refresh();

}

void YamchaCharacter::processEntity(artemis::Entity &e) {

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


	if (gokuInfo->state == R::CharacterState::PUNCH || gokuInfo->state == R::CharacterState::KICK || gokuInfo->state == R::CharacterState::BEAT){

		if (gokuInfo->timeOnState > gokuInfo->timeRadon){
			if (isHit(&goku, &yamcha)){

				PosComponent* position1 = (PosComponent*)goku.getComponent<PosComponent>();
				PosComponent* position2 = (PosComponent*)yamcha.getComponent<PosComponent>();

				YamchaCharacter* yamchaCharacter = (YamchaCharacter*)world->getSystemManager()->getSystem<YamchaCharacter>();
				yamchaCharacter->actionTrungDon(position1->x < position2->x ? R::Direction::RIGHT : R::Direction::LEFT);
				gokuInfo->state = R::CharacterState::STAND;
				gokuInfo->timeOnState = 0;
				yamchaInfo->blood -= gokuInfo->skill_a_power;
				CCLOG("Trung Don %f", yamchaInfo->blood);
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


DecisionSystem::DecisionSystem(){
	addComponentType<DecisionComponent>();
}

void DecisionSystem::initialize(){
	decisionMapper.init(*world);
}
void DecisionSystem::processEntity(artemis::Entity& e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	DecisionComponent* decision = (DecisionComponent*)e.getComponent<DecisionComponent>();
	PosComponent* gokuPosition =(PosComponent*) goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();

	decision->thinkingTime += world->getDelta();
	bool isLeftToGoku = characterPosition->x < gokuPosition->x;
	float distance = abs(gokuPosition->x - characterPosition->x);
	float rangeAttact = 100;
	if (distance > rangeAttact){
		


		// tung chuong tam xa.
		// tien lai gan.
		if (decision->thinkingTime > decision->DECISION_TIME){
			if (isLeftToGoku){
			//CharacterSystem* characterSystem = 	(CharacterSystem*) world->getSystemManager()->getSystem<CharacterSystem>();
			
			}
			decision->thinkingTime = 0;
		}

		// khieu khich
		
	}










}
