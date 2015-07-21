#include "CharacterStateSystem.h"




CharacterStateSystem::~CharacterStateSystem()
{
}

CharacterStateSystem::CharacterStateSystem(){
	addComponentType<StateComponent>();
	addComponentType<SkeletonComponent>();
}

void CharacterStateSystem::initialize(){
	stateMapper.init(*world);
	skeletonMapper.init(*world);
}

void CharacterStateSystem::processEntity(artemis::Entity& e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	if (state->time_on_state == 0){
		changeState(e);
		state->customAnimation = false;
	}
	else{
		WallSensorComponent* wallSensor = (WallSensorComponent*)e.getComponent<WallSensorComponent>();
		PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
		if (wallSensor->onFloor && state->state == R::CharacterState::JUMP){
			state->state = R::CharacterState::STAND;
			state->animations.clear();
			state->animations.push_back("Stand");
			state->customAnimation = true;
			changeState(e);
			state->customAnimation = false;
		}
	}
	state->time_on_state += world->getDelta();
}
void CharacterStateSystem::changeState(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = skeletonMapper.get(e);
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	std::string strAnimation;
	float angle;
	float force;
	switch (state->state){
	case R::CharacterState::STAND:
		strAnimation = state->customAnimation ? state->animations.at(0) : "Stand";
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, state->animations.at(0), true);
		skeletonAnimation->setToSetupPose();
		skeletonAnimation->setCompleteListener(nullptr);
		skeletonAnimation->setTimeScale(1);
		physicSystem->stopPhysic(e);
		break;
	case R::CharacterState::JUMP:
		actionJump(e);

		break;
	case R::CharacterState::LEFT:
		actionMove(e);
		break;
	case R::CharacterState::RIGHT:
		actionMove(e);
		break;
	case R::CharacterState::WALK_LEFT:
		actionMoveOn(e);
		break;
	case R::CharacterState::WALK_RIGHT:
		actionMoveOn(e);
		break;
	case R::CharacterState::KICK:
		actionKick(e);
		break;
	case R::CharacterState::BEAT:
		actionBeat(e);
		break;
	case R::CharacterState::PUNCH:
		actionPunch(e);
		break;
	default:
		break;
	}
	state->customAnimation = false;
}

void CharacterStateSystem::actionMove(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	std::string animation = state->customAnimation ? state->animations.at(0) : "Move";

	CCLOG(state->animations.at(0).c_str());
	// xử lý action
	if (state->direction == R::Direction::RIGHT) {
		physicSystem->push(e, 0, 160);
		physicSystem->clampVelocity(e, 0, 160);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, animation, true);
		skeletonAnimation->setCompleteListener(nullptr);
		node->setScaleX(1);
	}else if (state->direction == R::Direction::LEFT) {
		physicSystem->push(e, 180, 160);
		physicSystem->clampVelocity(e, 0, 160);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, animation, true);
		skeletonAnimation->setCompleteListener(nullptr);
		node->setScaleX(-1);
	}else if (state->direction == R::Direction::AUTO){
		physicSystem->push(e, node->getScaleX() > 0 ? 0 : 180, 160);
		physicSystem->clampVelocity(e, 0, 160);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, animation, true);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}


void CharacterStateSystem::actionMoveOn(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	
	// xử lý action
	if (state->direction == R::Direction::RIGHT) {
	
		physicSystem->push(e, 0, 160);
		physicSystem->clampVelocity(e, 0, 160);
		node->setScaleX(1);
	}
	else if (state->direction == R::Direction::LEFT) {
		physicSystem->push(e, 180, 160);
		physicSystem->clampVelocity(e, 0, 160);
		node->setScaleX(-1);
	}
	else if (state->direction == R::Direction::AUTO) {
		physicSystem->push(e, node->getScaleX() > 0 ? 0 : 180, 160);
		physicSystem->clampVelocity(e, 0, 160);
	}
}
void CharacterStateSystem::actionStand(artemis::Entity& e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	state->customAnimation = true;
	state->animations.clear();
	state->animations.push_back("Stand");
	state->setState(R::CharacterState::STAND);
}

void CharacterStateSystem::actionKick(artemis::Entity& e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	std::string animation = state->customAnimation ? state->animations.at(0) : "Kick";

	// xử lý action
	skeletonAnimation->clearTracks();
	skeletonAnimation->setAnimation(0, animation, false);
	skeletonAnimation->setTimeScale(1.5f);
	skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
		state->customAnimation = true;
		state->animations.clear();
		state->animations.push_back("Stand");
		state->setState(R::CharacterState::STAND);
	});
}


void CharacterStateSystem::actionAttact(artemis::Entity& e){

}
void CharacterStateSystem::actionDie(artemis::Entity& e){

}
void CharacterStateSystem::actionPunch(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	std::string animation = state->customAnimation ? state->animations.at(0) : "Punch";

	// xử lý action
	skeletonAnimation->clearTracks();
	skeletonAnimation->setAnimation(0, animation, false);
	skeletonAnimation->setTimeScale(1.5f);
	skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
		state->customAnimation = true;
		state->animations.clear();
		state->animations.push_back("Stand");
		state->setState(R::CharacterState::STAND);
	});
}
void CharacterStateSystem::actionJump(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;

	std::string  strAnimation = state->customAnimation ? state->animations.at(0) : "Jump";
	float angle = state->direction == R::Direction::TOP ? 90 : (state->direction == R::Direction::TOP_LEFT ? 120 : (state->direction == R::Direction::TOP_RIGHT ? 60 : 90));
	float force = state->direction == R::Direction::TOP || state->direction == R::Direction::AUTO ? 300 : 400;
	// xử lý action
	physicSystem->push(e, angle, force);
	physicSystem->clampVelocity(e, 0, force);
	skeletonAnimation->clearTracks();
	skeletonAnimation->setAnimation(0, strAnimation, false);
	skeletonAnimation->setCompleteListener(nullptr);
}

void CharacterStateSystem::actionBeat(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PhysicSystem* physicSystem = (PhysicSystem*)world->getSystemManager()->getSystem<PhysicSystem>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;
	std::string animation = state->customAnimation ? state->animations.at(0) : "Beat";

	// xử lý action
	skeletonAnimation->clearTracks();
	skeletonAnimation->setAnimation(0, animation, false);
	skeletonAnimation->setTimeScale(1.5f);
	skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
		state->customAnimation = true;
		state->animations.clear();
		state->animations.push_back("Stand");
		state->setState(R::CharacterState::STAND);
	});
}