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
		WallSensorComponent* wallSensor = (WallSensorComponent*)e.getComponent<WallSensorComponent>();
		PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
		if (wallSensor->onFloor && state->state == R::CharacterState::JUMP){
			state->setState(R::CharacterState::STAND);		
		}
	}
	state->time_on_state += world->getDelta();
}
