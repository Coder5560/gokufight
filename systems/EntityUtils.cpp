#include "EntityUtils.h"
#include "gokuartemis/Components.h"
#include "gokuartemis/RenderComponent.h"
#include "R.h"

EntityUtils* EntityUtils::instance = nullptr;
EntityUtils::EntityUtils(){}
EntityUtils::~EntityUtils(){}

EntityUtils* EntityUtils::getInstance(){
	if (instance == nullptr){
		instance = new EntityUtils();
	}
	return instance;
}


void EntityUtils::setWorld(artemis::World *world){
	this->world = world;
}
void EntityUtils::createAttackEntity(artemis::Entity &attacker, AttackComponent* attackComponent){
	CharacterInfoComponent* attackerInfo = (CharacterInfoComponent*)attacker.getComponent<CharacterInfoComponent>();
	if (!attackerInfo->hasManaForSkill(attackComponent->manaOfAttack)){
		StateComponent* state = (StateComponent*)attacker.getComponent<StateComponent>();
		// show text not enough mana
		attackerInfo->notifyNotEnoughMana();
		PosComponent* position = (PosComponent*)attacker.getComponent<PosComponent>();
		Node* node = RenderLayer::getInstance()->createGameNode();
		node->setPosition(Vec2(position->x, position->y + 60));
		NotEnoughManaEffect* effect = new NotEnoughManaEffect(node);
		effect->start();
		return;
	}
	else{
		attackerInfo->power -= attackComponent->manaOfAttack;
	}

	StateComponent* state = (StateComponent*)attacker.getComponent<StateComponent>();
	CharacterTypeComponent* characterType = (CharacterTypeComponent*)attacker.getComponent<CharacterTypeComponent>();
	if (state->state != R::CharacterState::ATTACK){
		return;
	}
	if (characterType->type == R::CharacterType::GOKU){
		if (state->attack == R::Attack::GOKU_PUNCH1) createGokuPunch1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::GOKU_PUNCH2) createGokuPunch2Attack(attacker, attackComponent);

		if (state->attack == R::Attack::GOKU_BEAT1) createGokuBeat1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::GOKU_BEAT2) createGokuBeat2Attack(attacker, attackComponent);
		if (state->attack == R::Attack::GOKU_BEAT3) createGokuBeat3Attack(attacker, attackComponent);

		if (state->attack == R::Attack::GOKU_KICK1) createGokuKick1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::GOKU_KICK2) createGokuKick2Attack(attacker, attackComponent);
		if (state->attack == R::Attack::GOKU_KICK3) createGokuKick3Attack(attacker, attackComponent);

	}
	if (characterType->type == R::CharacterType::KARILLIN){
		if (state->attack == R::Attack::KARILLIN_PUNCH1) createKarilinPunch1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::KARILLIN_PUNCH2) createKarilinPunch2Attack(attacker, attackComponent);

		if (state->attack == R::Attack::KARILLIN_BEAT1) createKarilinBeat1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::KARILLIN_BEAT2) createKarilinBeat2Attack(attacker, attackComponent);
		if (state->attack == R::Attack::KARILLIN_BEAT3) createKarilinBeat3Attack(attacker, attackComponent);

		if (state->attack == R::Attack::KARILLIN_KICK1) createKarilinKick1Attack(attacker, attackComponent);
		if (state->attack == R::Attack::KARILLIN_KICK2) createKarilinKick2Attack(attacker, attackComponent);
		if (state->attack == R::Attack::KARILLIN_KICK3) createKarilinKick3Attack(attacker, attackComponent);

	}
	else if (characterType->type == R::CharacterType::GIRAN){
		if (state->attack == R::Attack::GIRAN_PUNCH1)createGiranAttack1(attacker, attackComponent);
		if (state->attack == R::Attack::GIRAN_PUNCH2)createGiranAttack2(attacker, attackComponent);
		if (state->attack == R::Attack::GIRAN_PUNCH3)createGiranAttack3(attacker, attackComponent);

	}
	else if (characterType->type == R::CharacterType::BEAR){
		if (state->attack == R::Attack::BEAR_ATTACK1)createBearAttack1(attacker, attackComponent);
		if (state->attack == R::Attack::BEAR_ATTACK2)createBearAttack2(attacker, attackComponent);

	}
	else if (characterType->type == R::CharacterType::JACKIECHUN){
		if (state->attack == R::Attack::JACK_BEAT3)createJackiechunBeat3(attacker, attackComponent);
		if (state->attack == R::Attack::JACK_KICK2)createJackiechunKick2(attacker, attackComponent);
		if (state->attack == R::Attack::JACK_KICK3)createJackiechunKick3(attacker, attackComponent);
		if (state->attack == R::Attack::JACK_PUNCH_1)createJackiechunPunch1(attacker, attackComponent);
		if (state->attack == R::Attack::JACK_PUNCH_2)createJackiechunPunch2(attacker, attackComponent);
		if (state->attack == R::Attack::JACK_PUNCH_AIR)createJackiechunPunchAir(attacker, attackComponent);

	}

	else if (characterType->type == R::CharacterType::TEGIAC){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::TEGIAC_BEAT3)createTegiacBeat3(attacker, attackComponent);
			if (state->attack == R::Attack::TEGIAC_KICK2)createTegiacKick2(attacker, attackComponent);
			if (state->attack == R::Attack::TEGIAC_KICK3){ createTegiacKick3(attacker, attackComponent); }
			if (state->attack == R::Attack::TEGIAC_PUNCH1)createTegiacPunch1(attacker, attackComponent);
			if (state->attack == R::Attack::TEGIAC_PUNCH2)createTegiacPunch2(attacker, attackComponent);
			if (state->attack == R::Attack::TEGIAC_PUNCH3)createTegiacPunch3(attacker, attackComponent);
			if (state->attack == R::Attack::TEGIAC_PUNCH_AIR)createTegiacPunchAir(attacker, attackComponent);
		}
	}

	else if (characterType->type == R::CharacterType::RUA){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::RUA_ATTACK1)createRuaAttack1(attacker, attackComponent);
			if (state->attack == R::Attack::RUA_ATTACK2)createRuaAttack2(attacker, attackComponent);
			if (state->attack == R::Attack::RUA_ATTACK3){ createRuaAttack3(attacker, attackComponent); }
			if (state->attack == R::Attack::RUA_ATTACK4)createRuaAttack4(attacker, attackComponent);
			if (state->attack == R::Attack::RUA_ATTACK5)createRuaAttack5(attacker, attackComponent);
		}
	}

	else if (characterType->type == R::CharacterType::CAMAP){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::CAMAP_KICK2)createCamapKick2(attacker, attackComponent);
			if (state->attack == R::Attack::CAMAP_PUNCH1)createCamapPunch1(attacker, attackComponent);
			if (state->attack == R::Attack::CAMAP_PUNCH2){ createCamapPunch2(attacker, attackComponent); }
			if (state->attack == R::Attack::CAMAP_PUNCH3)createCamapPunch3(attacker, attackComponent);
			if (state->attack == R::Attack::CAMAP_PUNCH_AIR)createCamapPunchAir(attacker, attackComponent);
			if (state->attack == R::Attack::CAMAP_SKILL)createCamapSkill(attacker, attackComponent);
		}
	}

	else if (characterType->type == R::CharacterType::PICOLO){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::PICOLO_ATTACK1)createPicoloAttack1(attacker, attackComponent);
			if (state->attack == R::Attack::PICOLO_ATTACK2)createPicoloAttack2(attacker, attackComponent);
			if (state->attack == R::Attack::PICOLO_ATTACK3){ createPicoloAttack3(attacker, attackComponent); }
			if (state->attack == R::Attack::PICOLO_KICK)createPicoloAttackKick(attacker, attackComponent);
			//	if (state->attack == R::Attack::PICOLO_POWER1)createPicoloAttackPower1(attacker, attackComponent);
			//	if (state->attack == R::Attack::PICOLO_POWER2)createPicoloAttackPower2(attacker, attackComponent);
		}
	}

}

