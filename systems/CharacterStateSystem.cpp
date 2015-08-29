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
		if (state->state == R::CharacterState::ATTACK && state->attack == R::Attack::TEGIAC_PUNCH3){
			if (state->time_on_state >= .5){
				if (R::Constants::soundEnable) {
					CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::SKILL_3, false, 1, 0, 1);
				}
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

		artemis::Entity& gameState = world->getTagManager()->getEntity("gameState");
		GameStateComponent* gameStateComponent = (GameStateComponent*) gameState.getComponent<GameStateComponent>();

		if(gameStateComponent ->gameState == R::GameState::WIN || gameStateComponent->gameState == R::GameState::LOSE){
			return;
		}

		WallSensorComponent* wallSensor = (WallSensorComponent*)e.getComponent<WallSensorComponent>();
		PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
		CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
		if (wallSensor->onFloor && (state->state == R::CharacterState::JUMP || state->state == R::CharacterState::BACK)){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::LANDING, false, 1, 0, 1);
			}
			state->setState(R::CharacterState::STAND);
			
			return;
		}
		if (characterInfo->isMainCharacter &&  state->jump && physic->vy == 0 && state->state != R::CharacterState::STAND){
			state->setState(R::CharacterState::STAND);
			state->jump = false;
			return;
		}

	}
	if (state->characterBase != nullptr){
		state->characterBase->process();
	}
	state->time_on_state += world->getDelta();
}
