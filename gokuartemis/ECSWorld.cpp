#include "gokuartemis/ECSWorld.h"

ECSWorld* ECSWorld::instance = NULL;
ECSWorld::ECSWorld() : isIgnoreWorld(false), match(0), accumulate(0), STEP(0.015f), goku(nullptr), world(nullptr){	}
ECSWorld* ECSWorld::getInstance(){
	if (instance == NULL){
		instance = new ECSWorld();
	}
	return instance;
}

artemis::World* ECSWorld::getWorld(){
	return world;
}


ECSWorld::~ECSWorld()
{
	delete world;
}



void ECSWorld::createWorld(R::Match_Type matchType){
	this->matchType = matchType;
	if (R::Constants::musicEnable) {
		srand(time(NULL));
		int rad = rand() / 6+ 1;	
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(R::Constants::musicVolumn);
		if (rad <= 2) CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/theme_2.mp3", true);
		else if (rad <= 4) CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/theme_1.mp3", true);
		else  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/theme_3.mp3", true);
	}
	std::string str = "backgrounds/bg6.png";
	switch (matchType){
	case R::Match_Type::GOKU_BEAR:
		str = "backgrounds/bg6.png";
		break;
	case R::Match_Type::GOKU_TEGIAC:
		str = "backgrounds/bg7.png";
		break;
	case R::Match_Type::GOKU_KARILLIN:
		str = "backgrounds/bg4.png";
		break;
	case R::Match_Type::GOKU_GIRAN:
		str = "backgrounds/bg5.png";
		break;
	case R::Match_Type::GOKU_RUA:
		str = "backgrounds/bg1.png";
		break;
	case R::Match_Type::GOKU_CAMAP:
		str = "backgrounds/bg3.png";
		break;
	case R::Match_Type::GOKU_JACKIECHUN:
		str = "backgrounds/bg2.png";
		break;
	case R::Match_Type::GOKU_PICOLO:
		str = "backgrounds/bg8.png";
		break;
	}


	Sprite* bg = Sprite::create(str);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);

	world = new artemis::World();
	EntityUtils::getInstance()->setWorld(world);


	createEnemyCharacter();
	createMainCharacter();
	createCameraFollowEntity();

	//create GameState system manager
	artemis::Entity &e = world->createEntity();
	e.addComponent(new GameStateComponent());
	e.setTag("gameState");
	e.refresh();

	//create CharacterUI system manager
	artemis::Entity &ui = world->createEntity();
	ui.addComponent(new CharacterUIComponent());
	ui.setTag("characterrender");
	ui.refresh();


	inputSystem = new InputSystem();


	setSystem(new GameStateSystem());
	setSystem(inputSystem);

	setSystem(new SpecialSkillSystem());
	setSystem(new PhysicSystem());
	setSystem(new GravitySystem());
	setSystem(new MapCollisionSystem());
	setSystem(new WallSensorSystem());
	setSystem(new AttackSystem());
	setSystem(new MotionSystem());
	setSystem(new SkeletonSystem());
	//	setSystem(new UICharacterSystem());
	setSystem(new CharacterRenderSystem());
	setSystem(new CharacterStateSystem());
	setSystem(new RenderSystem());
	setSystem(new RemoveEntitySystem());
	setSystem(new CatFollowGokuSystem());
	setSystem(new BombSystem());
	setSystem(new CameraFollowSystem());
	//setSystem(new DebugSystem());
	setSystem(new DecisionSystem());
	if (matchType == R::Match_Type::GOKU_BEAR_INTRODUCE){
		//createIntroduceEntity
		artemis::Entity &introduceEntity = world->createEntity();
		introduceEntity.addComponent(new IntroduceComponent());
		introduceEntity.setTag("introduceEntity");
		introduceEntity.refresh();

		setSystem(new IntroduceSystem());
	}
	world->getSystemManager()->initializeAll();


	gameHud = new GameHud(Director::getInstance()->getOpenGLView()->getFrameSize());
	gameHud->setAnchorPoint(Vec2(.5, .5));
	gameHud->buildComponent();
	gameHud->setCallBack([=](Touch* touch, GameHud::EventType event, GameHud::TouchType type) {
		ECSWorld::getInstance()->inputSystem->notifyInput(touch, event, type);
	});
	RenderLayer::getInstance()->getHudLayer()->addChild(gameHud);
	RenderLayer::getInstance()->getHudLayer()->setCameraMask((unsigned short)CameraFlag::USER1);

	ui::Text* text = ui::Text::create("TOUCH TO BEGIN", "fonts/courbd.ttf", 30);
	text->setColor(Color3B::WHITE);
	text->enableOutline(Color4B::BLACK, 2);
	Node* node = RenderLayer::getInstance()->createHudNode();
	node->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	text->setAnchorPoint(Vec2(.5, .5));
	text->setPosition(Size(node->getContentSize().width / 2, node->getContentSize().height / 3));
	auto fadeIn = FadeIn::create(.5);
	auto fadeOut = FadeOut::create(.5);
	text->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, nullptr)));

	node->addChild(text);
	node->setTag(200);
	node->setCameraMask((unsigned short)CameraFlag::USER1);
}

