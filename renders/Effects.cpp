#include "Effects.h"


Effects::Effects(){}
Effects::~Effects(){}

HitEffect::HitEffect(Node* node) : hitType(R::CharacterType::NONAME) { this->node = node; }
void HitEffect::setHitStyle(R::CharacterType type){ this->hitType = type; }
void HitEffect::start(){
	node->setAnchorPoint(Vec2(.5,.5));
	node->ignoreAnchorPointForPosition(false);
	

	Sprite* sprite = Sprite::create();
	node->addChild(sprite);
	Vector<SpriteFrame*> animFrames(4);
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		if (hitType == R::CharacterType::GOKU){ sprintf(str, "textures/hit%02d.png", i); }
		else{
			sprintf(str, "textures/enemyhit%02d.png", i);
		}
		
		auto frame = SpriteFrame::create(str, Rect(0, 0, 60, 60)); //we assume that the sprites' dimentions are 40*40 rectangles.
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	auto animate = Animate::create(animation);
	auto sequence = Sequence::create(animate, CallFunc::create([this](){
		node->removeFromParent();
		delete this;
	}), nullptr);
	sprite->runAction(sequence);
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




	/*Sprite* sprite = Sprite::create("textures/effect_chuong.png"); 
	sprite->setScale(0);
	auto scaleOutAction = ScaleTo::create(.5,1);
	auto scaleOut = EaseBackOut::create(scaleOutAction->clone());
	sprite->runAction(scaleOut);
	node->addChild(sprite);*/
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


	auto timeLine = Spawn::create(moveBy,fadeIn,nullptr);
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