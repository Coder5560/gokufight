#include "Systems.h"
#include "gokuartermis/Components.h"
#include "RenderLayer.h"
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
		float radianAngle = rotation*M_PI / 180;
		float plusVx = force*cos(radianAngle);
		float plusVy = force*sin(radianAngle);
		physic->vx += plusVx;
		physic->vy += plusVy;
		//	CCLOG("Debug from PhysicSystem : %f - %f", physic->vx, physic->vy);
	}

}

void PhysicSystem::stopPhysic(artemis::Entity& e){
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic){
		physic->vx = 0;
		physic->vy = 0;
	}
}

void PhysicSystem::clampVelocity(artemis::Entity& e, float minSpeed, float maxSpeed){
	PhysicComponent* physic = static_cast<PhysicComponent*>(physicMapper.get(e));
	if (physic){
		Vec2 vTmp = Vec2(physic->vx, physic->vy);
		float length2 = vTmp.x*vTmp.x + vTmp.y*vTmp.y;
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
	addComponentType<PhysicComponent>();




}
void WallSensorSystem::initialize(){
	mapInfo = new MapInfo();
	pom.init(*world);
	wm.init(*world);
	bm.init(*world);
	physicMapper.init(*world);

}
void WallSensorSystem::begin(){

}

void WallSensorSystem::processEntity(artemis::Entity &e){
	PosComponent* position = (PosComponent*)(pom.get(e));
	WallSensorComponent* wallSensor = (WallSensorComponent*)(wm.get(e));
	BoundComponent* bound = (BoundComponent*)(bm.get(e));
	PhysicComponent* physic = (PhysicComponent*)(physicMapper.get(e));


	float px = position->x + world->getDelta()*physic->vx;
	float py = position->y + world->getDelta()*physic->vy;

	bool onFloor = mapInfo->checkCollide(px + bound->getCenterX(), py + bound->y1 - 1);
	bool onCelling = mapInfo->checkCollide(px + bound->getCenterX(), py + bound->y2 + 1);
	bool onWallLeft = mapInfo->checkCollide(px + bound->x1 - 1, py + bound->getCenterY());
	bool onWallRight = mapInfo->checkCollide(px + bound->x2 + 1, py + bound->getCenterY());

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
		float adjustedFriction = physic->friction * (wallSensor->onFloor ? .5 : 1);
		if (abs(physic->vx) > 2){
			if (physic->isMoving){}
			else{ physic->vx = physic->vx - (physic->vx*world->getDelta()*adjustedFriction); }
		}
		else{
			physic->vx = 0;
		}
		if (abs(physic->vy) > 2){
			//physic->vy -= (physic->vy*world->getDelta()*adjustedFriction);
		}
		else{
			physic->vy = 0;
		}
		if (abs(physic->vr) > 2){
			physic->vr = physic->vr - (physic->vr*world->getDelta()*adjustedFriction);
		}
		else{
			physic->vr = 0;
		}
	}
	if (wallSensor->onFloor){
		//CCLOG("On Floor");

		//	CCLOG("Jump but has check on the floor");

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
	std::string str = (event == GameHud::EventType::BEGIN) ? "begin" : (event == GameHud::EventType::HOLD ? "Hold" : "end");


	artemis::Entity& e = world->getTagManager()->getEntity("goku");
	GokuProcessingSystem* gokuSystem = (GokuProcessingSystem*)(world->getSystemManager()->getSystem<GokuProcessingSystem>());

	if (event == GameHud::EventType::END && touchType == GameHud::TouchType::NONE){
		gokuSystem->actionStand();
	}

	// sự kiện người dùng release
	if (event == GameHud::EventType::END && touchType == GameHud::TouchType::LEFT){
		gokuSystem->actionStand();

	}
	if (event == GameHud::EventType::END && touchType == GameHud::TouchType::RIGHT){
		gokuSystem->actionStand();

	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::A){
		gokuSystem->actionKick1(R::AUTO);

	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::B){
		gokuSystem->actionPunch1(R::AUTO);

	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::X){
		gokuSystem->actionBeat1(R::AUTO);
	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::Y){
		gokuSystem->actionTrungDon(R::AUTO);
	}

	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::UP){
		gokuSystem->actionJump1(R::AUTO);

	}

	if (event == GameHud::EventType::HOLD && touchType == GameHud::TouchType::LEFT){
		gokuSystem->actionMoveOn(R::LEFT);

	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::LEFT){
		gokuSystem->actionMove(R::LEFT);

	}
	if (event == GameHud::EventType::HOLD && touchType == GameHud::TouchType::RIGHT){
		gokuSystem->actionMoveOn(R::RIGHT);

	}
	if (event == GameHud::EventType::BEGIN && touchType == GameHud::TouchType::RIGHT){
		gokuSystem->actionMove(R::RIGHT);

	}
}
void InputSystem::processEntity(artemis::Entity &e){


}



