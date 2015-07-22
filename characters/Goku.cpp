#include "Goku.h"
#include "cocos2d.h"
#include "gokuartermis/Components.h"
#include "systems/EntityUtils.h"
Goku::Goku()
{
}


Goku::~Goku()
{
}
void Goku::changeState(artemis::Entity &e){
	CCLOG("Change State in Goku");
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	if (state->state == R::CharacterState::BEAT){
		actionBeat1(e, state->direction);
	}
	if (state->state == R::CharacterState::STAND){
		actionStand(e);
	}
	if (state->state == R::CharacterState::LEFT){
		actionMove(e, R::Direction::LEFT);
	}
	if (state->state == R::CharacterState::RIGHT){
		actionMove(e, R::Direction::RIGHT);
	}
	if (state->state == R::CharacterState::WALK_RIGHT){
		actionMoveOn(e, R::Direction::RIGHT);
	}
	if (state->state == R::CharacterState::WALK_LEFT){
		actionMoveOn(e, R::Direction::LEFT);
	}
	if (state->state == R::CharacterState::JUMP){
		actionJump1(e, R::Direction::AUTO);
	}
}
void Goku::actionStand(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
	skeleton->skeleton->clearTracks();
	skeleton->skeleton->setAnimation(0, "Stand", true);
	skeleton->skeleton->setToSetupPose();
	skeleton->skeleton->setCompleteListener(nullptr);
	skeleton->skeleton->setTimeScale(1);

	physic->vx = 0; physic->vy = 0;
}


void Goku::actionStandUp(artemis::Entity &e) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionDie(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionMove(artemis::Entity &e, R::Direction direction) {
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
		std::string animation = state->customAnimation ? state->animations.at(0) : "Move";

		CCLOG(state->animations.at(0).c_str());
		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, animation, true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, animation, true);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO){
			EntityUtils::getInstance()->push(e, node->getScaleX() > 0 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, animation, true);
			skeletonAnimation->setCompleteListener(nullptr);
		}













	}
}

void Goku::actionMoveOn(artemis::Entity &e, R::Direction direction) {
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
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() > 0 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
		}
	}
}

void Goku::actionRun(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionStart(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionTrungDon(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {

	}

}
void Goku::actionVictory(artemis::Entity &e) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionBeat1(artemis::Entity &e, R::Direction direction) {
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
		SkeletonComponent>();
	spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
	Node* node = skeleton->node;


	// xử lý action
	skeletonAnimation->clearTracks();
	skeletonAnimation->setAnimation(0, "Beat1", false);
	skeletonAnimation->setTimeScale(1.5f);
	skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
		state->customAnimation = true;
		state->animations.clear();
		state->animations.push_back("Stand");
		state->setState(R::CharacterState::STAND);
	});

	AttackComponent* attackComponent = new AttackComponent();
	attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
	attackComponent->powerOfAttack = 10;
}
void Goku::actionBeat2(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionBeat3(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionJump1(artemis::Entity &e, R::Direction direction) {
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
		// xử lý action
		EntityUtils::getInstance()->push(e, angle, force);
		EntityUtils::getInstance()->clampVelocity(e, 0, force);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump1", false);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}
void Goku::actionJump2(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionJump3(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionKick1(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionKick2(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionKick3(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionPunch1(artemis::Entity &e, R::Direction direction) {
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
			state->customAnimation = true;
			state->animations.clear();
			state->animations.push_back("Stand");
			state->setState(R::CharacterState::STAND);
		});
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->powerOfAttack = 10;

		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
}
void Goku::actionPunch2(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}
void Goku::actionPunch3(artemis::Entity &e, R::Direction direction) {
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		// xử lý action
	}
}