#include "DecisionSystem.h"

DecisionSystem::DecisionSystem(){
	addComponentType<DecisionComponent>();
}

void DecisionSystem::initialize(){
	decisionMapper.init(*world);
}
void DecisionSystem::processEntity(artemis::Entity& e){
	artemis::Entity &state = world->getTagManager()->getEntity("gameState");
	GameStateComponent* gameState = (GameStateComponent*)state.getComponent<GameStateComponent>();
	if (gameState->gameState != R::GameState::FIGHTING){return; }
	
	DecisionComponent* decision = decisionMapper.get(e);
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	if (stateComponent->state == R::CharacterState::DIE) return;
	decision->thinkingTime += world->getDelta();
	if (decision->thinkingTime > decision->DECISION_TIME){
		decision->decisionBase->decision(e);
		decision->thinkingTime = 0;
	}
	else{
		decision->decisionBase->obsever(e);
	}
}
