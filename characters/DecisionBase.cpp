#include "DecisionBase.h"
#include "systems/EntityUtils.h"

DecisionBase::DecisionBase()
{
}


DecisionBase::~DecisionBase()
{
}
void DecisionBase::setWorld(artemis::World* world){
	this->world = world;
}
void DecisionBase::obsever(artemis::Entity &e){
}
void DecisionBase::decision(artemis::Entity &e){
}

void GokuDecision::decision(artemis::Entity &e){

}
void GokuDecision::obsever(artemis::Entity &e){
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	if (stateComponent->state == R::CharacterState::STAND && stateComponent->time_on_state > 2){
		stateComponent->setState(R::CharacterState::START);
	}
}

void GiranDecision::decision(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	DecisionComponent* decision = (DecisionComponent*)e.getComponent<DecisionComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();



	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;

	if (distance > gokuBound->getWidth() + 20){
		if (gokuOnTheLeft){
			if (stateComponent->state != R::CharacterState::WALK_LEFT && stateComponent->state != R::CharacterState::LEFT){
				stateComponent->direction = R::Direction::LEFT;
				stateComponent->setState(R::CharacterState::LEFT);

			}
			else{
				stateComponent->direction = R::Direction::LEFT;
				stateComponent->setState(R::CharacterState::WALK_LEFT);
			}
		}
		else{
			if (stateComponent->state != R::CharacterState::WALK_RIGHT && stateComponent->state != R::CharacterState::RIGHT){
				stateComponent->direction = R::Direction::RIGHT;
				stateComponent->setState(R::CharacterState::RIGHT);
			}
			else{
				stateComponent->direction = R::Direction::RIGHT;
				stateComponent->setState(R::CharacterState::WALK_RIGHT);
			}
		}
	}
	
	else{
		srand(time(NULL));
		int rad = rand() % 10 + 1;
		R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		stateComponent->direction = direction;
		if (rad <= 3) stateComponent->attack = R::Attack::GIRAN_PUNCH1;
		else if (rad <= 6)	stateComponent->attack = R::Attack::GIRAN_PUNCH2;
		else stateComponent->attack = R::Attack::GIRAN_PUNCH3;
		stateComponent->setState(R::CharacterState::ATTACK);
	}
}
void GiranDecision::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();

	if (stateComponent->state == R::CharacterState::DEFENSE){
		return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}
	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;

	if (distance > gokuBound->getWidth()){
		if (gokuOnTheLeft){
			if (stateComponent->state != R::CharacterState::WALK_LEFT && stateComponent->state != R::CharacterState::LEFT){
				stateComponent->direction = R::Direction::LEFT;
				stateComponent->setState(R::CharacterState::LEFT);
			}
			else{
				stateComponent->direction = R::Direction::LEFT;
				stateComponent->setState(R::CharacterState::WALK_LEFT);
			}
		}
		else{
			if (stateComponent->state != R::CharacterState::WALK_RIGHT && stateComponent->state != R::CharacterState::RIGHT){
				stateComponent->direction = R::Direction::RIGHT;
				stateComponent->setState(R::CharacterState::RIGHT);
			}
			else{
				stateComponent->direction = R::Direction::RIGHT;
				stateComponent->setState(R::CharacterState::WALK_RIGHT);
			}
		}
	}
	else{
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}