#include "Rua.h"


Rua::Rua(){}

Rua::~Rua(){}



void Rua::changeState(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	if (state->state == R::CharacterState::ATTACK){
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->type = state->attack;
		attackComponent->powerOfAttack = characterInfo->NORMAL_SKILL_POWER;
		if (state->attack == R::Attack::RUA_ATTACK1){
			actionAttack1(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
		}
		else if (state->attack == R::Attack::RUA_ATTACK2){
			actionAttack2(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 140;
		}
		else if (state->attack == R::Attack::RUA_ATTACK3){
			actionAttack3(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
			attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
			attackComponent->isSpecialSkill = true;
			attackComponent->manaOfAttack = 40;
		}
		else if (state->attack == R::Attack::RUA_ATTACK4){
			actionAttack4(e, state->direction);
			attackComponent->minX = position->x -120;
			attackComponent->maxX = position->x + 120;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
		}
		else if (state->attack == R::Attack::RUA_ATTACK5){
			actionAttack5(e, state->direction);
			attackComponent->minX = position->x - 100;
			attackComponent->maxX = position->x + 100;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
			attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
			attackComponent->isSpecialSkill = true;
			attackComponent->manaOfAttack = 40;

		}
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/enemy_attack.mp3", false, 1, 0, 1);
		}
		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
	else if (state->state == R::CharacterState::DIE){
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/enemy_death.mp3", false, 1, 0, 1);
		} 
		actionDie(e, state->direction);
	}
	else if (state->state == R::CharacterState::DEFENSE){
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/punch.mp3", false, 1, 0, 1);
		}
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

void Rua::actionStart(artemis::Entity &e, R::Direction direction){
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
void Rua::actionStand(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	skeleton->skeleton->clearTracks();
	skeleton->skeleton->setAnimation(0, "standing", true);
	skeleton->skeleton->setToSetupPose();
	skeleton->skeleton->setCompleteListener(nullptr);
	skeleton->skeleton->setTimeScale(1);
	EntityUtils::getInstance()->stopPhysic(e);
	state->doneAction = true;
}
void Rua::actionStandUp(artemis::Entity &e){
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
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "jumpback", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(1);
		}
		else if (state->direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "jumpback", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(-1);
		}
		else if (state->direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "jumpback", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}

}
void Rua::actionDie(artemis::Entity &e, R::Direction direction){
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

void Rua::actionTrungDon(artemis::Entity &e, R::Direction direction){
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
			skeletonAnimation->setTimeScale(1.5f);
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
			skeletonAnimation->setTimeScale(1.5f);
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
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}
}
void Rua::actionTrungDonNga(artemis::Entity &e, R::Direction direction){
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
void Rua::actionMove(artemis::Entity &e, R::Direction direction){
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
		std::string animation = "moving";
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

void Rua::actionAttack1(artemis::Entity &e, R::Direction direction){
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
void Rua::actionAttack2(artemis::Entity &e, R::Direction direction){
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
void Rua::actionAttack3(artemis::Entity &e, R::Direction direction){
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
void Rua::actionAttack4(artemis::Entity &e, R::Direction direction){
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
		std::string animation = "attacking-4";
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
void Rua::actionAttack5(artemis::Entity &e, R::Direction direction){
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
		std::string animation = "attacking-5";
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