void EntityUtils::createCamapPunch1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -40;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createCamapPunch2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() - 10;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createCamapPunch3(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY();

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createCamapKick2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY();

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createCamapPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY();

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createCamapSkill(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 40 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY();

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}



void EntityUtils::createRuaAttack1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY();

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}

void EntityUtils::createRuaAttack2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 120 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -60;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 70;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}

void EntityUtils::createRuaAttack3(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 120 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createRuaAttack4(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 10;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}

void EntityUtils::createRuaAttack5(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 10;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}

void EntityUtils::createTegiacBeat3(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 60 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -40;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createTegiacKick2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 140 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createTegiacKick3(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 260 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();



}
void EntityUtils::createTegiacPunch1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 70 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -10;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createTegiacPunch2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 140 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();

}
void EntityUtils::createTegiacPunch3(artemis::Entity &attacker, AttackComponent* attackComponent){

	RenderComponent* renderNode = new RenderComponent();
	renderNode->renderType = R::RenderType::DYNAMIC;
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();

	Sprite* sprite = Sprite::create("particles/particle_chuong.png");

	ParticleSystemQuad* chuong = ParticleSystemQuad::create("particles/goku_chuong.plist");
	chuong->setTexture(sprite->getTexture());
	chuong->setAnchorPoint(Vec2(.5, .5));
	chuong->setPosition(Vec2(0, 0));
	chuong->setScale(.6);
	chuong->ignoreAnchorPointForPosition(false);
	chuong->setColor(Color3B::GREEN);
	renderNode->node->addChild(chuong);



	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 600 * (skeleton->node->getScaleX() > 0 ? 1 : -1);
	physicComponent->friction = 0;
	renderNode->node->setScaleX((skeleton->node->getScaleX() > 0 ? 1 : -1));


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(new PosComponent(attackPosition->x + ((renderNode->node->getScaleX() > 0) ? 40 : -40), attackPosition->y + attackBound->getHeight() / 2 - 20));
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(renderNode);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createTegiacPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 200 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + (isLeftDirection ? 40 : -40);
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}



void EntityUtils::createJackiechunBeat3(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 70 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -10;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->getCenterY() + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createJackiechunKick2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 140 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createJackiechunKick3(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 100 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 10;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createJackiechunPunch1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 70 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -10;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createJackiechunPunch2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 140 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -0;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();

}
void EntityUtils::createJackiechunPunchAir(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 20 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -140;
	physicComponent->friction = 0;

	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + (isLeftDirection ? -50 : 50);
	position->y = attackPosition->y + attackBound->y2 + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}


void EntityUtils::createBearAttack1(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());


	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 140 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -200;
	physicComponent->friction = 0;




	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + (isLeftDirection ? -40 : 40);
	position->y = attackPosition->y + attackBound->y2 + 20;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createBearAttack2(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());


	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -140;
	physicComponent->friction = 0;




	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y2 + 20;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}

void EntityUtils::createGiranAttack1(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());


	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 20 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -40;
	physicComponent->friction = 0;




	PosComponent* position = new PosComponent();
	position->x = isLeftDirection ? (attackPosition->x + attackBound->x1 - 20) : (attackPosition->x + attackBound->x2 + 20);
	position->y = attackPosition->y + attackBound->y2 - 20;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createGiranAttack2(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());


	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 100 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 100;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = isLeftDirection ? (attackPosition->x + attackBound->x1 - 15) : (attackPosition->x + attackBound->x2 + 15);
	position->y = attackPosition->y + 40;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createGiranAttack3(artemis::Entity &attacker, AttackComponent* attackComponent){

	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());


	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 60 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 300;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = isLeftDirection ? (attackPosition->x + attackBound->x1 - 15) : (attackPosition->x + attackBound->x2 + 15);
	position->y = attackPosition->y + 40;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();

}



