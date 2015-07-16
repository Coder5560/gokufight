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
	if (!LayerColor::initWithColor(Color4B::WHITE))
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
	//RenderLayer::getInstance()->getBackgroundLayer()->addChild(bg);

		
	gameHud = new GameHud(visibleSize);
	gameHud->setAnchorPoint(Vec2(.5, .5));
	gameHud->buildComponent();
	gameHud->setCallBack([=](GameHud::EventType event, GameHud::TouchType type){
		ECSWorld::getInstance()->inputSystem->notifyInput(event,type);
	});
	RenderLayer::getInstance()->getHudLayer()->addChild(gameHud);




	/*skeletonNode = spine::SkeletonAnimation::createWithFile("spine/skeleton.json", "spine/skeleton.atlas", 1.5f);
	skeletonNode->setAnimation(0, "stand", true);
	skeletonNode->setSkin("goku");

	skeletonNode->setScale(0.5);
	Size windowSize = Director::getInstance()->getWinSize();
	skeletonNode->setPosition(Vec2(windowSize.width / 2, 20));
	addChild(skeletonNode);

	scheduleUpdate();

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		if (!skeletonNode->getDebugBonesEnabled())
			skeletonNode->setDebugBonesEnabled(true);
		else if (skeletonNode->getTimeScale() == 1)
			skeletonNode->setTimeScale(0.3f);
		else
		{
			skeletonNode->setTimeScale(1);
			skeletonNode->setDebugBonesEnabled(false);
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/







	


    return true;
}



void HelloWorld::update(float delta){
	ECSWorld::getInstance()->processWorld(delta);
}

