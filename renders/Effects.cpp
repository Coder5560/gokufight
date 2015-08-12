#include "Effects.h"


Effects::Effects(){}
Effects::~Effects(){}

HitEffect::HitEffect(Node* node) : hitType(R::CharacterType::NONAME), safeToRemove(false) { this->node = node; }
void HitEffect::setHitStyle(R::CharacterType type){ this->hitType = type; }

void HitEffect::start(){
	node->setAnchorPoint(Vec2(.5, .5));
	node->ignoreAnchorPointForPosition(false);
	spine::SkeletonAnimation* animation = spine::SkeletonAnimation::createWithFile("spine/Trung don.json", "spine/Trung don.atlas", 1);
	if (hitType == R::CharacterType::GOKU){
		animation->setAnimation(0, "trungdon2", false);
	}
	else {
		animation->setAnimation(0, "trungdon", false);
	}
	animation->setTimeScale(.6f);
	animation->setAnchorPoint(Vec2(.5, .5));
	animation->ignoreAnchorPointForPosition(false);
	animation->setCompleteListener([this, animation](int trackIndex, int loopCount){
		dismiss();
	});

	node->addChild(animation);
}
void HitEffect::dismiss(){
	node->setVisible(false);
	RemoveSelf* actionRemove = RemoveSelf::create(true);
	node->runAction(actionRemove);
}


KameKameHa::KameKameHa(Node* node)  { this->node = node; }

void KameKameHa::start(bool isLeftDirection){
	node->setAnchorPoint(Vec2(.5, .5));
	node->ignoreAnchorPointForPosition(false);
	Sprite* sprite = Sprite::create("particles/particle_chuong.png");

	ParticleSystemQuad* chuong = ParticleSystemQuad::create("particles/particle_chuong.plist");
	chuong->setTexture(sprite->getTexture());
	chuong->setAnchorPoint(Vec2(.5, .5));
	chuong->ignoreAnchorPointForPosition(false);
	node->addChild(chuong);

}


NotEnoughManaEffect::NotEnoughManaEffect(Node* node)  { this->node = node; }

void NotEnoughManaEffect::start(){
	node->setAnchorPoint(Vec2(.5, .5));
	node->ignoreAnchorPointForPosition(false);

	ui::Text* text = ui::Text::create("Not enough mana", "fonts/Marker Felt.ttf", 16);
	text->setColor(Color3B::RED);
	text->setAnchorPoint(Vec2(.5, .5));
	text->setScale(0);
	auto scaleOut = ScaleTo::create(.1, 1);
	auto moveBy = MoveBy::create(.4f, Vec3(0, 50, 0));
	auto fadeIn = FadeIn::create(.5);


	auto timeLine = Spawn::create(moveBy, fadeIn, nullptr);
	text->runAction(Sequence::create(scaleOut, timeLine, CallFunc::create([=](){
		dismiss();
	}), nullptr));


	node->addChild(text);
}

void NotEnoughManaEffect::dismiss(){
	node->removeFromParent();
	delete this;
}


Message::Message(Node* node)  { this->node = node; }

void Message::start(std::string message){
	node->setAnchorPoint(Vec2(.5, .5));
	node->ignoreAnchorPointForPosition(false);

	ui::Text* text = ui::Text::create(message, "fonts/Marker Felt.ttf", 20);
	text->setColor(Color3B::RED);
	text->setAnchorPoint(Vec2(.5, .5));
	text->setScale(0);
	auto scaleOut = ScaleTo::create(.1, 1);
	auto moveBy = MoveBy::create(.5f, Vec3(0, 60, 0));
	auto fadeIn = FadeIn::create(.5);


	auto timeLine = Spawn::create(moveBy, fadeIn, nullptr);
	text->runAction(Sequence::create(scaleOut, timeLine, CallFunc::create([=](){
		dismiss();
	}), nullptr));

	node->addChild(text);
}

void Message::dismiss(){
	node->removeFromParent();
	delete this;
}

