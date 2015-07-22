#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/Systems.h"
#include "gokuartermis/CharacterSystem.h"
#include "systems/CharacterStateSystem.h"
#include "systems/AttackSystem.h"
#include "RenderSystem.h"
#include "Characters/Goku.h"
ECSWorld* ECSWorld::instance = NULL;
ECSWorld::ECSWorld() : goku(nullptr), world(nullptr)
{
}
ECSWorld* ECSWorld::getInstance(){
	if (instance == NULL){
		instance = new ECSWorld();
	}
	return instance;
}


ECSWorld::~ECSWorld()
{
	delete world;
}



void ECSWorld::createWorld(){
	world = new artemis::World();
	EntityUtils::getInstance()->setWorld(world);

	createMainCharacter();
	createEnemyCharacter();






	//artemis::Entity& e = world->getTagManager()->getEntity("goku");
	inputSystem = new InputSystem();
	// Tống vào world mấy cái system cho nó xử lý.
	// importance step, the way orgnize system affect to how system running.

	//setSystem(new GokuCharacter());
	//setSystem(new YamchaCharacter("yamcha", "spine/skeleton.json", "spine/skeleton.atlas"));

	setSystem(new GameStateSystem());
	setSystem(inputSystem);
	setSystem(new PhysicSystem());
	setSystem(new GravitySystem());
	setSystem(new MapCollisionSystem());
	setSystem(new WallSensorSystem());
	setSystem(new MotionSystem());
	setSystem(new SkeletonSystem());
	setSystem(new CharacterCollisionSystem());
	setSystem(new UICharacterSystem());
	setSystem(new CharacterStateSystem());
	setSystem(new RenderSystem());
	setSystem(new AttackSystem());
	setSystem(new DebugSystem());
	setSystem(new DecisionSystem());

	world->getSystemManager()->initializeAll();

}

void ECSWorld::createMainCharacter(){	// create main Character
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->state = R::CharacterState::STAND;
	characterInfo->tag = "goku";
	characterInfo->isMainCharacter = true;
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 60;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	characterInfo->skill_a_power = 5;
	characterInfo->skill_b_power = 5;
	characterInfo->skill_x_power = 5;

	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Goku.json",
		"spine/Goku.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	skeletonAnimation->setScale(.8);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Goku();

	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::GOKU));
	character.addComponent(new PosComponent(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-30, -20, 30, 60));
	character.addComponent(new WallSensorComponent());
	character.addComponent(new GravityComponent());
	character.addComponent(new PhysicComponent());
	character.addComponent(stateComponent);
	character.addComponent(characterSkeleton);
	character.addComponent(characterInfo);
	character.setTag(characterInfo->tag);
	character.refresh();

	((PhysicComponent*)(character.getComponent<PhysicComponent>()))->bounce = 0;
}
void ECSWorld::createEnemyCharacter(){

	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->state = R::CharacterState::STAND;
	characterInfo->tag = "giran";
	characterInfo->isMainCharacter = false;
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 60;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	characterInfo->skill_a_power = 5;
	characterInfo->skill_b_power = 5;
	characterInfo->skill_x_power = 5;

	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Giran.json",
		"spine/Giran.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("giran");
	skeletonAnimation->setScale(.5);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;

	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::GIRAN));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-50, -20, 50, 120));
	character.addComponent(new WallSensorComponent());
	character.addComponent(new GravityComponent());
	character.addComponent(new PhysicComponent());
	character.addComponent(new DecisionComponent());
	character.addComponent(new StateComponent());
	character.addComponent(characterSkeleton);
	character.addComponent(characterInfo);
	character.setTag(characterInfo->tag);
	character.setGroup("enemy");
	character.refresh();
	
	((PhysicComponent*)(character.getComponent<PhysicComponent>()))->bounce = 0;

}
artemis::EntitySystem*  ECSWorld::setSystem(artemis::EntitySystem* system){
	return world->getSystemManager()->setSystem(system);
}

void ECSWorld::processWorld(float delta){
	if (world == nullptr) return;
	accumulate += delta;
	while (accumulate > STEP){
		world->setDelta(STEP);
		world->loopStart();
		world->processWorld();
		accumulate -= STEP;
	}
	artemis::Entity &e = world->getTagManager()->getEntity("goku");
	PosComponent* pos = (PosComponent*)e.getComponent<PosComponent>();
	auto defaulcamera = Camera::getDefaultCamera();
	float cameraX = pos->x < 20 ? 20 : pos->x;
	cameraX = pos->x > 800 ? 800 : pos->x;
	defaulcamera->setPositionX(cameraX);


}
