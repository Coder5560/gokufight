#include "Picolo.h"


Picolo::Picolo(){}
Picolo::~Picolo(){}


void Picolo::changeState(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	if (state->state == R::CharacterState::ATTACK){
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->type = state->attack;
		attackComponent->powerOfAttack = characterInfo->NORMAL_SKILL_POWER;
		if (state->attack == R::Attack::PICOLO_ATTACK1){
			actionAttack1(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 300;
		}
		else if (state->attack == R::Attack::PICOLO_ATTACK2){
			actionAttack2(e, state->direction);
			attackComponent->minX = position->x - 60;
			attackComponent->maxX = position->x + 60;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 140;
		}
		else if (state->attack == R::Attack::PICOLO_ATTACK3){
			actionAttack3(e, state->direction);
			attackComponent->minX = position->x - 100;
			attackComponent->maxX = position->x + 100;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
			
		}
		else if (state->attack == R::Attack::PICOLO_KICK){
			actionAttackKick(e, state->direction);
			attackComponent->minX = position->x - 50;
			attackComponent->maxX = position->x + 50;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
		}
		else if (state->attack == R::Attack::PICOLO_POWER1){
			actionAttackPower1(e, state->direction);
			attackComponent->minX = position->x - 100;
			attackComponent->maxX = position->x + 100;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
			
			attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
			attackComponent->isSpecialSkill = true;
			attackComponent->manaOfAttack = 40;
		
		}
		else if (state->attack == R::Attack::PICOLO_POWER2){
			actionAttackPower2(e, state->direction);
			attackComponent->minX = position->x - 100;
			attackComponent->maxX = position->x + 100;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
			attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
			attackComponent->isSpecialSkill = true;
			attackComponent->manaOfAttack = 40;
			
		}

		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
	else if (state->state == R::CharacterState::DIE){ actionDie(e, state->direction); }
	else if (state->state == R::CharacterState::DEFENSE){
		if (state->defense == R::Defense::TRUNG_DON)  actionTrungDon(e, state->direction);
		if (state->defense == R::Defense::TRUNG_DON_NGA)  actionTrungDonNga(e, state->direction);
	}
	else if (state->state == R::CharacterState::START){
		//do nothing coz there is no animation for "start"
	}
	else if (state->state == R::CharacterState::STAND){ actionStand(e); }
	else if (state->state == R::CharacterState::STAND_UP){ actionStandUp(e); }
	else if (state->state == R::CharacterState::LEFT){ actionMove(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::RIGHT){ actionMove(e, R::Direction::RIGHT); }
	else if (state->state == R::CharacterState::WALK_LEFT){ actionMove(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::WALK_RIGHT){ actionMove(e, R::Direction::RIGHT); }

}

void Picolo::actionStart(artemis::Entity &e, R::Direction direction){
	// this character doesnt have start state
	return;
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		if (direction == R::Direction::LEFT) {
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Start", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::RIGHT) {
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Start", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::AUTO) {
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Start", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}

}
void Picolo::actionStand(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	skeleton->skeleton->clearTracks();
	skeleton->skeleton->setAnimation(0, "flying", true);
	skeleton->skeleton->setToSetupPose();
	skeleton->skeleton->setCompleteListener(nullptr);
	skeleton->skeleton->setTimeScale(1);
	EntityUtils::getInstance()->stopPhysic(e);
	state->doneAction = true;
}
void Picolo::actionStandUp(artemis::Entity &e){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "standup", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "standup", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "standup", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}

}
void Picolo::actionDie(artemis::Entity &e, R::Direction direction){
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
		skeletonAnimation->setAnimation(0, "die", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}

void Picolo::actionTrungDon(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		PosComponent* pos = (PosComponent*)(e.getComponent<PosComponent>());
		if (direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "hitting", false);
			skeletonAnimation->setTimeScale(3.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "hitting", false);
			skeletonAnimation->setTimeScale(3.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 60);
			EntityUtils::getInstance()->clampVelocity(e, 0, 60);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "hitting", false);
			skeletonAnimation->setTimeScale(3.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}
}
void Picolo::actionTrungDonNga(artemis::Entity &e, R::Direction direction){
	bool dudieukien = true;
	if (!dudieukien) {
		return;
	}
	else {
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		PosComponent* pos = (PosComponent*)(e.getComponent<PosComponent>());
		if (direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "die", false);
			skeletonAnimation->setTimeScale(1);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "die", false);
			skeletonAnimation->setTimeScale(1);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "die", false);
			skeletonAnimation->setTimeScale(1);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
		}
	}
}
void Picolo::actionMove(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "flying";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener(nullptr);
			skeleton->skeleton->setTimeScale(1);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() > 0 ? 0 : 180, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
		}
	}
}

void Picolo::actionAttack1(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "attacking-1";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setTimeScale(1.5f);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});

		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
void Picolo::actionAttack2(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "attacking-2";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});
			skeleton->skeleton->setTimeScale(1.5f);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
void Picolo::actionAttack3(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "attacking-3";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});
			skeleton->skeleton->setTimeScale(1.5f);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
void Picolo::actionAttackKick(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "kick";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});
			skeleton->skeleton->setTimeScale(1.5f);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
void Picolo::actionAttackPower1(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "power-1";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});
			skeleton->skeleton->setTimeScale(1.5f);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
void Picolo::actionAttackPower2(artemis::Entity &e, R::Direction direction){
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
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
		std::string animation = "power-2";
		if (skeletonAnimation->getCurrent() && skeletonAnimation && animation.compare(skeletonAnimation->getCurrent()->animation->name) != 0){
			skeleton->skeleton->clearTracks();
			skeleton->skeleton->setAnimation(0, animation, true);
			skeleton->skeleton->setCompleteListener([=](int trackID, int loopCount){
				state->setState(R::CharacterState::STAND);
			});
			skeleton->skeleton->setTimeScale(1.5f);
		}

		// xử lý action
		if (state->direction == R::Direction::RIGHT) {
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::LEFT) {
			node->setScaleX(-1);
		}

	}
}