GokuProcessingSystem::GokuProcessingSystem(){
	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();

}
void GokuProcessingSystem::initialize(){
	posMapper.init(*world);
	physicMapper.init(*world);
	physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<PhysicSystem>());
	goku = &(world->getTagManager()->getEntity("goku"));

	skeletonAnimation = spine::SkeletonAnimation::createWithFile("spine/skeleton.json", "spine/skeleton.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);



}

void GokuProcessingSystem::processEntity(artemis::Entity &e){

	PosComponent* position = (PosComponent*)(posMapper.get(e));
	PhysicComponent* physic = (PhysicComponent*)(physicMapper.get(e));
	node->setPosition(Vec2(position->x, position->y));
	if (physic->vx == physic->vy && physic->vx == 0){

		//actionStand();
	}
	else{
		//	CCLOG("%f %f", physic->vx, physic->vy);
	}
}


void GokuProcessingSystem::actionStand(){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		//	CCLOG("Stand...........");
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Stand", true);
		skeletonAnimation->setToSetupPose();
		skeletonAnimation->setCompleteListener(nullptr);
		skeletonAnimation->setTimeScale(1);
		physicSystem->stopPhysic(*goku);
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving = false;
	}

}
void GokuProcessingSystem::actionStandUp(){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Stand up", true);
		physicSystem->stopPhysic(*goku);
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving = true;
	}
}
void GokuProcessingSystem::actionDie(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionMove(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		if (direction == R::Direction::RIGHT){
			physicSystem->push(*goku, 0, 200);
			physicSystem->clampVelocity(*goku, 0, 200);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
		}
		else if (direction == R::Direction::LEFT){
			physicSystem->push(*goku, 180, 200);
			physicSystem->clampVelocity(*goku, 0, 200);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
	}
}

void GokuProcessingSystem::actionMoveOn(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		((PhysicComponent*)goku->getComponent<PhysicComponent>())->isMoving = true;
		// xử lý action
		if (direction == R::Direction::RIGHT){
			physicSystem->push(*goku, 0, 200);
			physicSystem->clampVelocity(*goku, 0, 200);
		}
		else if (direction == R::Direction::LEFT){
			physicSystem->push(*goku, 180, 200);
			physicSystem->clampVelocity(*goku, 180, 200);
		}
	}
}

