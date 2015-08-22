#include "EnemyNormalAI.h"

EnemyNormalAI::EnemyNormalAI() {
	this->isActive = true;
	mapInfo = new MapInfo();
}
EnemyNormalAI::~EnemyNormalAI() {
}

void EnemyNormalAI::obsever(artemis::Entity &e) {
	artemis::Entity &gameState = world->getTagManager()->getEntity("gameState");
	GameStateComponent* gameStateComponent =
			(GameStateComponent*) gameState.getComponent<GameStateComponent>();
	if (gameStateComponent->gameState == R::GameState::LOSE) {
		if (characterState->state != R::CharacterState::STAND) {
			characterState->setState(R::CharacterState::STAND);
		}
		return;
	}

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	targetBound = (BoundComponent*) goku.getComponent<BoundComponent>();
	characterBound = (BoundComponent*) e.getComponent<BoundComponent>();
	targetPosition = (PosComponent*) goku.getComponent<PosComponent>();
	characterPosition = (PosComponent*) e.getComponent<PosComponent>();
	targetState = (StateComponent*) goku.getComponent<StateComponent>();
	characterState = (StateComponent*) e.getComponent<StateComponent>();
	targetInfo = (CharacterInfoComponent*) goku.getComponent<
			CharacterInfoComponent>();
	characterInfo = (CharacterInfoComponent*) e.getComponent<
			CharacterInfoComponent>();
	targetPhysic = (PhysicComponent*) goku.getComponent<PhysicComponent>();
	characterPhysic = (PhysicComponent*) e.getComponent<PhysicComponent>();
	targetSkeleton =
			(SkeletonComponent*) goku.getComponent<SkeletonComponent>();
	characterSkeleton =
			(SkeletonComponent*) e.getComponent<SkeletonComponent>();

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

	float rangeAttack = 20;
	float distance = abs(targetPosition->x - characterPosition->x);

	processAvoidAttack();

	if (distance > targetBound->getWidth() + rangeAttack)
		processOutRangeAttack();
	else
		processInRangeAttack();
}
void EnemyNormalAI::processAvoidAttack() {
	if (world->getTagManager()->isSubscribed("gokupunch")) {
		artemis::Entity &gokuPunch = world->getTagManager()->getEntity(
				"gokupunch");
		SkillComponent* skill = (SkillComponent*) gokuPunch.getComponent<
				SkillComponent>();
		Rect characterRect = Rect(characterPosition->x + characterBound->x1,
				characterPosition->y + characterBound->y1,
				characterBound->getWidth(), characterBound->getHeight());
		bool inRangeSkillAttackLeft = skill->kamekameha->direction > 0
				&& (characterRect.getMinX()
						- skill->kamekameha->node->getPositionX()) > 30;
		bool inRangeSkillAttackRight = skill->kamekameha->direction < 0
				&& (skill->kamekameha->node->getPositionX()
						- characterRect.getMaxX()) > 30;
		if (inRangeSkillAttackLeft || inRangeSkillAttackRight) {
			if (characterState->state != R::CharacterState::JUMP
					&& characterState->state != R::CharacterState::DEFENSE) {
				characterState->setState(R::CharacterState::JUMP);
				return;
			}
		}
	}
}

void EnemyNormalAI::closeRangeAttack() {
	float thinkingTime = .02f;
	if ((characterState->state == R::CharacterState::STAND
			|| characterState->state == R::CharacterState::DEFENSE)
			&& characterState->time_on_state >= thinkingTime) { // attack
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
				< (characterPosition->x + characterBound->x1);
		float distance = characterPosition->x - targetPosition->x;

		srand(time(NULL));
		int random = rand() % 5 + 1;
		R::Direction direction =
				targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		characterState->direction = direction;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);

		if (characterInfo->hasManaForSkill(40)) {
			// kich 3 vs punch AIR
			if (distance > targetBound->getWidth()) {
				if (random <= 3)
					characterState->attack = R::Attack::TEGIAC_PUNCH3;
				else
					characterState->attack = R::Attack::TEGIAC_PUNCH_AIR;
			} else {
				if (random <= 2) {
					characterState->attack = R::Attack::TEGIAC_PUNCH1;
				} else if (random == 3) {
					characterState->attack = R::Attack::TEGIAC_KICK2;
				} else if (random == 4) {
					characterState->attack = R::Attack::TEGIAC_BEAT3;
				} else
					characterState->attack = R::Attack::TEGIAC_PUNCH2;
			}
		} else {
			if (random <= 2) {
				characterState->attack = R::Attack::TEGIAC_PUNCH1;
			} else if (random == 3) {
				characterState->attack = R::Attack::TEGIAC_KICK2;
			} else if (random == 4) {
				characterState->attack = R::Attack::TEGIAC_BEAT3;
			} else
				characterState->attack = R::Attack::TEGIAC_PUNCH2;
		}

		characterState->setState(R::CharacterState::ATTACK);
	}
}

void EnemyNormalAI::moveAway() {
	float thinkingTime = .02f;
	if ((characterState->state == R::CharacterState::STAND
			|| characterState->state == R::CharacterState::DEFENSE)
			&& characterState->time_on_state >= thinkingTime) { // attack
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
				< (characterPosition->x + characterBound->x1);
		srand(time(NULL));
		int randomInt = rand() % 15;
		if (randomInt == 2 || targetState->trungdonlientiep % 5 == 3) {
			R::Direction direction =
					!targetOnTheLeft ?
							R::Direction::TOP_LEFT : R::Direction::TOP_RIGHT;
			characterState->direction = direction;
			characterSkeleton->node->setScaleX(!targetOnTheLeft ? 1 : -1);
			characterState->setState(R::CharacterState::JUMP);
			targetState->trungdonlientiep = 0;
		}
	}
}

void EnemyNormalAI::processInRangeAttack() {
	moveAway();
	closeRangeAttack();
	if (characterState->state == R::CharacterState::WALK_LEFT
			|| characterState->state == R::CharacterState::WALK_RIGHT
			|| characterState->state == R::CharacterState::LEFT
			|| characterState->state == R::CharacterState::RIGHT) {
		characterState->setState(R::CharacterState::STAND);
	}
}

void EnemyNormalAI::processOutRangeAttack() {
	bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
			< (characterPosition->x + characterBound->x1);
	bool coTheRaChuong = (characterState->state == R::CharacterState::WALK_LEFT
			&& targetOnTheLeft)
			|| (characterState->state == R::CharacterState::WALK_RIGHT
					&& !targetOnTheLeft)
			|| (characterState->state == R::CharacterState::STAND);
	srand(time(NULL));
	int random = rand() % 3 + 1;
	float distance = targetPosition->x - characterPosition->x;

	if (coTheRaChuong && characterInfo->hasManaForSkill(40)) {
		if (random == 2 && characterState->totalAttack % 5 == 1) {
			characterState->attack = R::Attack::TEGIAC_KICK3;
			characterState->setState(R::CharacterState::ATTACK);
			return;
		}
	}

	if (characterState->direction == R::Direction::TOP_LEFT
			&& characterState->state == R::CharacterState::STAND) {
		characterState->direction = R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		characterState->setState(R::CharacterState::STAND);

	}

	if (targetOnTheLeft) {
		if (characterState->state != R::CharacterState::WALK_LEFT
				&& characterState->state != R::CharacterState::LEFT) {
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::LEFT);
		} else {
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::WALK_LEFT);
		}
	} else {
		if (characterState->state != R::CharacterState::WALK_RIGHT
				&& characterState->state != R::CharacterState::RIGHT) {
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::RIGHT);
		} else {
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::WALK_RIGHT);
		}
	}

}
