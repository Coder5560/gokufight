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
		if (state->characterBase != nullptr){
			state->characterBase->changeState(e);
		}
	}
	else{
		if (state->state == R::CharacterState::ATTACK && state->attack == R::Attack::GOKU_PUNCH1){
			if (state->time_on_state >= .5){
				PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
				AttackComponent* attackComponent = new AttackComponent();
				CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
				attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;


				attackComponent->minX = position->x - R::Constants::WIDTH_SCREEN;
				attackComponent->maxX = position->x + R::Constants::WIDTH_SCREEN;
				attackComponent->minY = position->y - R::Constants::WIDTH_SCREEN;
				attackComponent->maxY = position->y + R::Constants::WIDTH_SCREEN;
				attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
				attackComponent->isSpecialSkill = true;
				attackComponent->manaOfAttack = 40;

				EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
				state->setState(R::CharacterState::STAND);

			}
		}
		if (state->state == R::CharacterState::ATTACK && state->attack == R::Attack::KARILLIN_PUNCH1){
			if (state->time_on_state >= .5){
				PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
				AttackComponent* attackComponent = new AttackComponent();
				CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
				attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;


				attackComponent->minX = position->x - R::Constants::WIDTH_SCREEN;
				attackComponent->maxX = position->x + R::Constants::WIDTH_SCREEN;
				attackComponent->minY = position->y - R::Constants::HEIGHT_SCREEN;
				attackComponent->maxY = position->y + R::Constants::HEIGHT_SCREEN;
				attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
				attackComponent->isSpecialSkill = true;
				attackComponent->manaOfAttack = 40;

				EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
				state->setState(R::CharacterState::STAND);

			}
		}
		if (state->state == R::CharacterState::ATTACK && state->attack == R::Attack::TEGIAC_PUNCH3){
			if (state->time_on_state >= .5){
				PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
				AttackComponent* attackComponent = new AttackComponent();
				CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
				attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;		
				attackComponent->minX = position->x - R::Constants::WIDTH_SCREEN;
				attackComponent->maxX = position->x + R::Constants::WIDTH_SCREEN;
				attackComponent->minY = position->y - R::Constants::WIDTH_SCREEN;
				attackComponent->maxY = position->y + R::Constants::WIDTH_SCREEN;
				attackComponent->powerOfAttack = characterInfo->SPECIAL_SKILL_POWER;
				attackComponent->isSpecialSkill = true;
				attackComponent->manaOfAttack = 40;

				EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
				state->setState(R::CharacterState::STAND);
			}
		}


		WallSensorComponent* wallSensor = (WallSensorComponent*)e.getComponent<WallSensorComponent>();
		PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
		if (wallSensor->onFloor && (state->state == R::CharacterState::JUMP || state->state == R::CharacterState::BACK)){
			state->setState(R::CharacterState::STAND);
			return;
		}
	}
	if (state->characterBase != nullptr){
		state->characterBase->process();
	}

	state->time_on_state += world->getDelta();
}