void EntityUtils::createGokuPunch1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	RenderComponent* renderNode = new RenderComponent();
	renderNode->renderType = R::RenderType::DYNAMIC;
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();

	Sprite* chuong = Sprite::create("textures/globe.png");
	chuong->setAnchorPoint(Vec2(.5, .5));
	chuong->setPosition(Vec2(-1000, 0));
	chuong->setScale(.6);
	chuong->ignoreAnchorPointForPosition(false);
	renderNode->node->addChild(chuong);


	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 600 * (skeleton->node->getScaleX() > 0 ? 1 : -1);
	physicComponent->friction = 0;
	renderNode->node->setScaleX((skeleton->node->getScaleX() > 0 ? 1 : -1));


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(new PosComponent(attackPosition->x, attackPosition->y + attackBound->getHeight() / 2 - 30));
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(renderNode);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuBeat1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 120 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -40;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y2 - 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuKick1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 30;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y1 + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("gokus");
}
void EntityUtils::createGokuPunch2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuBeat2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 80;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuKick2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y1 + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("gokus");
}
void EntityUtils::createGokuBeat3Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -80;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + (isLeftDirection ? 20 : -20);
	position->y = attackPosition->y + attackBound->y2 + 30;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuKick3Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -30;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("gokus");
}




void EntityUtils::createKarilinPunch1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	RenderComponent* renderNode = new RenderComponent();
	renderNode->renderType = R::RenderType::DYNAMIC;
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();

	Sprite* sprite = Sprite::create("particles/particle_chuong.png");
	ParticleSystemQuad* chuong = ParticleSystemQuad::create("particles/particle_chuong.plist");
	chuong->setTexture(sprite->getTexture());
	chuong->setAnchorPoint(Vec2(.5, .5));
	chuong->setPosition(Vec2(0, 0));
	chuong->setScale(.6);
	chuong->ignoreAnchorPointForPosition(false);
	renderNode->node->addChild(chuong);


	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 600 * (skeleton->node->getScaleX() > 0 ? 1 : -1);
	physicComponent->friction = 0;
	renderNode->node->setScaleX((skeleton->node->getScaleX() > 0 ? 1 : -1));


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(new PosComponent(attackPosition->x, attackPosition->y + attackBound->getHeight() / 2 - 30));
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(renderNode);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createKarilinBeat1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 120 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -40;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y2 - 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createKarilinKick1Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 30;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y1 + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createKarilinPunch2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createKarilinBeat2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 180 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 80;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 10;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createKarilinKick2Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + attackBound->y1 + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createKarilinBeat3Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -80;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + (isLeftDirection ? 20 : -20);
	position->y = attackPosition->y + attackBound->y2 + 30;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("enemies");
	dan.refresh();
}
void EntityUtils::createKarilinKick3Attack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -30;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}



