#include "EnemyFullAttackAI.h"

EnemyFullAttackAI::EnemyFullAttackAI() {
	this->isActive = true;
	mapInfo = new MapInfo();
}
EnemyFullAttackAI::~EnemyFullAttackAI(){}

void EnemyFullAttackAI::obsever(artemis::Entity &e) {
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

	if (characterState->time_on_state >= 1.5f) {
		characterState->setState(R::CharacterState::STAND);
		bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
			< (characterPosition->x + characterBound->x1);
		characterState->direction =
			targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		return;
	}

	if (targetState->state == R::CharacterState::JUMP){
		if (characterState->state != R::CharacterState::STAND || characterState->state != R::CharacterState::JUMP){
			srand(time(NULL));
			int number = rand() % 20 + 1;
			if (number > 2){
				characterState->setState(R::CharacterState::STAND);
			}
			else{
				if (characterState->direction == R::Direction::RIGHT){
					characterState->direction = R::Direction::TOP_LEFT;
					characterState->setState(R::CharacterState::JUMP);
				}
				else if (characterState->direction == R::Direction::LEFT){
					characterState->direction = R::Direction::TOP_RIGHT;
					characterState->setState(R::CharacterState::JUMP);
				}
				else{
					auto camera = Camera::getDefaultCamera();
					bool jumpLeft = characterPosition->x > camera->getPositionX();
					characterState->direction = jumpLeft ? R::Direction::TOP_LEFT : R::Direction::TOP_RIGHT;
					characterState->setState(R::CharacterState::JUMP);
				}
			}
		}
		return;
	}

	// trường hợp đánh thằng goku ngã
	if (characterState->state == R::CharacterState::DEFENSE
		&& (targetState->defense == R::Defense::TRUNG_DON && targetState->time_on_state < .4f)) {
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

	float rangeAttack = 10;
	float distance = abs(targetPosition->x - characterPosition->x);

	processAvoidAttack();
	if (distance > targetBound->getWidth() + rangeAttack)
		processOutRangeAttack();
	else
		processInRangeAttack();
}
void EnemyFullAttackAI::processOutRangeAttack() {
	bool targetOnTheLeft = (targetPosition->x + targetBound->x2) < (characterPosition->x + characterBound->x1);
	float distance = abs(targetPosition->x - characterPosition->x);
	if (characterInfo->hasManaForSkill(30) && distance < 70) {
		if (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_RIGHT){
			characterState->setState(R::CharacterState::STAND);
			return;
		}
		if (characterState->state == R::CharacterState::STAND && characterState->time_on_state >= .03f){
			srand(time(NULL));
			int r = rand() % 20;
			if (r % 3 == 0)	{
				characterState->attack = R::Attack::JACK_PUNCH_1;
				characterState->setState(R::CharacterState::ATTACK);
			}
		}
		return;
	}

	if (characterState->direction == R::Direction::TOP_LEFT
		&& characterState->state == R::CharacterState::STAND) {
		characterState->direction = R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		characterState->setState(R::CharacterState::STAND);
	}
	if (characterState->direction == R::Direction::TOP_RIGHT
		&& characterState->state == R::CharacterState::STAND) {
		characterState->direction = R::Direction::LEFT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		characterState->setState(R::CharacterState::STAND);
	}

	if (targetOnTheLeft) {
		if (characterState->state != R::CharacterState::WALK_LEFT&& characterState->state != R::CharacterState::LEFT) {
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::LEFT);
		}
		else {
			characterState->direction = R::Direction::LEFT;
			characterState->setState(R::CharacterState::WALK_LEFT);
		}
	}
	else {

		if (characterState->state != R::CharacterState::WALK_RIGHT
			&& characterState->state != R::CharacterState::RIGHT) {
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::RIGHT);
		}
		else {
			characterState->direction = R::Direction::RIGHT;
			characterState->setState(R::CharacterState::WALK_RIGHT);
		}
	}
}
void EnemyFullAttackAI::processAvoidAttack() {
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

bool EnemyFullAttackAI::canMakeDecision(){
	float thinkingTime = .02f;
	return ((characterState->state == R::CharacterState::STAND || characterState->state == R::CharacterState::DEFENSE) && characterState->time_on_state >= thinkingTime);
}
void EnemyFullAttackAI::processInRangeAttack() {
	avoidCloseAttack();
	//moveAway();
	closeRangeAttack();
	if (characterState->state == R::CharacterState::WALK_LEFT
		|| characterState->state == R::CharacterState::WALK_RIGHT
		|| characterState->state == R::CharacterState::LEFT
		|| characterState->state == R::CharacterState::RIGHT) {
		characterState->setState(R::CharacterState::STAND);
	}
}
void EnemyFullAttackAI::closeRangeAttack() {
	float thinkingTime = .02f;
	bool targetDangTrundon = targetState->state == R::CharacterState::DEFENSE && targetState->time_on_state >= .1f;
	bool targetOnTheLeft = (targetPosition->x) < (characterPosition->x);
	if (!targetDangTrundon && canMakeDecision()){
		srand(time(NULL));
		int random = rand() % 8 + 1;
		float distance = characterPosition->x - targetPosition->x;

		if (characterInfo->hasManaForSkill(30)) {

			if (random % 2 == 0){
				characterState->direction = targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
				characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
				characterState->attack = R::Attack::JACK_PUNCH_1;
				characterState->setState(R::CharacterState::ATTACK);
				return;
			}

			/*R::Direction jumpDirection = targetOnTheLeft ? R::Direction::TOP_RIGHT : R::Direction::TOP_LEFT;
			if (random < 3){
			if (characterPosition->x < 60)	jumpDirection = R::Direction::TOP_RIGHT;
			if (characterPosition->x>R::Constants::MAX_SCREEN_WIDTH - 80) jumpDirection = R::Direction::TOP_LEFT;
			characterState->direction = jumpDirection;
			characterState->setState(R::CharacterState::JUMP);
			return;
			}*/
		}
		characterState->direction = targetOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		characterSkeleton->node->setScaleX(targetOnTheLeft ? -1 : 1);
		if (random <= 2) 	characterState->attack = R::Attack::JACK_BEAT3;
		else if (random == 3) 	characterState->attack = R::Attack::JACK_KICK2;
		else if (random == 4) 	characterState->attack = R::Attack::JACK_KICK3;
		else if (random == 5) 	characterState->attack = R::Attack::JACK_PUNCH_AIR;

		else  	characterState->attack = R::Attack::JACK_PUNCH_2;

		characterState->setState(R::CharacterState::ATTACK);

	}
}
void EnemyFullAttackAI::moveAway() {
	if (!canMakeDecision()) return;
	bool targetOnTheLeft = (targetPosition->x + targetBound->x2)
		< (characterPosition->x + characterBound->x1);
	srand(time(NULL));
	int randomInt = rand() % 15;
	if (randomInt == 2 || targetState->trungdonlientiep % 5 == 3) {
		R::Direction direction = !targetOnTheLeft ?
			R::Direction::TOP_LEFT : R::Direction::TOP_RIGHT;
		characterState->direction = direction;
		characterSkeleton->node->setScaleX(!targetOnTheLeft ? 1 : -1);
		characterState->setState(R::CharacterState::JUMP);
		targetState->trungdonlientiep = 0;
	}
}
void EnemyFullAttackAI::avoidCloseAttack(){
	float distance = abs(targetPosition->x - characterPosition->x);
	srand(time(NULL));
	if (rand() % 5 == 2 && canMakeDecision() && targetState->state == R::CharacterState::ATTACK && targetState->time_on_state < .2f  && distance < 80){

		bool targetOnTheLeft = (targetPosition->x) < (characterPosition->x);
		R::Direction direction = !targetOnTheLeft ? R::Direction::TOP_LEFT : R::Direction::TOP_RIGHT;
		if (characterPosition->x < 60)	direction = R::Direction::TOP_RIGHT;
		if (characterPosition->x>R::Constants::MAX_SCREEN_WIDTH - 80) direction = R::Direction::TOP_LEFT;
		characterState->direction = direction;
		characterSkeleton->node->setScaleX(!targetOnTheLeft ? 1 : -1);
		characterState->setState(R::CharacterState::JUMP);
	}
}