void ECSWorld::resetCurrentMatch(){
	ignoreWorld(false);
	RenderLayer::getInstance()->getHudLayer()->removeAllChildren();
	RenderLayer::getInstance()->getGameLayer()->removeAllChildren();
	createWorld(matchType);
}
void ECSWorld::nextMatch(){
	ignoreWorld(false);
	RenderLayer::getInstance()->getHudLayer()->removeAllChildren();
	RenderLayer::getInstance()->getGameLayer()->removeAllChildren();
	match++;
	int matchIndex = match % 6;
	if (matchIndex == 0){
		createWorld(R::Match_Type::GOKU_TEGIAC);
	}
	else	if (matchIndex == 1){
		createWorld(R::Match_Type::GOKU_CAMAP);
	}
	else	if (matchIndex == 2){
		createWorld(R::Match_Type::GOKU_RUA);
	}
	else	if (matchIndex == 3){
		createWorld(R::Match_Type::GOKU_JACKIECHUN);
	}
	else	if (matchIndex == 4){
		createWorld(R::Match_Type::GOKU_BEAR);
	}
	else	if (matchIndex == 5){
		createWorld(R::Match_Type::GOKU_GIRAN);
	}
}

void ECSWorld::createCameraFollowEntity(){
	artemis::Entity &e = world->createEntity();
	e.setTag("camera");
	e.addComponent(new CameraFollowComponent());
	e.refresh();
}

void ECSWorld::createMainCharacter(){	// create main Character
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/goku.png";
	characterInfo->name = "SONGOKU";
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
	else if (matchType == R::Match_Type::GOKU_BEAR){
		createBearCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_BEAR_INTRODUCE){
		createBearIntroduceCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_JACKIECHUN){
		createJackiechunCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_TEGIAC){
		createTegiacCharacter();
		createEnemyAssistant();
	}

	else if (matchType == R::Match_Type::GOKU_CAMAP){
		createCamapCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_RUA){
		createRuaCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_KARILLIN){
		createKarillinCharacter();
	}
	else if (matchType == R::Match_Type::GOKU_PICOLO){
		createPicoloCharacter();
	}
}

void ECSWorld::createGiranCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/casau.png";
	characterInfo->name = "GIRAN";
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
	decisionComponent->DECISION_TIME = .5;
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


void ECSWorld::createBearCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/bear.png";
	characterInfo->name = "BEAR";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Tuong gau.json",
		"spine/Tuong gau.atlas");
	skeletonAnimation->setAnimation(0, "standing", true);
	skeletonAnimation->setScale(.3);

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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new BearDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Bear();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::BEAR));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-50, 0, 50, 100));
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
void ECSWorld::createBearIntroduceCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/bear.png";
	characterInfo->name = "BEAR";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Tuong gau.json",
		"spine/Tuong gau.atlas");
	skeletonAnimation->setAnimation(0, "standing", true);
	skeletonAnimation->setScale(.3);

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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new BearDecision();
	decisionComponent->decisionBase->setWorld(world);
	decisionComponent->decisionBase->isActive = false;

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Bear();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::BEAR));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-50, 0, 50, 100));
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

