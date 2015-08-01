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
	if (characterType->type == R::CharacterType::GOKU){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::GOKU_PUNCH1 || state->attack == R::Attack::GOKU_PUNCH2 || state->attack == R::Attack::GOKU_PUNCH3){
				createGokuPunchAttack(attacker, attackComponent);
			}
			if (state->attack == R::Attack::GOKU_BEAT1 || state->attack == R::Attack::GOKU_BEAT2 || state->attack == R::Attack::GOKU_BEAT3){
				createGokuBeatAttack(attacker, attackComponent);
			}

			if (state->attack == R::Attack::GOKU_KICK1 || state->attack == R::Attack::GOKU_KICK2 || state->attack == R::Attack::GOKU_KICK3){
				createGokuKickAttack(attacker, attackComponent);
			}
		}
	}
	else if (characterType->type == R::CharacterType::GIRAN){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::GIRAN_PUNCH1)createGiranAttack1(attacker, attackComponent);
			if (state->attack == R::Attack::GIRAN_PUNCH2)createGiranAttack2(attacker, attackComponent);
			if (state->attack == R::Attack::GIRAN_PUNCH3)createGiranAttack3(attacker, attackComponent);
		}
	}
	else if (characterType->type == R::CharacterType::BEAR){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::BEAR_ATTACK1)createBearAttack1(attacker, attackComponent);
			if (state->attack == R::Attack::BEAR_ATTACK2)createBearAttack2(attacker, attackComponent);
		}
	}
	else if (characterType->type == R::CharacterType::JACKIECHUN){
		if (state->state == R::CharacterState::ATTACK){
			if (state->attack == R::Attack::JACK_BEAT3)createJackiechunBeat3(attacker, attackComponent);
			if (state->attack == R::Attack::JACK_KICK2)createJackiechunKick2(attacker, attackComponent);
			if (state->attack == R::Attack::JACK_KICK3)createJackiechunKick3(attacker, attackComponent);
			if (state->attack == R::Attack::JACK_PUNCH_1)createJackiechunPunch1(attacker, attackComponent);
			if (state->attack == R::Attack::JACK_PUNCH_2)createJackiechunPunch2(attacker, attackComponent);
			if (state->attack == R::Attack::JACK_PUNCH_AIR)createJackiechunPunchAir(attacker, attackComponent);
		}
	}

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
	position->y = attackPosition->y + attackBound->getCenterY()+20;

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
	position->y = attackPosition->y +20;

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
	position->x = attackPosition->x ;
	position->y = attackPosition->y + 20 ;

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
	position->x = attackPosition->x ;
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
	position->x = attackPosition->x ;
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

	Sprite* sprite = Sprite::create("effect_chuong.png");
	sprite->setAnchorPoint(Vec2(.5, .5));
	sprite->ignoreAnchorPointForPosition(false);
	sprite->setScale(.4);
	renderNode->node->addChild(sprite);

	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 600 * (skeleton->node->getScaleX() > 0 ? 1 : -1);
	physicComponent->friction = 0;


	GravityComponent* gravity = new GravityComponent();
	gravity->enable = false;
	dan.addComponent(new PosComponent(attackPosition->x + attackBound->x2 + 5, attackPosition->y + attackBound->getHeight() / 2 - 15));
	dan.addComponent(new BoundComponent(-15, -15, 15, 15));
	dan.addComponent(attackComponent);
	dan.addComponent(gravity);
	dan.addComponent(physicComponent);
	dan.addComponent(renderNode);
	dan.addComponent(new WallSensorComponent());
	dan.setGroup("gokus");
	dan.refresh();
}
void EntityUtils::createGokuPunchAttack(artemis::Entity &attacker, AttackComponent* attackComponent){
	RenderComponent* renderNode = new RenderComponent();
	renderNode->renderType = R::RenderType::DYNAMIC;
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();

	//Sprite* sprite = Sprite::create("effect_chuong.png");
	//sprite->setAnchorPoint(Vec2(.5, .5));
	//sprite->ignoreAnchorPointForPosition(false);
	//sprite->setScale(.4);
	//renderNode->node->addChild(sprite);

	Sprite* sprite = Sprite::create("particles/particle_chuong.png");

	ParticleSystemQuad* chuong = ParticleSystemQuad::create("particles/particle_chuong.plist");
	chuong->setTexture(sprite->getTexture());
	chuong->setAnchorPoint(Vec2(.5, .5));
	chuong->setPosition(Vec2(0, 0));
	chuong->setScale(.4);
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
void EntityUtils::createGokuBeatAttack(artemis::Entity &attacker, AttackComponent* attackComponent){
	PosComponent* attackPosition = (PosComponent*)attacker.getComponent<PosComponent>();
	BoundComponent* attackBound = (BoundComponent*)attacker.getComponent<BoundComponent>();
	SkeletonComponent* skeleton = (SkeletonComponent*)attacker.getComponent<SkeletonComponent>();
	bool isLeftDirection = skeleton->node->getScaleX() < 0;
	artemis::Entity &dan = (world->getEntityManager()->create());

	PhysicComponent* physicComponent = new PhysicComponent(true);
	physicComponent->vx = 80 * (isLeftDirection ? -1 : 1);
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
void EntityUtils::createGokuKickAttack(artemis::Entity &attacker, AttackComponent* attackComponent){
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
	if (renderComponent){
		renderComponent->node->removeFromParent();
	}
	world->getEntityManager()->remove(e);

}
