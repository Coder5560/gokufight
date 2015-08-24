#include "KarillinAI.h"


KarillinAI::KarillinAI() {
	this->isActive = true;
	mapInfo = new MapInfo();
}
KarillinAI::~KarillinAI() {
}

void KarillinAI::obsever(artemis::Entity &e) {
	artemis::Entity &gameState = world->getTagManager()->getEntity("gameState");
	GameStateComponent* gameStateComponent =
		(GameStateComponent*)gameState.getComponent<GameStateComponent>();
	if (gameStateComponent->gameState == R::GameState::LOSE) {
		if (characterState->state != R::CharacterState::STAND) {
			characterState->setState(R::CharacterState::STAND);
		}
		return;
	}

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	targetBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	targetPosition = (PosComponent*)goku.getComponent<PosComponent>();
	characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	targetState = (StateComponent*)goku.getComponent<StateComponent>();
	characterState = (StateComponent*)e.getComponent<StateComponent>();
	targetInfo = (CharacterInfoComponent*)goku.getComponent<
		CharacterInfoComponent>();
	characterInfo = (CharacterInfoComponent*)e.getComponent<
		CharacterInfoComponent>();
	targetPhysic = (PhysicComponent*)goku.getComponent<PhysicComponent>();
	characterPhysic = (PhysicComponent*)e.getComponent<PhysicComponent>();
	targetSkeleton =
		(SkeletonComponent*)goku.getComponent<SkeletonComponent>();
	characterSkeleton =
		(SkeletonComponent*)e.getComponent<SkeletonComponent>();

	if (characterState->time_on_state >= 1) {
		characterState->setState(R::CharacterState::STAND);
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
			< (characterPosition->x + characterBound->x1);
		characterState->direction =
			targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		return;
	}

	// trường hợp đánh thằng goku ngã
	if (characterState->state == R::CharacterState::DEFENSE
		&& (targetState->defense == R::Defense::TRUNG_DON_NGA
		|| targetState->state == R::CharacterState::STAND_UP)) {
		return;
	}

	// trường hợp đang dính đòn thì k làm gì
	if (characterState->state == R::CharacterState::DEFENSE
		&& (characterState->defense == R::Defense::TRUNG_DON_NGA
		|| characterState->state == R::CharacterState::STAND_UP)) {
		return;
	}
	if (characterState->state == R::CharacterState::DEFENSE
		&& characterState->defense == R::Defense::TRUNG_DON) {
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK) {
		return;
	}

	float rangeAttack = -4;
	float distance = abs(targetPosition->x - characterPosition->x);

	processAvoidAttack();

	if (distance > targetBound->getWidth() + rangeAttack)
		processOutRangeAttack();
	else
		processInRangeAttack();
}
void KarillinAI::processAvoidAttack() {
	if (world->getTagManager()->isSubscribed("gokupunch")) {
		artemis::Entity &gokuPunch = world->getTagManager()->getEntity(
			"gokupunch");
		SkillComponent* skill = (SkillComponent*)gokuPunch.getComponent<
			SkillComponent>();
		Rect characterRect = Rect(characterPosition->x + characterBound->x1,
			characterPosition->y + characterBound->y1,
			characterBound->getWidth(), characterBound->getHeight());
		bool inRangeSkillAttackLeft = skill->kamekameha->direction > 0
			&& (characterRect.getMinX()
			- skill->kamekameha->node->getPositionX()) > 20;
		bool inRangeSkillAttackRight = skill->kamekameha->direction < 0
			&& (skill->kamekameha->node->getPositionX()
			- characterRect.getMaxX()) > 20;
		if (inRangeSkillAttackLeft || inRangeSkillAttackRight) {
			if (characterState->state != R::CharacterState::JUMP
				&& characterState->state != R::CharacterState::DEFENSE) {
				characterState->setState(R::CharacterState::JUMP);
				characterState->jump = true;
				return;
			}
		}
	}
}