void EntityUtils::createPicoloAttack1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createPicoloAttack2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 100 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = -140;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x + 30 * (isLeftDirection ? -1 : 1);
	position->y = attackPosition->y + 120;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createPicoloAttack3(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 200 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createPicoloAttackKick(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 10;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-10, -10, 10, 10));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createPicoloAttackPower1(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}
void EntityUtils::createPicoloAttackPower2(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 160 * (isLeftDirection ? -1 : 1);
	physicComponent->vy = 0;
	physicComponent->friction = 0;


	PosComponent* position = new PosComponent();
	position->x = attackPosition->x;
	position->y = attackPosition->y + 30;

	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(position);
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(new WallSensorComponent());
	dan.refresh();
	dan.setGroup("enemies");
}


void EntityUtils::push(artemis::Entity &e, float rotation, float force) {
	PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
	if (physic) {
		float radianAngle = rotation * M_PI / 180;

		float plusVx = force * cos(radianAngle);
		float plusVy = force * sin(radianAngle);
		physic->vx += plusVx;
		physic->vy += plusVy;
		//	CCLOG("Debug from PhysicSystem : %f - %f", physic->vx, physic->vy);
	}

}

void EntityUtils::stopPhysic(artemis::Entity& e) {
	PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
	if (physic) {
		physic->vx = 0;
		physic->vy = 0;
	}
}

void EntityUtils::clampVelocity(artemis::Entity& e, float minSpeed,
	float maxSpeed) {
	PhysicComponent* physic = (PhysicComponent*)e.getComponent<PhysicComponent>();
	if (physic) {
		Vec2 vTmp = Vec2(physic->vx, physic->vy);
		float length2 = vTmp.x * vTmp.x + vTmp.y * vTmp.y;
		float angle = vTmp.getAngle();
		float max2 = maxSpeed * maxSpeed;
		float min2 = minSpeed * minSpeed;

		if (length2 > max2) {
			physic->vx = maxSpeed * cos(angle);
			physic->vy = maxSpeed * sin(angle);
		}

		if (length2 < min2) {
			physic->vx = minSpeed * cos(angle);
			physic->vy = minSpeed * sin(angle);
		}
	}
}
void EntityUtils::removeEntity(artemis::Entity &e){
	RenderComponent* renderComponent = (RenderComponent*)e.getComponent<RenderComponent>();
	if (renderComponent && renderComponent->node){
		renderComponent->node->removeFromParent();
		world->getEntityManager()->removeComponentsOfEntity(e);
	}
	e.remove();
}

Vec2 EntityUtils::checkAttack(artemis::Entity &defense, SkeletonComponent* skeleton, std::string boneName, float boneRadius){
	Vec2 collisionPoint = Vec2::ZERO;

	spine::SkeletonAnimation* animation = skeleton->skeleton;
	spBone* bone = animation->findBone(boneName);
	if (bone){
		float x = skeleton->node->getPositionX() + skeleton->node->getScaleX()* (bone->skeleton->x + bone->worldX);
		float y = skeleton->node->getPositionY() + skeleton->node->getScaleY()*(bone->skeleton->y + bone->worldY);

		PosComponent* position = (PosComponent*)defense.getComponent<PosComponent>();
		BoundComponent* bound = (BoundComponent*)defense.getComponent<BoundComponent>();

		Rect rect1 = Rect(position->x + bound->x1, position->y + bound->y1, bound->getWidth(), bound->getHeight());
		Rect rect2 = Rect(x - 5, y - 5, 10, 10);
		if (rect1.intersectsCircle(Vec2(x, y), 10)){
			bool isLeftDirection = x < position->x;
			collisionPoint.x = isLeftDirection ? (position->x + bound->x1) / 2 : (position->x + bound->x2) / 2;
			collisionPoint.y = y;
		}
	}

	return collisionPoint;
}