#include "RenderLayer.h"
#include "R.h"

RenderLayer* instance = nullptr;

RenderLayer::RenderLayer()
{
	visibleSize = Director::getInstance()->getWinSize();
	backgroundLayer = Layer::create();
	backgroundLayer->setContentSize(visibleSize);
	gameLayer = Layer::create();
	gameLayer->setContentSize(Size(R::Constants::MAX_SCREEN_WIDTH,R::Constants::HEIGHT_SCREEN));
	gameLayer->ignoreAnchorPointForPosition(false);
	gameLayer->setScale(2);
	gameLayer->setPositionX(R::Constants::MAX_SCREEN_WIDTH);
	gameLayer->setPositionY(R::Constants::HEIGHT_SCREEN);
	hudLayer = Layer::create();
	hudLayer->setContentSize(visibleSize);
}


RenderLayer::~RenderLayer(){}

RenderLayer* RenderLayer::getInstance(){
	if (instance == nullptr){
		instance = new RenderLayer();
	}
	return instance;
}
Node* RenderLayer::createHudNode(){
	Node* node = Node::create();
	hudLayer->addChild(node);
	return node;
}

Node* RenderLayer::createGameNode(){
	Node* node = Node::create();
	gameLayer->addChild(node);
	return node;
}

Node* RenderLayer::createBackgroundNode(){
	Node* node = Node::create();
	backgroundLayer->addChild(node);
	return node;
}

Layer* RenderLayer::getBackgroundLayer(){
	return backgroundLayer;
}
Layer* RenderLayer::getGameLayer(){
	return gameLayer;
}
Layer* RenderLayer::getHudLayer(){
	return hudLayer;
}


void RenderLayer::resetAll(){
	visibleSize = Director::getInstance()->getWinSize();
	backgroundLayer = Layer::create();
	backgroundLayer->setContentSize(visibleSize);
	gameLayer = Layer::create();
	gameLayer->setContentSize(Size(R::Constants::MAX_SCREEN_WIDTH, R::Constants::HEIGHT_SCREEN));
	gameLayer->ignoreAnchorPointForPosition(false);
	gameLayer->setScale(2);
	gameLayer->setPositionX(R::Constants::MAX_SCREEN_WIDTH);
	gameLayer->setPositionY(R::Constants::HEIGHT_SCREEN);
	hudLayer = Layer::create();
	hudLayer->setContentSize(visibleSize);
}