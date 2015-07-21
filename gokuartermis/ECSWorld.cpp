#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/Components.h"
#include "gokuartermis/Systems.h"
#include "gokuartermis/CharacterSystem.h"
#include "systems/CharacterStateSystem.h"

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
	
	// create main Character
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->tag = "goku1";
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

	artemis::Entity &character = (world->getEntityManager()->create());

	character.addComponent(new PosComponent(400, 400));
	character.addComponent(new BoundComponent(0, 0, 60, 60));
	character.addComponent(new WallSensorComponent());
	character.addComponent(new GravityComponent());
	character.addComponent(new PhysicComponent());
	character.addComponent(new StateComponent());
	character.addComponent(characterSkeleton);
	character.addComponent(characterInfo);
	character.setTag(characterInfo->tag);
	character.refresh();

	((PhysicComponent*)(character.getComponent<PhysicComponent>()))->bounce = 0;


	

	//artemis::Entity& e = world->getTagManager()->getEntity("goku");
	inputSystem = new InputSystem();
	// Tống vào world mấy cái system cho nó xử lý.
	// importance step, the way orgnize system affect to how system running.
	
	setSystem(new GokuCharacter());
	setSystem(new GameStateSystem());
	setSystem(inputSystem);
	
	setSystem(new YamchaCharacter("yamcha", "spine/skeleton.json", "spine/skeleton.atlas"));
	setSystem(new PhysicSystem());
	setSystem(new GravitySystem());
	setSystem(new MapCollisionSystem());
	setSystem(new WallSensorSystem());
	setSystem(new AfterPhysicSystem());
	setSystem(new SkeletonSystem());
	setSystem(new CharacterCollisionSystem());
	setSystem(new UICharacterSystem());
	setSystem(new CharacterStateSystem());

	world->getSystemManager()->initializeAll();
	
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
	
}
