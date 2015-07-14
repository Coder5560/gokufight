#include "RenderLayer.h"


RenderLayer* instance = nullptr;

RenderLayer::RenderLayer()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	backgroundLayer = Layer::create();
	backgroundLayer->setContentSize(visibleSize);
	gameLayer = Layer::create();
	gameLayer->setContentSize(visibleSize);
	hudLayer = Layer::create();
	hudLayer->setContentSize(visibleSize);

}


RenderLayer::~RenderLayer()
{
}

RenderLayer* RenderLayer::getInstance(){
	if (instance == nullptr){
		instance = new RenderLayer();
	}
	return instance;
}
Node* RenderLayer::createHudNode(){
	Node* node = Node::create();
	node->setContentSize(visibleSize);
	hudLayer->addChild(node);
	return node;
}

Node* RenderLayer::createGameNode(){
	Node* node = Node::create();
	node->setContentSize(visibleSize);
	gameLayer->addChild(node);
	return node;
}

Node* RenderLayer::createBackgroundNode(){
	Node* node = Node::create();
	node->setContentSize(visibleSize);
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


