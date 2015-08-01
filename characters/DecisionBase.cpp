#include "DecisionBase.h"
#include "systems/EntityUtils.h"

DecisionBase::DecisionBase(){}

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
	super.obsever(e);
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	if (stateComponent->state == R::CharacterState::STAND && stateComponent->time_on_state > 2){
		stateComponent->setState(R::CharacterState::START);
	}
}

GiranDecision::GiranDecision() :totalChoose(0){}
void GiranDecision::decision(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	DecisionComponent* decision = (DecisionComponent*)e.getComponent<DecisionComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();

	if (stateComponent->state == R::CharacterState::JUMP){
		return;
	}

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
	else if (distance < gokuBound->getWidth() / 2){
		//	R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		//	stateComponent->setState(gokuOnTheLeft ? R::CharacterState::WALK_LEFT : R::CharacterState::WALK_RIGHT);
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
		//	return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}
	if (stateComponent->state == R::CharacterState::JUMP){
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
	else if (distance < gokuBound->getWidth() - 30){
		stateComponent->setState(gokuOnTheLeft ? R::CharacterState::LEFT : R::CharacterState::RIGHT);
	}

	else{
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::RIGHT){
			//stateComponent->setState(R::CharacterState::STAND);
		}
	}
}



BearDecision::BearDecision() :totalChoose(0), isFarFromGoku(true){}
void BearDecision::decision(artemis::Entity &e){
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
		isFarFromGoku = true;
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
	else if (distance < gokuBound->getWidth() / 2){
		R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		stateComponent->setState(gokuOnTheLeft ? R::CharacterState::WALK_LEFT : R::CharacterState::WALK_RIGHT);
	}
	else{

		totalChoose += totalChoose % 3 + 1;
		R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		stateComponent->direction = direction;
		if ((totalChoose % 5) % 3 <= 1){
			if (isFarFromGoku){
				stateComponent->attack = R::Attack::BEAR_ATTACK2;
			}
			else{
				stateComponent->attack = R::Attack::BEAR_ATTACK1;
			}
		}
		else stateComponent->attack = R::Attack::BEAR_ATTACK2;
		stateComponent->setState(R::CharacterState::ATTACK);
		isFarFromGoku = false;
	}
}
void BearDecision::obsever(artemis::Entity &e){
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
	else if (distance < gokuBound->getWidth() / 2){
		stateComponent->setState(gokuOnTheLeft ? R::CharacterState::LEFT : R::CharacterState::RIGHT);
	}

	else{
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}

JackiechunDecision::JackiechunDecision() :totalChoose(0), isFarFromGoku(true){}
void JackiechunDecision::decision(artemis::Entity &e){
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
		stateComponent->direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		if (gokuOnTheLeft){
			(stateComponent->state != R::CharacterState::WALK_LEFT && stateComponent->state != R::CharacterState::LEFT) ?
				(stateComponent->setState(R::CharacterState::LEFT)) : (stateComponent->setState(R::CharacterState::WALK_LEFT));
		}
		else{
			(stateComponent->state != R::CharacterState::WALK_RIGHT && stateComponent->state != R::CharacterState::RIGHT) ?
				(stateComponent->setState(R::CharacterState::RIGHT)) : (stateComponent->setState(R::CharacterState::WALK_RIGHT));
		}
	}
	else{
		srand(time(NULL));

		/* generate secret number between 1 and 10: */
		int random = rand() % 6 + 1;

		R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		stateComponent->direction = direction;
		if (random == 1){ stateComponent->attack = R::Attack::JACK_PUNCH_2; }
		else if (random == 2){ stateComponent->attack = R::Attack::JACK_PUNCH_AIR; }
		else if (random == 3){ stateComponent->attack = R::Attack::JACK_KICK3; }
		else if (random == 4){ stateComponent->attack = R::Attack::JACK_BEAT3; }
		else if (random == 5){ stateComponent->attack = R::Attack::GOKU_KICK2; }
		else stateComponent->attack = R::Attack::JACK_PUNCH_2;
		stateComponent->setState(R::CharacterState::ATTACK);
		//isFarFromGoku = false;

	}
}
void JackiechunDecision::obsever(artemis::Entity &e){
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
	else if (distance < gokuBound->getWidth() / 2){
		stateComponent->setState(gokuOnTheLeft ? R::CharacterState::LEFT : R::CharacterState::RIGHT);
	}

	else{
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}

JackiechunDecision2::JackiechunDecision2() :totalChoose(0), isFarFromGoku(true), ai(nullptr){}
void JackiechunDecision2::decision(artemis::Entity &e){
	return;
}
void JackiechunDecision2::obsever(artemis::Entity &e){

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();

	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;

	stateComponent->direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
	if (distance > 120 && stateComponent->state == R::CharacterState::START){
		if (ai == nullptr){ ai = new JackiechunAIKhieuKhich(); ai->setActive(true); }
	}
	if (ai != nullptr) ai->process(e, goku);

}





RandomAI::RandomAI(){
	doiThuRaChieuTamXa = false;
	doiThuRaChieuGan = false;
	trongPhamViDonDanhCuaDoiThu = false;
	coTheRaChuongTamXa = false;
	nhieuMauHonDoiThu = false;
	phamViSatThuongCuaDoiThu = 0;
}

void RandomAI::decision(artemis::Entity &e){
}

void RandomAI::obsever(artemis::Entity &attacker){


	/*
	if (đối thủ ra chiêu) né
	if (trong phạm vi tấn công) ra đòn đánh
	if (ngoài phạm vi tấn công)
	{
	if (có chưởng tầm xa) đánh.
	if (không có chưởng tầm xa) di chuyển lại gần.
	}

	if (nhiều máu hơn đối thủ) đánh giáp lá cà.
	if (ít máu hơn đối thủ) không đánh xáp lá cà, né đòn, ra đòn tầm xa.
	*/




	artemis::Entity& defenser = world->getTagManager()->getEntity("goku");
	BoundComponent* defenseBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	PosComponent* defensePosition = (PosComponent*)defenser.getComponent<PosComponent>();
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	StateComponent* attackState = (StateComponent*)attacker.getComponent<StateComponent>();


	float distance = abs(defensePosition->x - attackPosition->x);
	bool defenseOnTheLeft = defensePosition->x < attackPosition->x;

}


AI::AI() :timeActive(0), active(false){}

bool AI::process(artemis::Entity &attacker, artemis::Entity &defenser){ return false; }
void AI::setActive(bool active){ this->active = active; }

JackiechunAIKhieuKhich::JackiechunAIKhieuKhich() : nextStep(false), alldone(false){}

bool JackiechunAIKhieuKhich::process(artemis::Entity &attacker, artemis::Entity &defenser){
	if (!active) return false;
	StateComponent* stateComponent = (StateComponent*)attacker.getComponent<StateComponent>();
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	if (timeActive == 0){

		if (stateComponent->state != R::CharacterState::STAND){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}

	if (timeActive >= 1 && timeActive <= 1.02){
		if (stateComponent->state != R::CharacterState::START){
			stateComponent->setState(R::CharacterState::START);
			Node* node = RenderLayer::getInstance()->createGameNode();
			node->setPosition(attackPosition->x, attackPosition->y + 40);

			Message* message = new Message(node);
			message->start("Ngon thì nhào vô");

		}
	}






	timeActive += 0.015;
	return false;
}