void KarillinAI::closeRangeAttack() {
	float thinkingTime = .02f;
	if ((characterState->state == R::CharacterState::STAND || characterState->state == R::CharacterState::DEFENSE) && characterState->time_on_state >= thinkingTime) { // attack
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2) < (characterPosition->x + characterBound->x1);
		float distance = characterPosition->x - targetPosition->x;

		//characterState->direction = targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		//characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);

		srand(time(NULL));
		int random = rand() % 9 + 1;
		if (random == 1){
			characterState->attack = R::Attack::KARILLIN_KICK1;
			characterState->setState(R::CharacterState::ATTACK);
		}
		else if (random == 2){
			characterState->attack = R::Attack::KARILLIN_KICK3;
			characterState->setState(R::CharacterState::ATTACK);
		}
		else if (random == 3){
			characterState->attack = R::Attack::KARILLIN_PUNCH2;
			characterState->setState(R::CharacterState::ATTACK);
		}
		else if (random == 4){
			characterState->attack = R::Attack::KARILLIN_BEAT3;
			characterState->setState(R::CharacterState::ATTACK);
		}
		else if (random == 5){ characterState->setState(R::CharacterState::ATTACK); characterState->attack = R::Attack::KARILLIN_KICK2; }
		else if (random == 6) { characterState->setState(R::CharacterState::ATTACK); characterState->attack = R::Attack::KARILLIN_BEAT1; }
		else {
			srand(time(NULL));
			int rad = rand() % 9 + 1;
			if (rad % 3 == 1){
				if (characterState->direction == R::Direction::LEFT){
					characterState->direction = R::Direction::TOP_RIGHT;
				}
				if (characterState->direction == R::Direction::RIGHT){
					characterState->direction = R::Direction::TOP_LEFT;
				}
				if (characterPosition->x < 40)
					characterState->direction = R::Direction::TOP_RIGHT;
				if (characterPosition->x > R::Constants::MAX_SCREEN_WIDTH - 40)
					characterState->direction = R::Direction::TOP_LEFT;

				characterState->setState(R::CharacterState::JUMP);
				characterState->jump = true;
				return;
			}
		}

	}
}

void KarillinAI::moveAway() {
	float thinkingTime = .02f;
	if ((characterState->state == R::CharacterState::STAND
		|| characterState->state == R::CharacterState::DEFENSE)
		&& characterState->time_on_state >= thinkingTime) { // attack
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
			< (characterPosition->x + characterBound->x1);
		srand(time(NULL));
		int randomInt = rand() % 15;
		if (randomInt == 2 || targetState->trungdonlientiep % 5 == 3) {
			R::Direction direction = !targetOnTheLeft ? R::Direction::TOP_LEFT : R::Direction::TOP_RIGHT;

			if (characterPosition->x < 40)
				direction = R::Direction::TOP_RIGHT;
			if (characterPosition->x > R::Constants::MAX_SCREEN_WIDTH - 40)
				direction = R::Direction::TOP_LEFT;


			characterState->direction = direction;
			characterSkeleton->node->setScaleX(!targetOnTheLeft ? 1 : -1);
			characterState->setState(R::CharacterState::JUMP);
			characterState->jump = true;
			targetState->trungdonlientiep = 0;
		}
	}
}

void KarillinAI::processInRangeAttack() {
	moveAway();
	closeRangeAttack();
	if (characterState->state == R::CharacterState::WALK_LEFT
		|| characterState->state == R::CharacterState::WALK_RIGHT
		|| characterState->state == R::CharacterState::LEFT
		|| characterState->state == R::CharacterState::RIGHT) {
		characterState->setState(R::CharacterState::STAND);
	}
}

void KarillinAI::processOutRangeAttack() {

	bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
		< (characterPosition->x + characterBound->x1);
	bool coTheRaChuong = (characterState->state == R::CharacterState::WALK_LEFT	&& targetOnTheLeft) || (characterState->state == R::CharacterState::WALK_RIGHT	&& !targetOnTheLeft) || (characterState->state == R::CharacterState::STAND);
	float distance = targetPosition->x - characterPosition->x;

	if (characterState->state == R::CharacterState::STAND && characterInfo->hasManaForSkill(40)) {
		characterState->attack = R::Attack::KARILLIN_PUNCH1;
		characterState->setState(R::CharacterState::ATTACK);
		return;
	}

	if ((characterState->direction == R::Direction::TOP_LEFT || characterState->direction == R::Direction::TOP_RIGHT) && characterState->state == R::CharacterState::STAND) {
		characterState->direction = targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		characterState->setState(R::CharacterState::STAND);
		return;
	}

	if (targetOnTheLeft) {
		if (characterState->state == R::CharacterState::WALK_LEFT) {
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::WALK_LEFT);
		}
		if (characterState->state == R::CharacterState::LEFT){
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::WALK_LEFT);
		}
		if (characterState->state == R::CharacterState::STAND){
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::LEFT);
		}
	}
	else {
		if (characterState->state == R::CharacterState::WALK_RIGHT) {
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::WALK_RIGHT);
		}
		if (characterState->state == R::CharacterState::RIGHT){
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::WALK_RIGHT);
		}
		if (characterState->state == R::CharacterState::STAND){
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::RIGHT);
		}

	}

}
