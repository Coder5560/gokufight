#include "HelloWorldScene.h"
#include "gokuartemis/ECSWorld.h"
#include "gokuartemis/RenderLayer.h"
#include "renders/Effects.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->setContentSize(Size(R::Constants::MAX_SCREEN_WIDTH,R::Constants::HEIGHT_SCREEN));
	layer->setSwallowsTouches(false);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}
	this->setKeypadEnabled(true);
	this->addChild(RenderLayer::getInstance()->getBackgroundLayer());
	this->addChild(RenderLayer::getInstance()->getGameLayer());
	this->addChild(RenderLayer::getInstance()->getHudLayer());
	auto cameraHud = Camera::create();
	cameraHud->setCameraFlag(CameraFlag::USER1);
	this->addChild(cameraHud);

	ECSWorld::getInstance()->createWorld(R::Match_Type::GOKU_JACKIECHUN);	
	this->scheduleUpdate();

	Sprite* bg = Sprite::create("bg1.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0,0));
	RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);

	// test particle

	
	//smokeParticle->setAutoRemoveOnFinish(true);


	return true;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		Director::getInstance()->end();
	}

	artemis::Entity &e = ECSWorld::getInstance()->getWorld()->getTagManager()->getEntity("enemy");
	StateComponent* stateComponent = (StateComponent*) e.getComponent<StateComponent>();
	SkeletonComponent* skeletonComponent = (SkeletonComponent*)e.getComponent<SkeletonComponent>();
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE){
		CCLOG("Particel");
		CCParticleSystemQuad * smokeParticle = CCParticleSystemQuad::create("textures/particle_texture.plist");
		
		Node* node = RenderLayer::getInstance()->createHudNode();
		node->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
		smokeParticle->setAnchorPoint(Vec2(.5, .5));
		smokeParticle->setPosition(node->getContentSize() / 2);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_BEAT3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_KICK2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_KICK3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_1; }
	if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_AIR; }
	if (keyCode == EventKeyboard::KeyCode::KEY_7){ skeletonComponent->node->setScaleX(1); }
	if (keyCode == EventKeyboard::KeyCode::KEY_8){ skeletonComponent->node->setScaleX(-1); }
}
void HelloWorld::update(float delta) {
	ECSWorld::getInstance()->processWorld(delta);
}

