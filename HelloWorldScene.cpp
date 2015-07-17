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
	this->setKeypadEnabled(true);
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


	// giả lập background

	LayerColor* layerColor = LayerColor::create(Color4B::BLACK);
	layerColor->setContentSize(Size(getContentSize().width,210));

	RenderLayer::getInstance()->getBackgroundLayer()->addChild(layerColor,1);








	


    return true;
}


void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK){
		Director::getInstance()->end();
	}

}
void HelloWorld::update(float delta){
	ECSWorld::getInstance()->processWorld(delta);
}

