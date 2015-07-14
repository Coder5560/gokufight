#include "HelloWorldScene.h"
#include "gokuartermis/ECSWorld.h"
#include "gokuartermis/RenderLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
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
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	this->addChild(RenderLayer::getInstance()->getBackgroundLayer());
	this->addChild(RenderLayer::getInstance()->getGameLayer());
	this->addChild(RenderLayer::getInstance()->getHudLayer());



	ECSWorld::getInstance()->createWorld();




    Size visibleSize = Director::getInstance()->getVisibleSize();
   
	this->scheduleUpdate();
	



	Sprite* bg = Sprite::create("textures/bg.png");
	bg->setContentSize(visibleSize);
	bg->setAnchorPoint(Vec2(.5, .5));
	bg->setOpacity(200);
	bg->setPosition(visibleSize / 2);
	RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);




	sprite = Sprite::create("CloseNormal.png");
	RenderLayer::getInstance()->getGameLayer()->addChild(sprite);
	ECSWorld::getInstance()->setSprite(sprite);



	gameHud = new GameHud(Director::getInstance()->getVisibleSize());
	gameHud->setAnchorPoint(Vec2(.5, .5));
	gameHud->buildComponent();
	gameHud->setCallBack([=](GameHud::EventType event, GameHud::TouchType type){
		ECSWorld::getInstance()->inputSystem->notifyInput(event,type);
	});
	RenderLayer::getInstance()->getHudLayer()->addChild(gameHud);

    return true;
}



void HelloWorld::update(float delta){
	ECSWorld::getInstance()->processWorld(delta);
}

