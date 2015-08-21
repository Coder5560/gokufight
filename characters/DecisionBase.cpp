#include "DecisionBase.h"
#include "systems/EntityUtils.h"

DecisionBase::DecisionBase(){
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
void DecisionBase::move(StateComponent* stateComponent, R::Direction direction){
	if (direction == R::Direction::LEFT || direction == R::Direction::RIGHT){
		stateComponent->direction = direction;
		if (direction == R::Direction::LEFT){
			if (stateComponent->state != R::CharacterState::WALK_LEFT && stateComponent->state != R::CharacterState::LEFT)
				stateComponent->setState(R::CharacterState::LEFT);
			else if (stateComponent->state == R::CharacterState::LEFT) stateComponent->setState(R::CharacterState::WALK_LEFT);

		}
		else{
			if (stateComponent->state != R::CharacterState::WALK_RIGHT && stateComponent->state != R::CharacterState::RIGHT)
				stateComponent->setState(R::CharacterState::RIGHT);
			else if (stateComponent->state == R::CharacterState::RIGHT) stateComponent->setState(R::CharacterState::WALK_RIGHT);
		}
	}
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

GiranDecision::GiranDecision() :totalChoose(0){ this->isActive = true; }
void GiranDecision::decision(artemis::Entity &e){
	return;
}
void GiranDecision::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PhysicComponent* gokuPhysic = (PhysicComponent*)goku.getComponent<PhysicComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();

	BoundComponent* characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* characterState = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	if (characterState->state == R::CharacterState::DEFENSE){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){
		return;
	}

	float distance = characterPosition->x - gokuPosition->x;
	float phamViTanCong = 20;


	bool isGokuDungyen = gokuState->state == R::CharacterState::STAND;
	bool isGokuDichuyenSangTrai = (gokuState->state == R::CharacterState::WALK_LEFT) || (gokuState->state == R::CharacterState::LEFT);
	bool isGokuDichuyenSangPhai = (gokuState->state == R::CharacterState::WALK_RIGHT) || (gokuState->state == R::CharacterState::RIGHT);
	

	bool isCharacterDangDichuyen = (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT);
	bool isDichuyenSangTrai = (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::LEFT);
	bool isDichuyenSangPhai = (characterState->state == R::CharacterState::WALK_RIGHT) || (characterState->state == R::CharacterState::RIGHT);

	bool isGokuDungBenPhaiPhamVitancong = (gokuPosition->x + gokuBound->x1) > (characterPosition->x + characterBound->x2 + phamViTanCong);
	bool isGokuDungBenTraiPhamVitancong = (gokuPosition->x + gokuBound->x2) < (characterPosition->x + characterBound->x1 - phamViTanCong);
	bool isGokuDungTrongPhamVitancong = (!isGokuDungBenPhaiPhamVitancong && !isGokuDungBenTraiPhamVitancong);

	bool isGokuDungGanBenTrai = distance>0 && distance < characterBound->getWidth() / 2;
	bool isGokuDungGanBenPhai = distance <0 && distance > -characterBound->getWidth() / 2;
	bool coTheRadon = (characterState->state == R::CharacterState::STAND) || (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::WALK_RIGHT || (characterState->state == R::CharacterState::LEFT) || (characterState->state == R::CharacterState::RIGHT));


	if (isGokuDungTrongPhamVitancong){
		// nằm trong phạm vi tấn công
		if (!isGokuDungGanBenTrai && !isGokuDungGanBenPhai){
			// Có thể tấn công
			if (coTheRadon && characterState->time_on_state>= .3f){
				if (distance > 0) {
					characterState->direction = R::Direction::LEFT;
					characterSkeleton->node->setScaleX(-1);
				}
				else {
					characterState->direction = R::Direction::RIGHT;
					characterSkeleton->node->setScaleX(1);
				}
				srand(time(NULL));
				int random = rand() % 3 + 1;
				if (random == 1)characterState->attack = R::Attack::GIRAN_PUNCH1;
				else if (random == 2)characterState->attack = R::Attack::GIRAN_PUNCH2;
				else characterState->attack = R::Attack::GIRAN_PUNCH3;
				characterState->setState(R::CharacterState::ATTACK);
			}
		}
		else{
			// không thể tấn công
			if (isGokuDichuyenSangPhai) characterPosition->x -= 2;
			else if (isGokuDichuyenSangTrai) characterPosition->x += 2;
			else if (isGokuDungyen){
				if (isGokuDungGanBenPhai) characterPosition->x -= 2;
				if (isGokuDungGanBenTrai) characterPosition->x += 2;
			}
		}
	}
	else {

		// ở ngoài phạm vi tấn công
		if (isGokuDungyen){
			// nếu goku đứng yên thì di chuyển lại gần.
			if (isGokuDungBenPhaiPhamVitancong)	{
				characterState->direction = R::Direction::RIGHT;
				if (characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_RIGHT)
					characterState->setState(R::CharacterState::WALK_RIGHT);
				else
					characterState->setState(R::CharacterState::RIGHT);
			}
			if (isGokuDungBenTraiPhamVitancong)	{
				characterState->direction = R::Direction::LEFT;
				if (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::WALK_LEFT)
					characterState->setState(R::CharacterState::WALK_LEFT);
				else
					characterState->setState(R::CharacterState::LEFT);

			}
			return;
		}
	}
}



BearDecision::BearDecision() :totalChoose(0), isFarFromGoku(true){ this->isActive = true; }
void BearDecision::decision(artemis::Entity &e){
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
	BoundComponent* gokuBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();

	BoundComponent* characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* characterState = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	if (characterState->state == R::CharacterState::DEFENSE){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){
		return;
	}

	float distance = characterPosition->x - gokuPosition->x;
	float phamViTanCong = 20;


	bool isGokuDungyen = gokuState->state == R::CharacterState::STAND;
	bool isGokuDichuyenSangTrai = (gokuState->state == R::CharacterState::WALK_LEFT) || (gokuState->state == R::CharacterState::LEFT);
	bool isGokuDichuyenSangPhai = (gokuState->state == R::CharacterState::WALK_RIGHT) || (gokuState->state == R::CharacterState::RIGHT);

	bool isCharacterDangDichuyen = (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT);
	bool isDichuyenSangTrai = (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::LEFT);
	bool isDichuyenSangPhai = (characterState->state == R::CharacterState::WALK_RIGHT) || (characterState->state == R::CharacterState::RIGHT);

	bool isGokuDungBenPhaiPhamVitancong = (gokuPosition->x + gokuBound->x1) > (characterPosition->x + characterBound->x2 + phamViTanCong);
	bool isGokuDungBenTraiPhamVitancong = (gokuPosition->x + gokuBound->x2) < (characterPosition->x + characterBound->x1 - phamViTanCong);
	bool isGokuDungTrongPhamVitancong = (!isGokuDungBenPhaiPhamVitancong && !isGokuDungBenTraiPhamVitancong);

	bool isGokuDungGanBenTrai = distance>0 && distance < characterBound->getWidth() / 2;
	bool isGokuDungGanBenPhai = distance <0 && distance > -characterBound->getWidth() / 2;
	bool coTheRadon = (characterState->state == R::CharacterState::STAND) || (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::WALK_RIGHT || (characterState->state == R::CharacterState::LEFT) || (characterState->state == R::CharacterState::RIGHT));


	if (isGokuDungTrongPhamVitancong){
		// nằm trong phạm vi tấn công
		if (!isGokuDungGanBenTrai && !isGokuDungGanBenPhai){
			// Có thể tấn công
			if (coTheRadon){
				if (distance > 0) {
					characterState->direction = R::Direction::LEFT;
					characterSkeleton->node->setScaleX(-1);
				}
				else {
					characterState->direction = R::Direction::RIGHT;
					characterSkeleton->node->setScaleX(1);
				}
				srand(time(NULL));
				int random = rand() % 3 + 1;
				if (random <= 2)characterState->attack = R::Attack::BEAR_ATTACK1;
				else characterState->attack = R::Attack::BEAR_ATTACK2;
				characterState->setState(R::CharacterState::ATTACK);
			}
		}
		else{
			// không thể tấn công
			if (isGokuDichuyenSangPhai) characterPosition->x -= 2;
			else if (isGokuDichuyenSangTrai) characterPosition->x += 2;
			else if (isGokuDungyen){
				if (isGokuDungGanBenPhai) characterPosition->x -= 2;
				if (isGokuDungGanBenTrai) characterPosition->x += 2;
			}
		}
	}
	else {

		// ở ngoài phạm vi tấn công
		if (isGokuDungyen){
			// nếu goku đứng yên thì di chuyển lại gần.
			if (isGokuDungBenPhaiPhamVitancong)	{
				characterState->direction = R::Direction::RIGHT; characterState->setState(R::CharacterState::RIGHT);
			}
			if (isGokuDungBenTraiPhamVitancong)	{
				characterState->direction = R::Direction::LEFT; characterState->setState(R::CharacterState::LEFT);
			}
			return;
		}
	}

}





JackiechunDecision::JackiechunDecision() :totalChoose(0), isFarFromGoku(true){ this->isActive = true; }
void JackiechunDecision::decision(artemis::Entity &e){
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

TegiacDecision::TegiacDecision() :currentStep(0), doneSpeicalSkill(false), doSpecificSkill(false), isFarFromGoku(true), timeInRageAttack(0){ this->isActive = true; }
void TegiacDecision::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();


	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;
	bool itmauhon = (gokuInfo->blood - characterInfo->blood) > 10;
	bool cotherachuongtamxa = characterInfo->hasManaForSkill(40);
	bool trungdon2lanlientiep = stateComponent->trungdonlientiep >= 2;



	// trường hợp đánh thằng goku ngã
	if (gokuState->state == R::CharacterState::DEFENSE && (gokuState->defense == R::Defense::TRUNG_DON_NGA || gokuState->state == R::CharacterState::STAND_UP)){
		// đứng khiên khích
		return;
	}

	// trường hợp đang dính đòn thì k làm gì
	if (stateComponent->state == R::CharacterState::DEFENSE && (stateComponent->defense == R::Defense::TRUNG_DON_NGA || stateComponent->state == R::CharacterState::STAND_UP)){
		return;
	}
	if (stateComponent->state == R::CharacterState::DEFENSE && stateComponent->defense == R::Defense::TRUNG_DON){
		//	if (stateComponent->state != R::CharacterState::STAND) stateComponent->setState(R::CharacterState::STAND);
		return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}




	if (distance > gokuBound->getWidth() + 20){
		if (((stateComponent->state == R::CharacterState::WALK_LEFT && gokuOnTheLeft) || (stateComponent->state == R::CharacterState::WALK_RIGHT && !gokuOnTheLeft)) && stateComponent->doneAction){
			if (distance > (gokuBound->getWidth() + 60) && distance < (gokuBound->getWidth() + 70)){
				srand(time(NULL));
				int random = rand() % 6 + 1;
				if (random < 4 && characterInfo->hasManaForSkill(40)){
					stateComponent->doneAction = false;
					stateComponent->attack = R::Attack::TEGIAC_KICK3;
					stateComponent->setState(R::CharacterState::ATTACK);
					return;
				}
			}
		}


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
		float thinkingTime = .02f;
		if ((stateComponent->state == R::CharacterState::STAND || stateComponent->state == R::CharacterState::DEFENSE) && stateComponent->time_on_state >= thinkingTime){ // attack
			solanTrungDon = 0;
			srand(time(NULL));
			int random = rand() % 5 + 1;
			R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
			stateComponent->direction = direction;

			if (characterInfo->hasManaForSkill(40)){
				// kich 3 vs punch AIR
				if (distance > gokuBound->getWidth()){
					if (random <= 3) stateComponent->attack = R::Attack::TEGIAC_PUNCH3;
					else stateComponent->attack = R::Attack::TEGIAC_PUNCH_AIR;
				}
				else{
					if (random <= 2){ stateComponent->attack = R::Attack::TEGIAC_PUNCH1; }
					else if (random == 3){ stateComponent->attack = R::Attack::TEGIAC_KICK2; }
					else if (random == 4){ stateComponent->attack = R::Attack::TEGIAC_BEAT3; }
					else stateComponent->attack = R::Attack::TEGIAC_PUNCH2;
				}
			}
			else{
				if (random <= 2){ stateComponent->attack = R::Attack::TEGIAC_PUNCH1; }
				else if (random == 3){ stateComponent->attack = R::Attack::TEGIAC_KICK2; }
				else if (random == 4){ stateComponent->attack = R::Attack::TEGIAC_BEAT3; }
				else stateComponent->attack = R::Attack::TEGIAC_PUNCH2;
			}


			stateComponent->setState(R::CharacterState::ATTACK);
		}
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}



RuaDecision::RuaDecision() { this->isActive = true; }
void RuaDecision::decision(artemis::Entity &e){
	return;
}
void RuaDecision::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();


	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;


	// trường hợp đánh thằng goku ngã
	if (gokuState->state == R::CharacterState::DEFENSE && (gokuState->defense == R::Defense::TRUNG_DON_NGA || gokuState->state == R::CharacterState::STAND_UP)){
		// đứng khiên khích
		return;
	}

	// trường hợp đang dính đòn thì k làm gì
	if (stateComponent->state == R::CharacterState::DEFENSE && (stateComponent->defense == R::Defense::TRUNG_DON_NGA || stateComponent->state == R::CharacterState::STAND_UP)){
		return;
	}
	if (stateComponent->state == R::CharacterState::DEFENSE && stateComponent->defense == R::Defense::TRUNG_DON){
		//if (stateComponent->state != R::CharacterState::STAND) stateComponent->setState(R::CharacterState::STAND);
		return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}
	stateComponent->direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
	if (distance > gokuBound->getWidth() + 10){
		if (((stateComponent->state == R::CharacterState::WALK_LEFT && gokuOnTheLeft) || (stateComponent->state == R::CharacterState::WALK_RIGHT && !gokuOnTheLeft)) && stateComponent->doneAction){
			if (distance > (gokuBound->getWidth() + 30) && distance < (gokuBound->getWidth() + 40)){
				srand(time(NULL));
				int random = rand() % 6 + 1;
				if (random < 4){
					stateComponent->doneAction = false;
					stateComponent->attack = R::Attack::RUA_ATTACK4;
					stateComponent->setState(R::CharacterState::ATTACK);
					return;
				}
			}
		}


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
	else if (distance < gokuBound->getWidth() / 4){
		if (gokuState->state == R::CharacterState::STAND){
			stateComponent->setState(gokuOnTheLeft ? R::CharacterState::RIGHT : R::CharacterState::LEFT);
			characterPosition->x += gokuOnTheLeft ? 20 : -20;
		}
		else{
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
	else{
		float thinkingTime = .02f;
		if ((stateComponent->state == R::CharacterState::STAND || stateComponent->state == R::CharacterState::DEFENSE) && stateComponent->time_on_state >= thinkingTime){ // attack
			srand(time(NULL));
			int random = rand() % 5 + 1;


			if (characterInfo->hasManaForSkill(40)){
				// kich 3 vs punch AIR
				if (distance > gokuBound->getWidth()){
					if (random <= 3) stateComponent->attack = R::Attack::RUA_ATTACK3;
					else stateComponent->attack = R::Attack::RUA_ATTACK5;
				}
				else{
					if (random <= 2){ stateComponent->attack = R::Attack::RUA_ATTACK1; }
					else if (random <= 4){ stateComponent->attack = R::Attack::RUA_ATTACK4; }
					else stateComponent->attack = R::Attack::RUA_ATTACK2;
				}
			}
			else{
				if (random <= 2){ stateComponent->attack = R::Attack::RUA_ATTACK1; }
				else if (random <= 4){ stateComponent->attack = R::Attack::RUA_ATTACK4; }
				else stateComponent->attack = R::Attack::RUA_ATTACK2;
			}
			stateComponent->setState(R::CharacterState::ATTACK);
		}
		if (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::LEFT || stateComponent->state == R::CharacterState::RIGHT){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}



CamapDecision::CamapDecision() { this->isActive = true; canAvoidGokuAttack = true; totalAvoid = 0; }
void CamapDecision::decision(artemis::Entity &e){
	return;
}
void CamapDecision::obsever(artemis::Entity &e){

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();

	BoundComponent* characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* characterState = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	if (characterState->state == R::CharacterState::DEFENSE){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){
		return;
	}

	float distance = characterPosition->x - gokuPosition->x;
	float phamViTanCong = 20;


	bool isGokuDungyen = gokuState->state == R::CharacterState::STAND;
	bool isGokuDichuyenSangTrai = (gokuState->state == R::CharacterState::WALK_LEFT) || (gokuState->state == R::CharacterState::LEFT);
	bool isGokuDichuyenSangPhai = (gokuState->state == R::CharacterState::WALK_RIGHT) || (gokuState->state == R::CharacterState::RIGHT);

	bool isCharacterDangDichuyen = (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT);
	bool isDichuyenSangTrai = (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::LEFT);
	bool isDichuyenSangPhai = (characterState->state == R::CharacterState::WALK_RIGHT) || (characterState->state == R::CharacterState::RIGHT);

	bool isGokuDungBenPhaiPhamVitancong = (gokuPosition->x + gokuBound->x1) > (characterPosition->x + characterBound->x2 + phamViTanCong);
	bool isGokuDungBenTraiPhamVitancong = (gokuPosition->x + gokuBound->x2) < (characterPosition->x + characterBound->x1 - phamViTanCong);
	bool isGokuDungTrongPhamVitancong = (!isGokuDungBenPhaiPhamVitancong && !isGokuDungBenTraiPhamVitancong);

	bool isGokuDungGanBenTrai = distance>0 && distance < characterBound->getWidth() / 2;
	bool isGokuDungGanBenPhai = distance <0 && distance > -characterBound->getWidth() / 2;
	bool coTheRadon = (characterState->state == R::CharacterState::STAND) || (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::WALK_RIGHT || (characterState->state == R::CharacterState::LEFT) || (characterState->state == R::CharacterState::RIGHT));


	if (isGokuDungTrongPhamVitancong){
		// nằm trong phạm vi tấn công
		if (!isGokuDungGanBenTrai && !isGokuDungGanBenPhai){
			// Có thể tấn công
			if (coTheRadon){
				if (distance > 0) {
					characterState->direction = R::Direction::LEFT;
					characterSkeleton->node->setScaleX(-1);
				}
				else {
					characterState->direction = R::Direction::RIGHT;
					characterSkeleton->node->setScaleX(1);
				}
				srand(time(NULL));
				int random = rand() % 5 + 1;
				if (random == 1){ characterState->attack = R::Attack::CAMAP_KICK2; }
				else if (random == 2){ characterState->attack = R::Attack::CAMAP_PUNCH1; }
				else if (random == 3){ characterState->attack = R::Attack::CAMAP_PUNCH2; }
				else if (random == 4){ characterState->attack = R::Attack::CAMAP_PUNCH3; }
				else { characterState->attack = R::Attack::CAMAP_PUNCH2; }
				characterState->setState(R::CharacterState::ATTACK);
			}
		}
		else{
			// không thể tấn công
			if (isGokuDichuyenSangPhai) characterPosition->x -= 2;
			else if (isGokuDichuyenSangTrai) characterPosition->x += 2;
			else if (isGokuDungyen){
				if (isGokuDungGanBenPhai) characterPosition->x -= 2;
				if (isGokuDungGanBenTrai) characterPosition->x += 2;
			}
		}
	}
	else {
		// ở ngoài phạm vi tấn công

		//tranh don tam xa
		if (world->getTagManager()->isSubscribed("gokupunch")){
			if (canAvoidGokuAttack){
				totalAvoid++;
				canAvoidGokuAttack = false;
			}
			if (totalAvoid % 3 == 1){
				return;
			}

			artemis::Entity &gokuPunch = world->getTagManager()->getEntity("gokupunch");
			SkillComponent* skill = (SkillComponent*)gokuPunch.getComponent<SkillComponent>();
			Rect characterRect = Rect(characterPosition->x + characterBound->x1, characterPosition->y + characterBound->y1, characterBound->getWidth(), characterBound->getHeight());

			if (skill->kamekameha->direction > 0 && ((characterRect.getMinX() - skill->kamekameha->node->getPositionX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					totalAvoid++;
					return;
				}
			}
			if (skill->kamekameha->direction <0 && ((skill->kamekameha->node->getPositionX() - characterRect.getMaxX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					totalAvoid++;
					return;
				}
			}
		}
		else{
			canAvoidGokuAttack = true;
		}
		
		if (isGokuDungyen){
			// có thể ra chưởng tầm xa thì chiến luôn :
			bool inRangleAttack = abs(distance) < gokuBound->getWidth() + 2 * phamViTanCong;
			if (inRangleAttack && characterState->state != R::CharacterState::JUMP && !isCharacterDangDichuyen &&  characterInfo->hasManaForSkill(40)){
				characterState->doneAction = false;
				srand(time(NULL));
				int rad = rand() % 3 + 1;
				if (rad <= 2){
					characterState->attack = R::Attack::CAMAP_SKILL;
				}
				else{
					characterState->attack = R::Attack::CAMAP_PUNCH_AIR;
				}
				characterState->setState(R::CharacterState::ATTACK);
				return;
			}

			// nếu goku đứng yên thì di chuyển lại gần.
			if (isGokuDungBenPhaiPhamVitancong)	{
				characterState->direction = R::Direction::RIGHT;
				if (characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_RIGHT)
					characterState->setState(R::CharacterState::WALK_RIGHT);
				else
					characterState->setState(R::CharacterState::RIGHT);
			}
			if (isGokuDungBenTraiPhamVitancong)	{
				characterState->direction = R::Direction::LEFT;
				if (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::WALK_LEFT)
					characterState->setState(R::CharacterState::WALK_LEFT);
				else
					characterState->setState(R::CharacterState::LEFT);
			}
			return;
		}
	
	}

}



KarillinDecision::KarillinDecision() { this->isActive = true; canAvoidGokuAttack = true; totalAvoid = 0; }
void KarillinDecision::decision(artemis::Entity &e){
	return;
}
void KarillinDecision::obsever(artemis::Entity &e){

	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
	PhysicComponent* gokuPhysic = (PhysicComponent*)goku.getComponent<PhysicComponent>();

	BoundComponent* characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* characterState = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	if (characterState->state == R::CharacterState::DEFENSE){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){
		return;
	}

	float distance = characterPosition->x - gokuPosition->x;
	float phamViTanCong = 20;


	bool isGokuDungyen = gokuState->state == R::CharacterState::STAND;
	bool isGokuDichuyenSangTrai = (gokuState->state == R::CharacterState::WALK_LEFT) || (gokuState->state == R::CharacterState::LEFT);
	bool isGokuDichuyenSangPhai = (gokuState->state == R::CharacterState::WALK_RIGHT) || (gokuState->state == R::CharacterState::RIGHT);
	bool isGokuDichuyenRaxaBenTrai = ((gokuPosition->x < characterPosition->x) && gokuPhysic->vx < 0);
	bool isGokuDichuyenRaxaBenPhai = ((gokuPosition->x > characterPosition->x) && gokuPhysic->vx > 0);
	bool isGokuDichuyenLaiGan = (!isGokuDichuyenRaxaBenPhai && !isGokuDichuyenRaxaBenTrai && gokuPhysic->vx != 0);

	bool isCharacterDangDichuyen = (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT);
	bool isDichuyenSangTrai = (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::LEFT);
	bool isDichuyenSangPhai = (characterState->state == R::CharacterState::WALK_RIGHT) || (characterState->state == R::CharacterState::RIGHT);

	bool isGokuDungBenPhaiPhamVitancong = (gokuPosition->x + gokuBound->x1) > (characterPosition->x + characterBound->x2 + phamViTanCong);
	bool isGokuDungBenTraiPhamVitancong = (gokuPosition->x + gokuBound->x2) < (characterPosition->x + characterBound->x1 - phamViTanCong);
	bool isGokuDungTrongPhamVitancong = (!isGokuDungBenPhaiPhamVitancong && !isGokuDungBenTraiPhamVitancong);

	bool isGokuDungGanBenTrai = distance>0 && distance < characterBound->getWidth() / 2;
	bool isGokuDungGanBenPhai = distance <0 && distance > -characterBound->getWidth() / 2;
	bool coTheRadon = (characterState->state == R::CharacterState::STAND) || (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::WALK_RIGHT || (characterState->state == R::CharacterState::LEFT) || (characterState->state == R::CharacterState::RIGHT));


	if (isGokuDungTrongPhamVitancong){
		// nằm trong phạm vi tấn công
		if (!isGokuDungGanBenTrai && !isGokuDungGanBenPhai){
			// Có thể tấn công
			if (coTheRadon){
				if (distance > 0) {
					characterState->direction = R::Direction::LEFT;
					characterSkeleton->node->setScaleX(-1);
				}
				else {
					characterState->direction = R::Direction::RIGHT;
					characterSkeleton->node->setScaleX(1);
				}
				srand(time(NULL));
				int random = rand() % 7 + 1;
				if (random == 1){ characterState->attack = R::Attack::KARILLIN_KICK1; }
				else if (random == 2){ characterState->attack = R::Attack::KARILLIN_KICK3; }
				else if (random == 3){ characterState->attack = R::Attack::KARILLIN_PUNCH2; }
				else if (random == 4){ characterState->attack = R::Attack::KARILLIN_BEAT3; }
				else if (random == 5){ characterState->attack = R::Attack::KARILLIN_KICK2; }
				else { characterState->attack = R::Attack::KARILLIN_BEAT1; }
				characterState->setState(R::CharacterState::ATTACK);
			}
		}
		else{
			// không thể tấn công
			if (isGokuDichuyenSangPhai) characterPosition->x -= 2;
			else if (isGokuDichuyenSangTrai) characterPosition->x += 2;
			else if (isGokuDungyen){
				if (isGokuDungGanBenPhai) characterPosition->x -= 2;
				if (isGokuDungGanBenTrai) characterPosition->x += 2;
			}
		}
	}
	else {
		// ở ngoài phạm vi tấn công

		//tranh don tam xa
		if (world->getTagManager()->isSubscribed("gokupunch")){
			if (canAvoidGokuAttack){
				totalAvoid++;
				canAvoidGokuAttack = false;
			}
			if (totalAvoid % 3 == 1){
				return;
			}

			artemis::Entity &gokuPunch = world->getTagManager()->getEntity("gokupunch");
			SkillComponent* skill = (SkillComponent*)gokuPunch.getComponent<SkillComponent>();
			Rect characterRect = Rect(characterPosition->x + characterBound->x1, characterPosition->y + characterBound->y1, characterBound->getWidth(), characterBound->getHeight());

			if (skill->kamekameha->direction > 0 && ((characterRect.getMinX() - skill->kamekameha->node->getPositionX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					totalAvoid++;
					return;
				}
			}
			if (skill->kamekameha->direction <0 && ((skill->kamekameha->node->getPositionX() - characterRect.getMaxX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					totalAvoid++;
					return;
				}
			}
		}
		else{
			canAvoidGokuAttack = true;
		}
		
		// Goku đứng yên
		if (isGokuDungyen){
			// có thể ra chưởng tầm xa thì chiến luôn :
			if (characterState->state != R::CharacterState::JUMP && !isCharacterDangDichuyen &&  characterInfo->hasManaForSkill(40)){
				characterState->doneAction = false;
				characterState->attack = R::Attack::KARILLIN_PUNCH1;
				characterState->setState(R::CharacterState::ATTACK);
				return;
			}

			if (isGokuDungBenPhaiPhamVitancong)	{
				characterState->direction = R::Direction::RIGHT;
				if (characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_RIGHT)
					characterState->setState(R::CharacterState::WALK_RIGHT);
				else
					characterState->setState(R::CharacterState::RIGHT);
			}
			if (isGokuDungBenTraiPhamVitancong)	{
				characterState->direction = R::Direction::LEFT;
				if (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::WALK_LEFT)
					characterState->setState(R::CharacterState::WALK_LEFT);
				else
					characterState->setState(R::CharacterState::LEFT);
			}
			return;
		}
		else{
		// Goku không đứng yên 
		
		}

	}

}



PicoloDecision::PicoloDecision() { this->isActive = true; }
void PicoloDecision::decision(artemis::Entity &e){
	return;
}
void PicoloDecision::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	BoundComponent* gokuBound = (BoundComponent*)e.getComponent<BoundComponent>();
	PosComponent* gokuPosition = (PosComponent*)goku.getComponent<PosComponent>();
	PosComponent* characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	StateComponent* gokuState = (StateComponent*)goku.getComponent<StateComponent>();
	CharacterInfoComponent* characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	CharacterInfoComponent* gokuInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();


	float distance = abs(gokuPosition->x - characterPosition->x);
	bool gokuOnTheLeft = gokuPosition->x < characterPosition->x;


	// trường hợp đánh thằng goku ngã
	if (gokuState->state == R::CharacterState::DEFENSE && (gokuState->defense == R::Defense::TRUNG_DON_NGA || gokuState->state == R::CharacterState::STAND_UP)){
		// đứng khiên khích
		return;
	}

	// trường hợp đang dính đòn thì k làm gì
	if (stateComponent->state == R::CharacterState::DEFENSE && (stateComponent->defense == R::Defense::TRUNG_DON_NGA || stateComponent->state == R::CharacterState::STAND_UP)){
		return;
	}
	if (stateComponent->state == R::CharacterState::DEFENSE && stateComponent->defense == R::Defense::TRUNG_DON){
		//	if (stateComponent->state != R::CharacterState::STAND) stateComponent->setState(R::CharacterState::STAND);
		return;
	}

	if (stateComponent->state == R::CharacterState::ATTACK){
		return;
	}

	stateComponent->direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
	if (distance > gokuBound->getWidth() + 40){
		if (((stateComponent->state == R::CharacterState::WALK_LEFT && gokuOnTheLeft) || (stateComponent->state == R::CharacterState::WALK_RIGHT && !gokuOnTheLeft)) && stateComponent->doneAction){
			if (distance > (gokuBound->getWidth() + 50) && distance < (gokuBound->getWidth() + 80)){
				srand(time(NULL));
				int random = rand() % 6 + 1;
				if (random < 4 && characterInfo->hasManaForSkill(40)){
					stateComponent->doneAction = false;
					stateComponent->attack = R::Attack::PICOLO_POWER1;
					stateComponent->setState(R::CharacterState::ATTACK);
					return;
				}

				if (random >= 4 && characterInfo->hasManaForSkill(40)){
					stateComponent->doneAction = false;
					stateComponent->attack = R::Attack::PICOLO_POWER2;
					stateComponent->setState(R::CharacterState::ATTACK);
					return;
				}
			}
		}

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
	else if (distance < gokuBound->getWidth() / 4){
		if (gokuState->state == R::CharacterState::STAND){
			stateComponent->setState(gokuOnTheLeft ? R::CharacterState::RIGHT : R::CharacterState::LEFT);
			characterPosition->x += gokuOnTheLeft ? 20 : -20;
		}
		else{
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
	else{
		float thinkingTime = .2f;
		if (stateComponent->state == R::CharacterState::STAND && stateComponent->time_on_state >= thinkingTime){ // attack

			srand(time(NULL));
			int random = rand() % 6 + 1;

			if (distance > gokuBound->getWidth() + 10){
				if (random <= 3) stateComponent->attack = R::Attack::PICOLO_KICK;
				else stateComponent->attack = R::Attack::PICOLO_ATTACK3;
			}
			else{
				// kick1, beat1, punch2,beat3,kick3,kick2
				if (random <= 2){ stateComponent->attack = R::Attack::PICOLO_KICK; }
				else if (random <= 4){ stateComponent->attack = R::Attack::PICOLO_ATTACK2; }
				else { stateComponent->attack = R::Attack::PICOLO_ATTACK1; }

			}
			stateComponent->setState(R::CharacterState::ATTACK);
		}
		if (gokuState->state == R::CharacterState::STAND && (stateComponent->state == R::CharacterState::WALK_LEFT || stateComponent->state == R::CharacterState::WALK_RIGHT || stateComponent->state == R::CharacterState::LEFT || stateComponent->state == R::CharacterState::RIGHT)){
			stateComponent->setState(R::CharacterState::STAND);
		}
	}
}

