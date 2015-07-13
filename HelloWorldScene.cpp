#include "HelloWorldScene.h"
#include "gokuartermis/ECSWorld.h"


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

 



    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->scheduleUpdate();
	
	ECSWorld::getInstance()->createWorld();


	

	Sprite* bg = Sprite::create("textures/bg.png");
	bg->setContentSize(visibleSize);
	bg->setAnchorPoint(Vec2(.5,.5));
	bg->setOpacity(200);
	

	bg->setPosition(visibleSize/2);
	this->addChild(bg);
	sprite = Sprite::create("CloseNormal.png");
	this->addChild(sprite);
	ECSWorld::getInstance()->setSprite(sprite);



	gameHud = new GameHud(Director::getInstance()->getVisibleSize());
	gameHud->setAnchorPoint(Vec2(.5, .5));

	gameHud->buildComponent();
	this->addChild(gameHud);

    return true;
}



void HelloWorld::update(float delta){
	ECSWorld::getInstance()->processWorld(delta);
}

