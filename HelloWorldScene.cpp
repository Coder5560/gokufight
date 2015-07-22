#include "HelloWorldScene.h"
#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/RenderLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

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
	auto layer = LayerColor::create(Color4B::WHITE);
	layer->setContentSize(Size(1600,1000));
	layer->setPositionY(220);





	auto cameraHud = Camera::create();
	cameraHud->setCameraFlag(CameraFlag::USER1);
	this->addChild(cameraHud);
	this->setKeypadEnabled(true);
	this->addChild(RenderLayer::getInstance()->getBackgroundLayer());
	this->addChild(layer);
	this->addChild(RenderLayer::getInstance()->getGameLayer());
	this->addChild(RenderLayer::getInstance()->getHudLayer());

	ECSWorld::getInstance()->createWorld();

	Size visibleSize = Director::getInstance()->getWinSize();

	this->scheduleUpdate();

//	Sprite* bg = Sprite::create("map1.jpg");
//	bg->setAnchorPoint(Vec2(0, 0));
//	bg->setPosition(Vec2(0,210));
//	RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);

	gameHud = new GameHud(visibleSize);
	gameHud->setAnchorPoint(Vec2(.5, .5));
	gameHud->buildComponent();
	gameHud->setCallBack(
			[=](GameHud::EventType event, GameHud::TouchType type) {
				ECSWorld::getInstance()->inputSystem->notifyInput(event,type);
			});
	RenderLayer::getInstance()->getHudLayer()->addChild(gameHud);
	RenderLayer::getInstance()->getHudLayer()->setCameraMask(
			(unsigned short) CameraFlag::USER1);

	// giả lập background

	LayerColor* layerColor = LayerColor::create(Color4B::BLACK);
	layerColor->setContentSize(Size(getContentSize().width, 220));

	RenderLayer::getInstance()->getBackgroundLayer()->addChild(layerColor, 1);
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

