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
				attackComponent->whoAttack = ((CharacterTypeComponent*)e.getComponent<CharacterTypeComponent>())->type;
				attackComponent->powerOfAttack = 10;
				attackComponent->manaOfAttack = 40;
				attackComponent->minX = position->x - R::Constants::WIDTH_SCREEN;
				attackComponent->maxX = position->x + R::Constants::WIDTH_SCREEN;
				attackComponent->minY = position->y - R::Constants::WIDTH_SCREEN;
				attackComponent->maxY = position->y + R::Constants::WIDTH_SCREEN;

				EntityUtils::getInstance()->createAttackEntity(e, attackComponent);
				state->setState(R::CharacterState::STAND);
				CCLOG("create goku puch1");
			}
		}

		WallSensorComponent* wallSensor = (WallSensorComponent*)e.getComponent<WallSensorComponent>();
		PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
		if (wallSensor->onFloor && state->state == R::CharacterState::JUMP){
			state->setState(R::CharacterState::STAND);
			return;
		}
	}
	state->time_on_state += world->getDelta();
}
