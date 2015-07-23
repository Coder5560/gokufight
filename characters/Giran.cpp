#include "Giran.h"


Giran::Giran()
{
}


Giran::~Giran()
{
}


void Giran::changeState(artemis::Entity &e){

	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	if (state->state == R::CharacterState::ATTACK){
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->powerOfAttack = 5;
		if (state->attack == R::Attack::GIRAN_PUNCH1){ CCLOG("Attack giran"); actionPunch1(e, state->direction);  attackComponent->maxTimeAlive = 4; attackComponent->timeAttack = .4; }
		else if (state->attack == R::Attack::GIRAN_PUNCH2){ actionPunch2(e, state->direction);  attackComponent->maxTimeAlive = 4; attackComponent->timeAttack = .2; }
		else if (state->attack == R::Attack::GIRAN_PUNCH3){ actionPunch3(e, state->direction);  attackComponent->maxTimeAlive = 4; attackComponent->timeAttack = .2; }
		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
		state->state == R::CharacterState::STAND;
	}
	else if (state->state == R::CharacterState::DEFENSE){ actionTrungDon(e, state->direction); }
	else if (state->state == R::CharacterState::STAND){ actionStand(e); }
	else if (state->state == R::CharacterState::LEFT){ actionMove(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::RIGHT){ actionMove(e, R::Direction::RIGHT); }
	else if (state->state == R::CharacterState::WALK_RIGHT){ actionMoveOn(e, R::Direction::RIGHT); }
	else if (state->state == R::CharacterState::WALK_LEFT){ actionMoveOn(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::JUMP){ actionJump(e, R::Direction::AUTO); }

}
void Giran::actionTrungDon(artemis::Entity &e, R::Direction direction){

	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	skeleton->skeleton->clearTracks();
	skeleton->skeleton->setAnimation(0, "Stand", true);
	skeleton->skeleton->setToSetupPose();
	skeleton->skeleton->setCompleteListener(nullptr);
	skeleton->skeleton->setTimeScale(1);
//	if (direction == R::Direction::LEFT){ skeleton->node->setScaleX(1); }
//	if (direction == R::Direction::RIGHT){ skeleton->node->setScaleX(-1); }

	if (direction == R::Direction::LEFT){
		EntityUtils::getInstance()->push(e, 180, 240);
	}
	else if (direction == R::Direction::RIGHT){
		EntityUtils::getInstance()->push(e, 0, 240);
	}
}
void Giran::actionStand(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	skeleton->skeleton->clearTracks();
	skeleton->skeleton->setAnimation(0, "Stand", true);
	skeleton->skeleton->setToSetupPose();
	skeleton->skeleton->setCompleteListener(nullptr);
	skeleton->skeleton->setTimeScale(1);
	EntityUtils::getInstance()->stopPhysic(e);
}
void Giran::actionDie(artemis::Entity &e, R::Direction direction){	}
void Giran::actionMove(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO){
			EntityUtils::getInstance()->push(e, node->getScaleX() > 0 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Move", true);
			skeletonAnimation->setCompleteListener(nullptr);
		}
	}
}
void Giran::actionMoveOn(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 100);
			EntityUtils::getInstance()->clampVelocity(e, 0, 100);
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 100);
			EntityUtils::getInstance()->clampVelocity(e, 0, 100);
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() > 0 ? 0 : 180, 100);
			EntityUtils::getInstance()->clampVelocity(e, 0, 100);
		}
	}
}
void Giran::actionJump(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		float angle = state->direction == R::Direction::TOP ? 90 : (state->direction == R::Direction::TOP_LEFT ? 120 : (state->direction == R::Direction::TOP_RIGHT ? 60 : 90));
		float force = state->direction == R::Direction::TOP || state->direction == R::Direction::AUTO ? 300 : 400;
		EntityUtils::getInstance()->push(e, angle, force);
		EntityUtils::getInstance()->clampVelocity(e, 0, force);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump", false);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}

void Giran::actionPunch1(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();

		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});

	}

}
void Giran::actionPunch2(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();

		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch2", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});

	}


}
void Giran::actionPunch3(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();

		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch3", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});

	}
}