void ECSWorld::createJackiechunCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/jackiechun.png";
	characterInfo->name = "JACKIECHUN";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/JackieChun.json",
		"spine/JackieChun.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new JackiechunDecision();
	//decisionComponent->DECISION_TIME = 10000;
	//decisionComponent->decisionBase = new JackiechunDecision2();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Jackiechun();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::JACKIECHUN));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-30, -10, 30, 80));
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


void ECSWorld::createTegiacCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/tegiac.png";
	characterInfo->name = "YAMCHA";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	characterInfo->NORMAL_SKILL_POWER = 2;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Tegiac.json",
		"spine/Tegiac.atlas");
	skeletonAnimation->setAnimation(0, "Prepare", true);
	skeletonAnimation->setScale(.3);

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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new TegiacDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Tegiac();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::TEGIAC));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-30, -10, 30, 80));
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
// con mèo máy hỗ trợ nhân vật
void ECSWorld::createEnemyAssistant(){

	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Meo.json",
		"spine/Meo.atlas");
	skeletonAnimation->setAnimation(0, "stand", true);
	skeletonAnimation->setScale(.3);


	Node* node = RenderLayer::getInstance()->createGameNode();
	node->setAnchorPoint(Vec2(.5, .5));
	node->setContentSize(skeletonAnimation->getContentSize());
	node->addChild(skeletonAnimation);
	node->setVisible(true);
	node->setPosition(Vec2(-1000, 1000));

	SkeletonComponent* characterSkeleton = new SkeletonComponent();
	characterSkeleton->skeleton = skeletonAnimation;
	characterSkeleton->node = node;
	characterSkeleton->isCreated = true;




	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::CAT));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(characterSkeleton);
	character.addComponent(new CatFollowComponent());
	character.setTag("meo");

	character.refresh();


}



void ECSWorld::createRuaCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/rua.png";
	characterInfo->name = "TURTLE";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/rua.json",
		"spine/rua.atlas");
	skeletonAnimation->setAnimation(0, "standing", true);
	skeletonAnimation->setScale(.25);

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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new RuaDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Rua();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::RUA));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-40, 0, 40, 90));
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

void ECSWorld::createCamapCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/camap.png";
	characterInfo->name = "SHARK";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Camap.json",
		"spine/Camap.atlas", .4);
	skeletonAnimation->setAnimation(0, "Stand", true);

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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new CamapDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new CaMap();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::CAMAP));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-30, -10, 30, 80));
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


void ECSWorld::createKarillinCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/khi.png";
	characterInfo->name = "KARILLIN";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Karilin.json",
		"spine/Karilin.atlas");
	skeletonAnimation->setAnimation(0, "Stand", true);
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
	decisionComponent->DECISION_TIME = .4;
	decisionComponent->decisionBase = new KarillinDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Karillin();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::KARILLIN));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-25, -12, 25, 50));
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
void ECSWorld::createPicoloCharacter(){
	CharacterInfoComponent* characterInfo = new CharacterInfoComponent();
	characterInfo->avatar = "textures/doi.png";
	characterInfo->name = "PICOLO";
	characterInfo->MAX_BLOOD = 100;
	characterInfo->MAX_POWER = 100;
	characterInfo->blood = 100;
	characterInfo->power = 100;
	//create keletoncomponent
	spine::SkeletonAnimation* skeletonAnimation =
		spine::SkeletonAnimation::createWithFile("spine/doi.json",
		"spine/doi.atlas", .2f);
	skeletonAnimation->setAnimation(0, "flying", true);


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
	decisionComponent->DECISION_TIME = .5;
	decisionComponent->decisionBase = new PicoloDecision();
	decisionComponent->decisionBase->setWorld(world);

	StateComponent* stateComponent = new StateComponent();
	stateComponent->characterBase = new Picolo();


	artemis::Entity &character = (world->getEntityManager()->create());
	character.addComponent(new CharacterTypeComponent(R::CharacterType::PICOLO));
	character.addComponent(new PosComponent(3 * Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 2));
	character.addComponent(new BoundComponent(-25, 0, 25, 50));
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
	if (isIgnoreWorld) return;
	accumulate += delta;
	while (accumulate > STEP){
		world->setDelta(STEP);
		world->loopStart();
		world->processWorld();
		accumulate -= STEP;
	}
}

void ECSWorld::ignoreWorld(bool ignore){
	this->isIgnoreWorld = ignore;
}