void GokuProcessingSystem::actionRun(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionStart(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionTrungDon(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		if (direction == R::Direction::LEFT){
			physicSystem->push(*goku, 180, 200);
			physicSystem->clampVelocity(*goku, 0, 200);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener([=](int trackIndex, int loopCount){
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x -= 20;
				actionStand();

			});
			node->setScaleX(1);
		}
		else 	if (direction == R::Direction::RIGHT){
			physicSystem->push(*goku, 0, 200);
			physicSystem->clampVelocity(*goku, 0, 200);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener([=](int trackIndex, int loopCount){
				actionStand();
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x += 20;
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO){

			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Trungdon", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener([=](int trackIndex, int loopCount){
				PosComponent* pos = (PosComponent*)(goku->getComponent<PosComponent>());
				pos->x -= node->getScaleX() == 1 ? 20 : -20;
				actionStand();

			});
		}




	}
}
void GokuProcessingSystem::actionVictory(){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionBeat1(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Beat1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackIndex, int loopCount){
			skeletonAnimation->setTimeScale(1);
			actionStand();

		});
	}
}
void GokuProcessingSystem::actionBeat2(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionBeat3(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionJump1(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		physicSystem->push(*goku, 90, 400);
		physicSystem->clampVelocity(*goku, 0, 400);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump1", false);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}
void GokuProcessingSystem::actionJump2(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionJump3(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionKick1(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Kick1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount){
			actionStand();
		});

		CCLOG("on Kick ");
	}
}
void GokuProcessingSystem::actionKick2(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionKick3(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionPunch1(R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(goku->getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		//skeletonAnimation->addAnimation(1, "Stand", true,.1f);
		skeletonAnimation->setCompleteListener([=](int trackIndex, int loopCount){
			actionStand();
		});
		CCLOG("on Kick ");
	}
}
void GokuProcessingSystem::actionPunch2(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void GokuProcessingSystem::actionPunch3(R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}





CharacterProcessingSystem::CharacterProcessingSystem(std::string &tag, std::string& skeletonDataFile, const std::string& atlasFile){

	addComponentType<PosComponent>();
	addComponentType<PhysicComponent>();
	this->tag = tag;
	this->skeletonDatafile = skeletonDataFile;
	this->skeletonAtlas = atlasFile;


}
void CharacterProcessingSystem::initialize(){
	posMapper.init(*world);
	physicMapper.init(*world);
	initSystem = false;
	character = nullptr;
	skeletonAnimation = nullptr;

}


void CharacterProcessingSystem::initSystemInformation(){
	if (!initSystem) return;
	physicSystem = (PhysicSystem*)(world->getSystemManager()->getSystem<PhysicSystem>());
	character = &(world->getTagManager()->getEntity("goku"));
	skeletonAnimation = spine::SkeletonAnimation::createWithFile(skeletonDatafile, skeletonAtlas);

	node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);

}

void CharacterProcessingSystem::begin(){
	if (!initSystem){
		initSystemInformation();
		initSystem = true;
	}
}

void CharacterProcessingSystem::processEntity(artemis::Entity &e){}

void CharacterProcessingSystem::actionStand(){}
void CharacterProcessingSystem::actionStandUp(){}
void CharacterProcessingSystem::actionDie(R::Direction direction){}
void CharacterProcessingSystem::actionMove(R::Direction direction){}
void CharacterProcessingSystem::actionMoveOn(R::Direction direction){}
void CharacterProcessingSystem::actionRun(R::Direction direction){}
void CharacterProcessingSystem::actionStart(R::Direction direction){}
void CharacterProcessingSystem::actionTrungDon(R::Direction direction){}
void CharacterProcessingSystem::actionVictory(){}
void CharacterProcessingSystem::actionBeat(R::Direction direction){}
void CharacterProcessingSystem::actionJump(R::Direction direction){}
void CharacterProcessingSystem::actionKick(R::Direction direction){}
void CharacterProcessingSystem::actionPunch(R::Direction direction){}
void CharacterProcessingSystem::actionThink(){}
void CharacterProcessingSystem::actionSpecialSkill(R::Direction direction){}





YamchaCharecter::YamchaCharecter(std::string &tag, std::string& skeletonDataFile, const std::string& atlasFile){
	CharacterProcessingSystem(tag,skeletonDataFile, atlasFile);
}


void YamchaCharecter::initialize(){
	posMapper.init(*world);
	physicMapper.init(*world);
	initSystem = false;
	character = nullptr;
	skeletonAnimation = nullptr;

}


void YamchaCharecter::initSystemInformation(){
	CharacterProcessingSystem::initSystemInformation();
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
}

void YamchaCharecter::begin(){
	CharacterProcessingSystem::begin();
}

void YamchaCharecter::processEntity(artemis::Entity &e){



}