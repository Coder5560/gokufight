#include "AITegiac.h"


AITegiac::AITegiac() : avoidAttack(false){ this->isActive = true; mapInfo = new MapInfo(); }

void AITegiac::obsever(artemis::Entity &e){
	artemis::Entity& goku = world->getTagManager()->getEntity("goku");
	targetBound = (BoundComponent*)goku.getComponent<BoundComponent>();
	characterBound = (BoundComponent*)e.getComponent<BoundComponent>();
	targetPosition = (PosComponent*)goku.getComponent<PosComponent>();
	characterPosition = (PosComponent*)e.getComponent<PosComponent>();
	targetState = (StateComponent*)goku.getComponent<StateComponent>();
	characterState = (StateComponent*)e.getComponent<StateComponent>();
	targetInfo = (CharacterInfoComponent*)goku.getComponent<CharacterInfoComponent>();
	characterInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
	targetPhysic = (PhysicComponent*)goku.getComponent<PhysicComponent>();
	characterPhysic = (PhysicComponent*)e.getComponent<PhysicComponent>();

	targetSkeleton = (SkeletonComponent*)goku.getComponent<SkeletonComponent>();
	characterSkeleton = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	float distance = abs(targetPosition->x - characterPosition->x);
	bool gokuOnTheLeft = targetPosition->x < characterPosition->x;
	bool itmauhon = (targetInfo->blood - characterInfo->blood) > 10;

	//processAvoidAttack();
	//return;

	// trường hợp đánh thằng goku ngã
	if (targetState->state == R::CharacterState::DEFENSE && (targetState->defense == R::Defense::TRUNG_DON_NGA || targetState->state == R::CharacterState::STAND_UP)){ return; }

	// trường hợp đang dính đòn thì k làm gì
	if (characterState->state == R::CharacterState::DEFENSE && (characterState->defense == R::Defense::TRUNG_DON_NGA || characterState->state == R::CharacterState::STAND_UP)){ return; }
	if (characterState->state == R::CharacterState::DEFENSE && characterState->defense == R::Defense::TRUNG_DON){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){ return; }


	if (distance > targetBound->getWidth() + 20){
		if (((characterState->state == R::CharacterState::WALK_LEFT && gokuOnTheLeft) || (characterState->state == R::CharacterState::WALK_RIGHT && !gokuOnTheLeft)) && characterState->doneAction){
			if (distance > (targetBound->getWidth() + 60) && distance < (targetBound->getWidth() + 70)){
				srand(time(NULL));
				int random = rand() % 6 + 1;
				if (random < 4 && characterInfo->hasManaForSkill(40)){
					characterState->doneAction = false;
					characterState->attack = R::Attack::TEGIAC_KICK3;
					characterState->setState(R::CharacterState::ATTACK);
					return;
				}
			}
		}


		if (gokuOnTheLeft){
			if (characterState->state != R::CharacterState::WALK_LEFT && characterState->state != R::CharacterState::LEFT){
				characterState->direction = R::Direction::LEFT;
				characterState->setState(R::CharacterState::LEFT);
			}
			else{
				characterState->direction = R::Direction::LEFT;
				characterState->setState(R::CharacterState::WALK_LEFT);
			}
		}
		else{
			if (characterState->state != R::CharacterState::WALK_RIGHT && characterState->state != R::CharacterState::RIGHT){
				characterState->direction = R::Direction::RIGHT;
				characterState->setState(R::CharacterState::RIGHT);
			}
			else{
				characterState->direction = R::Direction::RIGHT;
				characterState->setState(R::CharacterState::WALK_RIGHT);
			}
		}
	}
	else if (distance < targetBound->getWidth() / 3){
		characterState->setState(gokuOnTheLeft ? R::CharacterState::LEFT : R::CharacterState::RIGHT);
	}
	else{
		float thinkingTime = .02f;
		if ((characterState->state == R::CharacterState::STAND || characterState->state == R::CharacterState::DEFENSE) && characterState->time_on_state >= thinkingTime){ // attack

			srand(time(NULL));
			int random = rand() % 5 + 1;
			R::Direction direction = gokuOnTheLeft ? R::Direction::LEFT : R::Direction::RIGHT;
			characterState->direction = direction;

			if (characterInfo->hasManaForSkill(40)){
				// kich 3 vs punch AIR
				if (distance > targetBound->getWidth()){
					if (random <= 3) characterState->attack = R::Attack::TEGIAC_PUNCH3;
					else characterState->attack = R::Attack::TEGIAC_PUNCH_AIR;
				}
				else{
					if (random <= 2){ characterState->attack = R::Attack::TEGIAC_PUNCH1; }
					else if (random == 3){ characterState->attack = R::Attack::TEGIAC_KICK2; }
					else if (random == 4){ characterState->attack = R::Attack::TEGIAC_BEAT3; }
					else characterState->attack = R::Attack::TEGIAC_PUNCH2;
				}
			}
			else{
				if (random <= 2){ characterState->attack = R::Attack::TEGIAC_PUNCH1; }
				else if (random == 3){ characterState->attack = R::Attack::TEGIAC_KICK2; }
				else if (random == 4){ characterState->attack = R::Attack::TEGIAC_BEAT3; }
				else characterState->attack = R::Attack::TEGIAC_PUNCH2;
			}


			characterState->setState(R::CharacterState::ATTACK);
		}
		if (characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT || characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT){
			characterState->setState(R::CharacterState::STAND);
		}
	}
}
void AITegiac::processAvoidAttack(){
	if (characterState->state == R::CharacterState::DEFENSE){
		return;
	}

	if (characterState->state == R::CharacterState::ATTACK){
		return;
	}

	float distance = characterPosition->x - targetPosition->x;
	float phamViTanCong = 20;


	bool isGokuDungyen = targetState->state == R::CharacterState::STAND;
	bool isGokuDichuyenSangTrai = (targetState->state == R::CharacterState::WALK_LEFT) || (targetState->state == R::CharacterState::LEFT);
	bool isGokuDichuyenSangPhai = (targetState->state == R::CharacterState::WALK_RIGHT) || (targetState->state == R::CharacterState::RIGHT);
	bool isGokuDichuyenRaxaBenTrai = ((targetPosition->x < characterPosition->x) && targetPhysic->vx < 0);
	bool isGokuDichuyenRaxaBenPhai = ((targetPosition->x > characterPosition->x) && targetPhysic->vx > 0);
	bool isGokuDichuyenLaiGan = (!isGokuDichuyenRaxaBenPhai && !isGokuDichuyenRaxaBenTrai && targetPhysic->vx != 0);

	bool isCharacterDangDichuyen = (characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT || characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT);
	bool isDichuyenSangTrai = (characterState->state == R::CharacterState::WALK_LEFT) || (characterState->state == R::CharacterState::LEFT);
	bool isDichuyenSangPhai = (characterState->state == R::CharacterState::WALK_RIGHT) || (characterState->state == R::CharacterState::RIGHT);

	bool isGokuDungBenPhaiPhamVitancong = (targetPosition->x + targetBound->x1) > (characterPosition->x + characterBound->x2 + phamViTanCong);
	bool isGokuDungBenTraiPhamVitancong = (targetPosition->x + targetBound->x2) < (characterPosition->x + characterBound->x1 - phamViTanCong);
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
				float thinkingTime = .02f;
				if ((characterState->state == R::CharacterState::STAND || characterState->state == R::CharacterState::DEFENSE) && characterState->time_on_state >= thinkingTime){ // attack

					srand(time(NULL));
					int random = rand() % 5 + 1;
					if (characterInfo->hasManaForSkill(40)){
						// kich 3 vs punch AIR
						if (distance > targetBound->getWidth()){
							if (random <= 3) characterState->attack = R::Attack::TEGIAC_PUNCH3;
							else characterState->attack = R::Attack::TEGIAC_PUNCH_AIR;
						}
						else{
							if (random <= 2){ characterState->attack = R::Attack::TEGIAC_PUNCH1; }
							else if (random == 3){ characterState->attack = R::Attack::TEGIAC_KICK2; }
							else if (random == 4){ characterState->attack = R::Attack::TEGIAC_BEAT3; }
							else characterState->attack = R::Attack::TEGIAC_PUNCH2;
						}
					}
					else{
						if (random <= 2){ characterState->attack = R::Attack::TEGIAC_PUNCH1; }
						else if (random == 3){ characterState->attack = R::Attack::TEGIAC_KICK2; }
						else if (random == 4){ characterState->attack = R::Attack::TEGIAC_BEAT3; }
						else characterState->attack = R::Attack::TEGIAC_PUNCH2;
					}


					characterState->setState(R::CharacterState::ATTACK);
				}
				if (characterState->state == R::CharacterState::WALK_LEFT || characterState->state == R::CharacterState::WALK_RIGHT || characterState->state == R::CharacterState::LEFT || characterState->state == R::CharacterState::RIGHT){
					characterState->setState(R::CharacterState::STAND);
				}
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


			artemis::Entity &gokuPunch = world->getTagManager()->getEntity("gokupunch");
			SkillComponent* skill = (SkillComponent*)gokuPunch.getComponent<SkillComponent>();
			Rect characterRect = Rect(characterPosition->x + characterBound->x1, characterPosition->y + characterBound->y1, characterBound->getWidth(), characterBound->getHeight());

			if (skill->kamekameha->direction > 0 && ((characterRect.getMinX() - skill->kamekameha->node->getPositionX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					return;
				}
			}
			if (skill->kamekameha->direction <0 && ((skill->kamekameha->node->getPositionX() - characterRect.getMaxX()) > characterBound->getWidth())){
				if (characterState->state != R::CharacterState::JUMP && characterState->state != R::CharacterState::DEFENSE){
					characterState->setState(R::CharacterState::JUMP);
					return;
				}
			}
		}{

		}

		// Goku đứng yên
		if (isGokuDungyen){
			// có thể ra chưởng tầm xa thì chiến luôn :
			if (characterState->state != R::CharacterState::JUMP && !isCharacterDangDichuyen &&  characterInfo->hasManaForSkill(40)){
				characterState->doneAction = false;
				characterState->attack = R::Attack::TEGIAC_KICK3;
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