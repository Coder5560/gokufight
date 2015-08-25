#include "JackiechunAI.h"



JackiechunAI::JackiechunAI() :totalChoose(0), isFarFromGoku(true){ this->isActive = true; }
void JackiechunAI::decision(artemis::Entity &e){
	return;
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
	bool onRangeAttack = distance > gokuBound->getWidth();

	if (!onRangeAttack){
		isFarFromGoku = true;
		move(stateComponent, gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT);
	}
	else{
		// attack

		srand(time(NULL));

		/* generate secret number between 1 and 10: */
		int random = rand() % 6 + 1;

		R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		stateComponent->direction = direction;
		if (random == 1){ stateComponent->attack = R::Attack::JACK_PUNCH_2; }
		else if (random == 2){ stateComponent->attack = R::Attack::JACK_PUNCH_AIR; }
		else if (random == 3){ stateComponent->attack = R::Attack::JACK_KICK3; }
		else if (random == 4){ stateComponent->attack = R::Attack::JACK_BEAT3; }
		else if (random == 5){ stateComponent->attack = R::Attack::JACK_KICK2; }
		else stateComponent->attack = R::Attack::JACK_PUNCH_2;
		stateComponent->setState(R::CharacterState::ATTACK);
		//isFarFromGoku = false;

	}
}
void JackiechunAI::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();

	if (stateComponent->state == R::CharacterState::DEFENSE){
		//	return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}


	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;

	if (distance > gokuBound->getWidth() + 10){
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
	else if (distance < gokuBound->getWidth() / 3){
		stateComponent->setState(gokuOnTheLeft ? R::CharacterState::LEFT : R::CharacterState::RIGHT);
	}
	else{
		if (stateComponent->state == R::CharacterState::STAND){ // attack
			srand(time(NULL));
			/* generate secret number between 1 and 10: */
			int random = rand() % 6 + 1;
			R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
			stateComponent->direction = direction;
			if (random == 1){ stateComponent->attack = R::Attack::JACK_PUNCH_2; }
			else if (random == 2){ stateComponent->attack = R::Attack::JACK_PUNCH_AIR; }
			else if (random == 3){ stateComponent->attack = R::Attack::JACK_KICK3; }
			else if (random == 4){ stateComponent->attack = R::Attack::JACK_BEAT3; }
			else if (random == 5){ stateComponent->attack = R::Attack::JACK_KICK2; }
			else stateComponent->attack = R::Attack::JACK_PUNCH_2;
			stateComponent->setState(R::CharacterState::ATTACK);
		}
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}