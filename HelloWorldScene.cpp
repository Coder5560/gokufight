#include "HelloWorldScene.h"
#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/RenderLayer.h"
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

	ECSWorld::getInstance()->createWorld(R::Match_Type::GOKU_GIRAN);	
	this->scheduleUpdate();

	Sprite* bg = Sprite::create("bg1.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0,0));
	RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);


	return true;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		Director::getInstance()->end();
	}
}
void HelloWorld::update(float delta) {
	ECSWorld::getInstance()->processWorld(delta);
}

