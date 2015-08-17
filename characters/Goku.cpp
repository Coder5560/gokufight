#include "Goku.h"
#include "renders/Effects.h"
Goku::Goku() :timeHoldInput(0){}


Goku::~Goku()
{
}
void Goku::changeState(artemis::Entity &e){
	StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	SkeletonComponent* characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	if (state->state == R::CharacterState::ATTACK){
		PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->type = state->attack;
		attackComponent->powerOfAttack = characterInfo->NORMAL_SKILL_POWER;

		if (state->attack == R::Attack::GOKU_BEAT1){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionBeat1(e, state->direction);
			attackComponent->minX = position->x - 60;
			attackComponent->maxX = position->x + 60;
			attackComponent->minY = position->y - 80;
			attackComponent->maxY = position->y + 80;

		}
		else if (state->attack == R::Attack::GOKU_BEAT2){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionBeat2(e, state->direction);
			attackComponent->minX = position->x - 80;
			attackComponent->maxX = position->x + 80;
			attackComponent->minY = position->y - 80;
			attackComponent->maxY = position->y + 80;


		}
		else if (state->attack == R::Attack::GOKU_BEAT3){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionBeat3(e, state->direction);
			attackComponent->minX = position->x - 80;
			attackComponent->maxX = position->x + 80;
			attackComponent->minY = position->y - 40;
			attackComponent->maxY = position->y + 140;
		}
		else if (state->attack == R::Attack::GOKU_PUNCH1){
			if (characterInfo->hasManaForSkill(40)){
				actionPunch1(e, state->direction);
				Node* node = RenderLayer::getInstance()->createGameNode();
				node->setPosition(Vec2(position->x, position->y));
				node->setScale(.2f);
				KameKameHa* kame = new KameKameHa(node);
				kame->setTarget("enemy");
				kame->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
				kame->direction = (characterSkeleton->node->getScaleX() < 0) ? -1 : 1;
				artemis::Entity &entity = EntityUtils::getInstance()->getWorld()->createEntity();
				entity.addComponent(new SkillComponent(kame));
				entity.refresh();
				entity.setGroup("gokuattack");
				characterInfo->power -= 40;
			}
			else{
				Node* node = RenderLayer::getInstance()->createGameNode();
				node->setPosition(Vec2(position->x, position->y + 60));
				NotEnoughManaEffect* effect = new NotEnoughManaEffect(node);
				effect->start();
				state->setState(R::CharacterState::STAND);
			}
			return;
		}
		else if (state->attack == R::Attack::GOKU_PUNCH2){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionPunch2(e, state->direction);
			attackComponent->minX = position->x - 100;
			attackComponent->maxX = position->x + 100;
			attackComponent->minY = position->y - 120;
			attackComponent->maxY = position->y + 120;

		}

		else if (state->attack == R::Attack::GOKU_KICK1){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionKick1(e, state->direction);
			attackComponent->minX = position->x - 30;
			attackComponent->maxX = position->x + 30;
			attackComponent->minY = position->y - 30;
			attackComponent->maxY = position->y + 30;
		}
		else if (state->attack == R::Attack::GOKU_KICK2){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionKick2(e, state->direction);
			attackComponent->minX = position->x - 30;
			attackComponent->maxX = position->x + 30;
			attackComponent->minY = position->y - 40;
			attackComponent->maxY = position->y + 60;
		}
		else if (state->attack == R::Attack::GOKU_KICK3){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_attack.mp3", false, 1, 0, 1);
			}
			actionKick3(e, state->direction);
			attackComponent->minX = position->x - 30;
			attackComponent->maxX = position->x + 30;
			attackComponent->minY = position->y - 40;
			attackComponent->maxY = position->y + 60;
		}
		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
		state->state == R::CharacterState::STAND;
	}
	else if (state->state == R::CharacterState::DEFENSE){
		
		EntityUtils::getInstance()->removeGroup("gokuattack");
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/goku_behit.mp3", false, 1, 0, 1);
		}
		if (state->defense == R::Defense::TRUNG_DON) {
			actionTrungDon(e, state->direction);
		}
		if (state->defense == R::Defense::TRUNG_DON_NGA)  {
			actionTrungDonNga(e, state->direction);
		}
	}
	else if (state->state == R::CharacterState::WIN){ actionVictory(e); }
	else if (state->state == R::CharacterState::DIE){
		EntityUtils::getInstance()->removeGroup("gokuattack");
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/enemy_death_3.mp3", false, 1, 0, 1);
		}
		actionDie(e, state->direction);
	}
	else if (state->state == R::CharacterState::START){ actionStart(e, state->direction); }
	else if (state->state == R::CharacterState::STAND){ actionStand(e); }
	else if (state->state == R::CharacterState::STAND_UP){ actionStandUp(e); }
	else if (state->state == R::CharacterState::LEFT){ actionMove(e, R::Direction::LEFT); timeHoldInput = 0; }
	else if (state->state == R::CharacterState::RIGHT){ actionMove(e, R::Direction::RIGHT); timeHoldInput = 0; }
	else if (state->state == R::CharacterState::WALK_RIGHT){ actionMoveOn(e, R::Direction::RIGHT); timeHoldInput += .017; }
	else if (state->state == R::CharacterState::WALK_LEFT){ actionMoveOn(e, R::Direction::LEFT); timeHoldInput += .017; }
	else if (state->state == R::CharacterState::JUMP){
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/jump.mp3", false, 1, 0, 1);
		}
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
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		SkeletonComponent* skeleton = (SkeletonComponent*)e.getComponent<
			SkeletonComponent>();
		spine::SkeletonAnimation* skeletonAnimation = skeleton->skeleton;
		Node* node = skeleton->node;
		if (state->direction == R::Direction::LEFT) {
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
void Goku::actionDie(artemis::Entity &e, R::Direction direction) {
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
		if (direction == R::Direction::LEFT) {
			EntityUtils::getInstance()->push(e, 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 0, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr);
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 0 : 180, 160);
			EntityUtils::getInstance()->clampVelocity(e, 0, 160);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(nullptr);
		}
	}
}
void Goku::actionMove(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionMoveOn(artemis::Entity &e, R::Direction direction) {
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());
	bool dudieukien = wallSensor->onFloor;
	if (!dudieukien) {
		return;
	}
	else {

		// xử lý action
		StateComponent* state = (StateComponent*)e.getComponent<StateComponent>();
		//	CCLOG("Time on State : %f ", timeHoldInput);
		if (timeHoldInput >= .4 && (state->state == R::CharacterState::WALK_LEFT || state->state == R::CharacterState::WALK_RIGHT)){

			actionRun(e, direction);

		}
		else{

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
}
void Goku::actionRun(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionStart(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionTrungDon(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionTrungDonNga(artemis::Entity &e, R::Direction direction) {
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
			EntityUtils::getInstance()->push(e, 140, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
			node->setScaleX(1);
		}
		else if (direction == R::Direction::RIGHT) {
			EntityUtils::getInstance()->push(e, 40, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
			node->setScaleX(-1);
		}
		else if (direction == R::Direction::AUTO) {
			EntityUtils::getInstance()->push(e, node->getScaleX() == 1 ? 40 : 140, 300);
			EntityUtils::getInstance()->clampVelocity(e, 0, 300);
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, "Die", false);
			skeletonAnimation->setTimeScale(1.5f);
			skeletonAnimation->setCompleteListener(
				[=](int trackIndex, int loopCount) {
				state->setState(R::CharacterState::STAND_UP);
			});
		}
	}
}
void Goku::actionVictory(artemis::Entity &e) {



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
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Victory", false);
		skeletonAnimation->setTimeScale(1);
		skeletonAnimation->setCompleteListener(nullptr);

	}
}
void Goku::actionBeat1(artemis::Entity &e, R::Direction direction) {
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
		skeletonAnimation->setAnimation(0, "Beat1", false);
		skeletonAnimation->setTimeScale(2.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
	}
}
void Goku::actionBeat2(artemis::Entity &e, R::Direction direction) {
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
		skeletonAnimation->setAnimation(0, "Beat2", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});



	}
}
void Goku::actionBeat3(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionJump1(artemis::Entity &e, R::Direction direction) {
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
			animation = (animation.compare("Run") != 0) ? "Jump1" : "Jump3";
			skeletonAnimation->clearTracks();
			skeletonAnimation->setAnimation(0, animation, false);
			skeletonAnimation->setCompleteListener(nullptr);
		}
	}
}
void Goku::actionJump2(artemis::Entity &e, R::Direction direction) {
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
		float angle = state->direction == R::Direction::TOP ? 90 : (state->direction == R::Direction::TOP_LEFT ? 120 : (state->direction == R::Direction::TOP_RIGHT ? 60 : 90));
		float force = state->direction == R::Direction::TOP || state->direction == R::Direction::AUTO ? 300 : 400;
		// xử lý action
		EntityUtils::getInstance()->push(e, angle, force);
		EntityUtils::getInstance()->clampVelocity(e, 0, force);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump2", false);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}
void Goku::actionJump3(artemis::Entity &e, R::Direction direction) {
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
		float angle = state->direction == R::Direction::TOP ? 90 : (state->direction == R::Direction::TOP_LEFT ? 120 : (state->direction == R::Direction::TOP_RIGHT ? 60 : 90));
		float force = state->direction == R::Direction::TOP || state->direction == R::Direction::AUTO ? 300 : 400;
		// xử lý action
		EntityUtils::getInstance()->push(e, angle, force);
		EntityUtils::getInstance()->clampVelocity(e, 0, force);
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Jump3", false);
		skeletonAnimation->setCompleteListener(nullptr);
	}
}
void Goku::actionKick1(artemis::Entity &e, R::Direction direction) {
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
		skeletonAnimation->setAnimation(0, "Kick1", false);
		skeletonAnimation->setTimeScale(1.5f);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});

	}
}
void Goku::actionKick2(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionKick3(artemis::Entity &e, R::Direction direction) {
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
void Goku::actionPunch1(artemis::Entity &e, R::Direction direction) {
	WallSensorComponent* wallSensor = (WallSensorComponent*)(e.getComponent<WallSensorComponent>());

	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();


	bool dudieukien = wallSensor->onFloor && characterInfo->hasManaForSkill(40);
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
		artemis::Entity *entity = &e;

		// xử lý action
		skeletonAnimation->clearTracks();
		skeletonAnimation->setAnimation(0, "Punch1", false);
		skeletonAnimation->setTimeScale(2);
		skeletonAnimation->setCompleteListener([this, state, entity](int trackID, int loopCount){
			state->setState(R::CharacterState::STAND);
		});
	}
}
void Goku::actionPunch2(artemis::Entity &e, R::Direction direction) {
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
		skeletonAnimation->setTimeScale(2);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {
			state->setState(R::CharacterState::STAND);
		});
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->powerOfAttack = 10;

		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
}
void Goku::actionPunch3(artemis::Entity &e, R::Direction direction) {
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
		skeletonAnimation->setAnimation(0, "Punch3", false);
		skeletonAnimation->setTimeScale(2);
		skeletonAnimation->setCompleteListener([=](int trackID, int loopCount) {

			state->setState(R::CharacterState::STAND);
		});
		AttackComponent* attackComponent = new AttackComponent();
		attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
		attackComponent->powerOfAttack = 10;

		EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
	}
}
