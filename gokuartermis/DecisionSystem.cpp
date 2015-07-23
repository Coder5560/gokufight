#include "DecisionSystem.h"

DecisionSystem::DecisionSystem(){
	addComponentType<DecisionComponent>();
}

void DecisionSystem::initialize(){
	decisionMapper.init(*world);
}
void DecisionSystem::processEntity(artemis::Entity& e){
	DecisionComponent* decision = decisionMapper.get(e);
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();


	decision->thinkingTime += world->getDelta();
	if (decision->thinkingTime > decision->DECISION_TIME){
		decision->decisionBase->decision(e);
		decision->thinkingTime = 0;
	}
	else{
		decision->decisionBase->obsever(e);
	}
	

}
