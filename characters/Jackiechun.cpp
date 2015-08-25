#include "Jackiechun.h"


Jackiechun::Jackiechun(){}

Jackiechun::~Jackiechun(){}


void Jackiechun::changeState(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	if (state->state == R::CharacterState::ATTACK){
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->type = state->attack;
		attackComponent->powerOfAttack = characterInfo->NORMAL_SKILL_POWER;
		if (state->attack == R::Attack::JACK_BEAT3){
			actionBeat3(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 70;
			attackComponent->maxY = position->y + 70;
		}
		else if (state->attack == R::Attack::JACK_KICK2){
			actionKick2(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 70;
			attackComponent->maxY = position->y + 70;
		}
		else if (state->attack == R::Attack::JACK_KICK3){
			actionKick3(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 100;
			attackComponent->maxY = position->y + 100;
		}
		else if (state->attack == R::Attack::JACK_PUNCH_1){
			actionPunch1(e, state->direction);
			attackComponent->minX = position->x - 70;
			attackComponent->maxX = position->x + 70;
			attackComponent->minY = position->y - 140;
			attackComponent->maxY = position->y + 140;
		}
		else if (state->attack == R::Attack::JACK_PUNCH_2){
			actionPunch2(e, state->direction);
			attackComponent->minX = position->x - 80;
			attackComponent->maxX = position->x + 80;
			attackComponent->minY = position->y - 140;
			attackComponent->maxY = position->y + 140;
		}
		else if (state->attack == R::Attack::JACK_PUNCH_AIR){
			actionPunchAir(e, state->direction);
			attackComponent->minX = position->x - 140;
			attackComponent->maxX = position->x + 140;
			attackComponent->minY = position->y - 140;
			attackComponent->maxY = position->y + 140;
		}
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::ENEMY_ATTACK, false, 1, 0, 1);
		}
		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
	else if (state->state == R::CharacterState::DIE){
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::ENEMY_DEATH, false, 1, 0, 1);
		}
		actionDie(e, state->direction); }
	else if (state->state == R::CharacterState::DEFENSE){ 
		if (R::Constants::soundEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::ENEMY_HIT3, false, 1, 0, 1);
		}
		actionTrungDon(e, state->direction); }
	else if (state->state == R::CharacterState::START){ actionStart(e,state->direction); }
	else if (state->state == R::CharacterState::STAND){ actionStand(e); }
	else if (state->state == R::CharacterState::STAND_UP){ actionStandUp(e); }
	else if (state->state == R::CharacterState::LEFT){ actionMove(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::RIGHT){ actionMove(e, R::Direction::RIGHT); }
	else if (state->state == R::CharacterState::WALK_LEFT){ actionRun(e, R::Direction::LEFT); }
	else if (state->state == R::CharacterState::WALK_RIGHT){ actionRun(e, R::Direction::RIGHT); }
	else if (state->state == R::CharacterState::JUMP){ actionJump(e, R::Direction::RIGHT); }

}

void Jackiechun::actionStart(artemis::Entity &e,R::Direction direction){
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
void Jackiechun::actionStand(artemis::Entity &e){
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
		
		skeleton->skeleton->clearTracks();
		skeleton->skeleton->setAnimation(0, "Stand", true);
		skeleton->skeleton->setToSetupPose();
		skeleton->skeleton->setCompleteListener(nullptr);
		skeleton->skeleton->setTimeScale(1);
		EntityUtils::getInstance()->stopPhysic(e);
	
}
void Jackiechun::actionStandUp(artemis::Entity &e){
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
			skeletonAnimation->setAnimation(0, "Stand up", false);
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
			skeletonAnimation->setAnimation(0, "Stand up", false);
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
			skeletonAnimation->setAnimation(0, "Stand up", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
		}
	}

}
void Jackiechun::actionDie(artemis::Entity &e, R::Direction direction){
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
		skeletonAnimation->setAnimation(0, "Die", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}

void Jackiechun::actionTrungDon(artemis::Entity &e, R::Direction direction){
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
				EntityUtils::getInstance()->push(e, 180, 160);
				EntityUtils::getInstance()->clampVelocity(e, 0, 160);
				skeletonAnimation->clearTracks();
				skeletonAnimation->setAnimation(0, "Trungdon", false);
				skeletonAnimation->setTimeScale(1.5f);
				skeletonAnimation->setCompleteListener(
					[=](int trackIndex, int loopCount) {
					state->setState(R::CharacterState::STAND);
				});
				node->setScaleX(1);
			}
			else if (direction == R::Direction::RIGHT) {
				EntityUtils::getInstance()->push(e, 0, 160);
				EntityUtils::getInstance()->clampVelocity(e, 0, 160);
				skeletonAnimation->clearTracks();
				skeletonAnimation->setAnimation(0, "Trungdon", false);
				skeletonAnimation->setTimeScale(1.5f);
				skeletonAnimation->setCompleteListener(
					[=](int trackIndex, int loopCount) {
					state->setState(R::CharacterState::STAND);
				});
				node->setScaleX(-1);
			}
			else if (direction == R::Direction::AUTO) {
				EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 160);
				EntityUtils::getInstance()->clampVelocity(e, 0, 160);
				skeletonAnimation->clearTracks();
				skeletonAnimation->setAnimation(0, "Trungdon", false);
				skeletonAnimation->setTimeScale(1.5f);
				skeletonAnimation->setCompleteListener(
					[=](int trackIndex, int loopCount) {
					state->setState(R::CharacterState::STAND);
				});
			}
		}

}
void Jackiechun::actionGetHit1(artemis::Entity &e, R::Direction direction){
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
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "GetHit1", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "GetHit1", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND);
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "GetHit1", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {

				state->setState(R::CharacterState::STAND);
			});
		}
	}

}
void Jackiechun::actionBack(artemis::Entity &e, R::Direction direction){
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
			EntityUtils::getInstance()->push(e, 140, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Back", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr	);
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 40, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Back", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 40 : 140, 220);
			EntityUtils::getInstance()->clampVelocity(e, 0, 220);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Back", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr);
		}
	}

}

void Jackiechun::actionMove(artemis::Entity &e, R::Direction direction){
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
void Jackiechun::actionRun(artemis::Entity &e, R::Direction direction){
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
		std::string animation = "Run";
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
// thằng này có 2 loại Jump, 1 loại jump bình thường, một loại xoay vòng
void Jackiechun::actionJump(artemis::Entity &e, R::Direction direction){
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
		float angle = state->direction == R::Direction::TOP ? 90 : (state->direction == R::Direction::TOP_LEFT ? 135 : (state->direction == R::Direction::TOP_RIGHT ? 45 : 90));
		float force = state->direction == R::Direction::TOP || state->direction == R::Direction::AUTO ? 300 : 400;
		// xử lý action
		EntityUtils::getInstance()->push(e, angle, force);
		EntityUtils::getInstance()->clampVelocity(e, 0, force);
		if (skeletonAnimation->getCurrent() != 0){
			std::string animation = skeletonAnimation->getCurrent()->animation->name;
			animation = (animation.compare("Run") != 0) ? "Jump" : "Jump3";
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, animation, false);
			skeletonAnimation->setCompleteListener(nullptr);
		}
	}
}


void Jackiechun::actionPunchAir(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch air", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}

}
void Jackiechun::actionPunch1(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}

}
void Jackiechun::actionPunch2(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch2", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}

}
void Jackiechun::actionKick2(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Kick2", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}

}
void Jackiechun::actionKick3(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Kick3", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}
}
void Jackiechun::actionBeat3(artemis::Entity &e, R::Direction direction){
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
		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Beat3", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}
}