PlayerInfoLeft::PlayerInfoLeft(Node* container, std::string name, std::string avatar) : _name(name), _avatar(avatar){
	this->node = container;
	ui::ImageView* imgAvatar = ui::ImageView::create(_avatar);
	ui::ImageView* board_bg = ui::ImageView::create("textures/board_info.png");
	ui::Text* playerName = ui::Text::create(name, "fonts/courbd.ttf", 20);
	playerName->setColor(Color3B::BLACK);

	size = Size(100, 6);
	ui::Layout* bgMana = ui::Layout::create();
	bgMana->setContentSize(size);
	bgMana->setBackGroundImageScale9Enabled(true);
	bgMana->setBackGroundImage("textures/column_off.png", ui::Widget::TextureResType::LOCAL);

	mana = ui::Layout::create();
	mana->setContentSize(size);
	mana->setBackGroundImageScale9Enabled(true);
	mana->setBackGroundImage("textures/column_mana.png", ui::Widget::TextureResType::LOCAL);

	ui::Layout* bgBlood = ui::Layout::create();
	bgBlood->setContentSize(size);
	bgBlood->setBackGroundImageScale9Enabled(true);
	bgBlood->setBackGroundImage("textures/column_off.png", ui::Widget::TextureResType::LOCAL);


	blood = ui::Layout::create();
	blood->setContentSize(size);
	blood->setBackGroundImageScale9Enabled(true);
	blood->setBackGroundImage("textures/column_strength.png", ui::Widget::TextureResType::LOCAL);


	node->addChild(board_bg);
	node->addChild(imgAvatar);
	node->addChild(playerName);
	node->addChild(bgMana);
	node->addChild(bgBlood);
	node->addChild(mana);
	node->addChild(blood);



	imgAvatar->setPosition(Vec2(imgAvatar->getContentSize().width / 2, imgAvatar->getContentSize().height / 2 + 20));
	board_bg->setPosition(Vec2(imgAvatar->getContentSize().width + board_bg->getContentSize().width / 2 - 20, board_bg->getContentSize().height / 2));
	playerName->setAnchorPoint(Vec2(0, .5f));
	playerName->setPosition(Vec2(board_bg->getPositionX() - board_bg->getContentSize().width / 2 + 30, board_bg->getPositionY() + 10));

	bgMana->setAnchorPoint(Vec2(0, .5f));
	bgMana->setPosition(Vec2(playerName->getPositionX(), playerName->getPositionY() - playerName->getContentSize().height / 2 - bgMana->getContentSize().height / 2));
	mana->setAnchorPoint(Vec2(0, .5f));
	mana->setPosition(Vec2(bgMana->getPositionX(), bgMana->getPositionY()));

	bgBlood->setAnchorPoint(Vec2(0, .5f));
	bgBlood->setPosition(Vec2(playerName->getPositionX() - 8, bgMana->getPositionY() - bgMana->getContentSize().height / 2 - bgMana->getContentSize().height / 2 - 6));

	blood->setAnchorPoint(Vec2(0, .5f));
	blood->setPosition(Vec2(bgBlood->getPositionX(), bgBlood->getPositionY()));
	blood->setContentSize(Size(size.width / 2, size.height));


}


PlayerInfoRight::PlayerInfoRight(Node* container, std::string name, std::string avatar) : _name(name), _avatar(avatar){
	this->node = container;
	ui::ImageView* imgAvatar = ui::ImageView::create(_avatar);
	ui::ImageView* board_bg = ui::ImageView::create("textures/board_info.png");
	board_bg->setFlipX(-1);
	ui::Text* playerName = ui::Text::create(name, "fonts/courbd.ttf", 20);
	playerName->setColor(Color3B::BLACK);

	size = Size(100, 6);
	ui::Layout* bgMana = ui::Layout::create();
	bgMana->setContentSize(size);
	bgMana->setBackGroundImageScale9Enabled(true);
	bgMana->setBackGroundImage("textures/column_off.png", ui::Widget::TextureResType::LOCAL);

	 mana = ui::Layout::create();
	mana->setContentSize(size);
	mana->setBackGroundImageScale9Enabled(true);
	mana->setBackGroundImage("textures/column_mana.png", ui::Widget::TextureResType::LOCAL);

	ui::Layout* bgBlood = ui::Layout::create();
	bgBlood->setContentSize(size);
	bgBlood->setBackGroundImageScale9Enabled(true);
	bgBlood->setBackGroundImage("textures/column_off.png", ui::Widget::TextureResType::LOCAL);

	blood = ui::Layout::create();
	blood->setContentSize(size);
	blood->setBackGroundImageScale9Enabled(true);
	blood->setBackGroundImage("textures/column_strength.png", ui::Widget::TextureResType::LOCAL);

	node->addChild(board_bg);
	node->addChild(imgAvatar);
	node->addChild(playerName);
	node->addChild(bgMana);
	node->addChild(bgBlood);
	node->addChild(mana);
	node->addChild(blood);

	board_bg->setPosition(Vec2(board_bg->getContentSize().width / 2, board_bg->getContentSize().height / 2));
	imgAvatar->setPosition(Vec2(board_bg->getPositionX() + board_bg->getContentSize().width / 2 + 10, board_bg->getPositionY() + 20));
	playerName->setAnchorPoint(Vec2(0, .5f));
	playerName->setPosition(Vec2(board_bg->getPositionX() - 30, board_bg->getPositionY() + 10));

	bgMana->setAnchorPoint(Vec2(1, .5f));
	bgMana->setPosition(Vec2(board_bg->getPositionX() + 52, playerName->getPositionY() - playerName->getContentSize().height / 2 - bgMana->getContentSize().height / 2));
	mana->setAnchorPoint(Vec2(1, .5f));
	mana->setPosition(Vec2(bgMana->getPositionX(), bgMana->getPositionY()));

	bgBlood->setAnchorPoint(Vec2(1, .5f));
	bgBlood->setPosition(Vec2(board_bg->getPositionX() + 60, bgMana->getPositionY() - bgMana->getContentSize().height / 2 - bgMana->getContentSize().height / 2 - 6));

	blood->setAnchorPoint(Vec2(1, .5f));
	blood->setPosition(Vec2(bgBlood->getPositionX(), bgBlood->getPositionY()));
	blood->setContentSize(Size(size.width / 2, size.height));
}

void PlayerInfoLeft::update(float _blood, float _mana){
	_blood = (_blood<0) ? 0 : ((_blood>1) ? 1 : _blood);
	_mana = (_mana<0) ? 0 : ((_mana>1) ? 1 : _mana);
	blood->setContentSize(Size(size.width * _blood, size.height));
	mana->setContentSize(Size(size.width * _mana, size.height));
}
void PlayerInfoRight::update(float _blood, float _mana){
	_blood = (_blood<0) ? 0 : ((_blood>1) ? 1 : _blood);
	_mana = (_mana<0) ? 0 : ((_mana>1) ? 1 : _mana);
	blood->setContentSize(Size(size.width * _blood, size.height));
	mana->setContentSize(Size(size.width * _mana, size.height));
}