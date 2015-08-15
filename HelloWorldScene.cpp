#include "HelloWorldScene.h"
#include "gokuartemis/ECSWorld.h"
#include "gokuartemis/RenderLayer.h"
#include "renders/Effects.h"

USING_NS_CC;

Scene* HelloWorld::createScene(R::Match_Type type) {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->setContentSize(Size(R::Constants::MAX_SCREEN_WIDTH, R::Constants::HEIGHT_SCREEN));
	layer->setSwallowsTouches(false);
	layer->gotoMatch(type);
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

	return true;
}

void HelloWorld::gotoMatch(R::Match_Type type){
	this->removeAllChildren();
	RenderLayer::getInstance()->resetAll();
	this->addChild(RenderLayer::getInstance()->getBackgroundLayer());
	this->addChild(RenderLayer::getInstance()->getGameLayer());
	this->addChild(RenderLayer::getInstance()->getHudLayer());

	auto cameraHud = Camera::create();
	cameraHud->setCameraFlag(CameraFlag::USER1);
	this->addChild(cameraHud);
	ECSWorld::getInstance()->createWorld(type);
	this->scheduleUpdate();
}


void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		Director::getInstance()->end();
	}

	artemis::Entity &e = ECSWorld::getInstance()->getWorld()->getTagManager()->getEntity("enemy");
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeletonComponent = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	// test Jackichun
	/*if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_BEAT3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_KICK2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_KICK3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_1; }
	if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::JACK_PUNCH_AIR; }
	if (keyCode == EventKeyboard::KeyCode::KEY_7){ skeletonComponent->node->setScaleX(1); }
	if (keyCode == EventKeyboard::KeyCode::KEY_8){ skeletonComponent->node->setScaleX(-1); }
	if (keyCode == EventKeyboard::KeyCode::KEY_9){ CCLOG("State : %d  %d", stateComponent->state,stateComponent->attack); }*/

	//// test tegiac
	//if (keyCode == EventKeyboard::KeyCode::KEY_0){ skeletonComponent->node->setScaleX(-skeletonComponent->node->getScaleX()); }
	//if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_BEAT3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_KICK2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_KICK3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_PUNCH1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_PUNCH2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_PUNCH3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_7){ stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::TEGIAC_PUNCH_AIR; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_8){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::LEFT; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_9){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::RIGHT; }


	//// test rua
	//if (keyCode == EventKeyboard::KeyCode::KEY_0){ skeletonComponent->node->setScaleX(-skeletonComponent->node->getScaleX()); }
	//if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::RUA_ATTACK1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::RUA_ATTACK2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::RUA_ATTACK3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::RUA_ATTACK4; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::RUA_ATTACK5; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_6){}
	//if (keyCode == EventKeyboard::KeyCode::KEY_7){}
	//if (keyCode == EventKeyboard::KeyCode::KEY_8){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::LEFT; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_9){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::RIGHT; }


	//// test camap
	//if (keyCode == EventKeyboard::KeyCode::KEY_0){ skeletonComponent->node->setScaleX(-skeletonComponent->node->getScaleX()); }
	//if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_KICK2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_PUNCH1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_PUNCH2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_PUNCH3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_PUNCH_AIR; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::CAMAP_SKILL; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_7){}
	//if (keyCode == EventKeyboard::KeyCode::KEY_8){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::LEFT; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_9){ stateComponent->setState(R::CharacterState::BACK); stateComponent->direction = R::Direction::RIGHT; }

	////for Karillin
	//if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_BEAT1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_BEAT2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_BEAT3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_KICK1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_KICK2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_KICK3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_7){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_PUNCH1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_8){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::KARILLIN_PUNCH2; }


	////for Picolo
	//if (keyCode == EventKeyboard::KeyCode::KEY_0){ skeletonComponent->node->setScaleX(-skeletonComponent->node->getScaleX()); }
	//if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_ATTACK1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_ATTACK2; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_ATTACK3; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_KICK; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_POWER1; }
	//if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::PICOLO_POWER2; }



	// for GOKU
	/*artemis::Entity &e = ECSWorld::getInstance()->getWorld()->getTagManager()->getEntity("goku");
	StateComponent* stateComponent = (StateComponent*)e.getComponent<StateComponent>();
	SkeletonComponent* skeletonComponent = (SkeletonComponent*)e.getComponent<SkeletonComponent>();

	if (keyCode == EventKeyboard::KeyCode::KEY_0){ skeletonComponent->node->setScaleX(-skeletonComponent->node->getScaleX()); }
	if (keyCode == EventKeyboard::KeyCode::KEY_1){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_BEAT1; }
	if (keyCode == EventKeyboard::KeyCode::KEY_2){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_BEAT2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_3){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_BEAT3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_4){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_KICK1; }
	if (keyCode == EventKeyboard::KeyCode::KEY_5){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_KICK2; }
	if (keyCode == EventKeyboard::KeyCode::KEY_6){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_KICK3; }
	if (keyCode == EventKeyboard::KeyCode::KEY_7){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_PUNCH1; }
	if (keyCode == EventKeyboard::KeyCode::KEY_8){ stateComponent->direction = skeletonComponent->node->getScaleX() < 0 ? R::Direction::LEFT : R::Direction::RIGHT; stateComponent->setState(R::CharacterState::ATTACK); stateComponent->attack = R::Attack::GOKU_PUNCH2; }*/


}
void HelloWorld::update(float delta) {
	ECSWorld::getInstance()->processWorld(delta);
}

