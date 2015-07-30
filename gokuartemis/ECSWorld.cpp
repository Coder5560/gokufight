#include "gokuartemis/ECSWorld.h"

ECSWorld* ECSWorld::instance = NULL;
ECSWorld::ECSWorld() : goku(nullptr), world(nullptr){	}
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



void ECSWorld::createWorld(R::Match_Type matchType){
	this->matchType = matchType;
	world = new artemis::World();
	EntityUtils::getInstance()->setWorld(world);


	createEnemyCharacter();
	createMainCharacter();

	//create GameState system manager
	artemis::Entity &e = world->createEntity();
	e.addComponent(new GameStateComponent());
	e.setTag("gameState");
	e.refresh();







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
	setSystem(new AttackSystem());
	setSystem(new MotionSystem());
	setSystem(new SkeletonSystem());
	setSystem(new UICharacterSystem());
	setSystem(new CharacterStateSystem());
	setSystem(new RenderSystem());
	setSystem(new RemoveEntitySystem());

	setSystem(new DebugSystem());
	//setSystem(new DecisionSystem());
	

	world->getSystemManager()->initializeAll();

	gameHud = new GameHud(Director::getInstance()->getOpenGLView()->getFrameSize());
	gameHud->setAnchorPoint(Vec2(.5, .5));
	gameHud->buildComponent();
	gameHud->setCallBack([=](GameHud::EventType event, GameHud::TouchType type) {
		ECSWorld::getInstance()->inputSystem->notifyInput(event, type);
	});
	RenderLayer::getInstance()->getHudLayer()->addChild(gameHud);
	RenderLayer::getInstance()->getHudLayer()->setCameraMask((unsigned short)CameraFlag::USER1);

	ui::Text* text = ui::Text::create("Touch to begin", "fonts/Marker Felt.ttf", 40);
	text->setColor(Color3B::RED);
	Node* node = RenderLayer::getInstance()->createHudNode();
	node->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	text->setAnchorPoint(Vec2(.5, .5));
	text->setPosition(node->getContentSize() / 2);
	auto fadeIn = FadeIn::create(.5);
	auto fadeOut = FadeOut::create(.5);
	text->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, nullptr)));

	node->addChild(text);
	node->setTag(200);
	node->setCameraMask((unsigned short)CameraFlag::USER1);
}

void ECSWorld::createMainCharacter(){	// create main Character
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/goku.png";
	characterInfo->isMainCharacter = true;
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
		
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Goku.json",
		"spine/Goku.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("goku");
	skeletonAnimation->setScale(.4);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);
	node->setVisible(false);

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Goku();
	stateComponent->setState(R::CharacterState::START);

	DecisionComponent* decisionComponent = new DecisionComponent();
	decisionComponent->decisionBase = new GokuDecision();
	decisionComponent->decisionBase->setWorld(world);


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::GOKU));
	character.addComponent(new PosComponent(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-30, -12, 30, 60));
	character.addComponent(new WallSensorComponent());
	character.addComponent(new GravityComponent());
	character.addComponent(new PhysicComponent());
	character.addComponent(stateComponent);
	character.addComponent(characterSkeleton);
	character.addComponent(characterInfo);
	character.addComponent(decisionComponent);
	character.setTag("goku");
	character.refresh();

	((PhysicComponent*)(character.getComponent<PhysicComponent>()))->bounce = 0;
}

void ECSWorld::createEnemyCharacter(){
	if (matchType == R::Match_Type::GOKU_GIRAN){
		createGiranCharacter();
	}
}

void ECSWorld::createGiranCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/giran.png";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Giran.json",
		"spine/Giran.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
	skeletonAnimation->setSkin("giran");
	skeletonAnimation->setScale(.4);

	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);
	node->setVisible(false);

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;

	DecisionComponent* decisionComponent = new DecisionComponent();
	decisionComponent->DECISION_TIME = 1;
	decisionComponent->decisionBase = new GiranDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Giran();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::GIRAN));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-50, -12, 50, 100));
	character.addComponent(new WallSensorComponent());
	character.addComponent(new GravityComponent());
	character.addComponent(new PhysicComponent());
	character.addComponent(decisionComponent);
	character.addComponent(stateComponent);
	character.addComponent(characterSkeleton);
	character.addComponent(characterInfo);
	character.setTag("enemy");

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
	if (world->getTagManager()->isSubscribed("goku")){
		artemis::Entity &e = world->getTagManager()->getEntity("goku");
		PosComponent* pos = (PosComponent*)e.getComponent<PosComponent>();
		auto defaulcamera = Camera::getDefaultCamera();
		float cameraX = defaulcamera->getPositionX();
		if (cameraX - pos->x > R::Constants::WIDTH_SCREEN / 2 - 140){
			cameraX = pos->x + R::Constants::WIDTH_SCREEN / 2 - 140;
		}

		if (pos->x - cameraX > R::Constants::WIDTH_SCREEN / 2 - 140){
			cameraX = pos->x - R::Constants::WIDTH_SCREEN / 2 + 140;
		}
		cameraX = cameraX < R::Constants::WIDTH_SCREEN / 2 ? R::Constants::WIDTH_SCREEN / 2 : cameraX;
		cameraX = cameraX>(R::Constants::MAX_SCREEN_WIDTH - R::Constants::WIDTH_SCREEN / 2) ? (R::Constants::MAX_SCREEN_WIDTH - R::Constants::WIDTH_SCREEN / 2) : cameraX;
		defaulcamera->setPositionX(cameraX);
	}
}
