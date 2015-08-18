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


KameKameHa::KameKameHa(Node* node,R::CharacterType whoAttack){
	this->node = node;
	this->whoAttack = whoAttack;
	STATE_TODAN = 1;
	STATE_FLYING = STATE_TODAN + 1;
	STATE_COLLISION = STATE_FLYING + 1;
	timeOnState = 0;
	state = STATE_TODAN;
	timeHit = 0;
	velocity = Vec2::ZERO;
	giatoc = Vec2::ZERO;
	powerOfAttack = 6;
	direction = 1;


	if (whoAttack == R::CharacterType::GOKU){
		hatNhan = Sprite::create("textures/globe.png");
		hieuUng1 = Sprite::create("textures/effect4.png");
		hieuUng2 = Sprite::create("textures/effect4.png");

		hieuUng1->setColor(Color3B::RED);
		hieuUng2->setColor(Color3B::RED);
	}
	else{
		hatNhan = Sprite::create("textures/globe2.png");
		hieuUng1 = Sprite::create("textures/effect4.png");
		hieuUng2 = Sprite::create("textures/effect4.png");

		hieuUng1->setColor(Color3B::BLUE);
		hieuUng2->setColor(Color3B::BLUE);
	}

	hieuUng1->setAnchorPoint(Vec2(.5f, .5f));
	hieuUng2->setAnchorPoint(Vec2(.5f, .5f));
	hieuUng1->runAction(RepeatForever::create(RotateBy::create(.1f, 10)));
	hieuUng2->runAction(RepeatForever::create(RotateBy::create(.1f, -10)));

	ScaleTo* scaleIn = ScaleTo::create(.1, .8f);
	ScaleTo* scaleOut = ScaleTo::create(.1, 1.1f);
	FadeIn* fadeIn = FadeIn::create(.2f);
	FadeOut* fadeOut = FadeOut::create(.2f);
	Spawn* spawn1 = Spawn::create(scaleIn, fadeIn, nullptr);
	Spawn* spawn2 = Spawn::create(scaleOut, fadeOut, nullptr);
	Sequence* sequence = Sequence::create(spawn1, spawn2, nullptr);

	hatNhan->runAction(RepeatForever::create(sequence));
	node->addChild(hieuUng1);
	node->addChild(hieuUng2);
	node->addChild(hatNhan);
	node->setContentSize(hatNhan->getContentSize());
}
void KameKameHa::setTarget(std::string target){
	this->target = target;
}
void KameKameHa::toDan(){
	// do some thing
	velocity.x = 40;
}

void KameKameHa::bayDi(){
	state = STATE_FLYING;
	timeOnState = 0;
	velocity.x = 600;
	giatoc.x = 200;
}

void KameKameHa::bienMat(){
	state = STATE_DISMISS;
	timeOnState = 0;

}

void KameKameHa::update(artemis::World* world){
	float delta = world->getDelta();

	timeOnState += delta;

	if (velocity != Vec2::ZERO){
		velocity.x += giatoc.x*delta;
		velocity.y += giatoc.y*delta;
		node->setPositionX(node->getPositionX() + direction* velocity.x*delta);
		node->setPositionY(node->getPositionY() + direction* velocity.y*delta);
	}
	if (state == STATE_TODAN){
		
	}
	if (state == STATE_TODAN && timeOnState > .5f){
		bayDi();
	}
	if (state == STATE_FLYING){
		artemis::Entity &e = world->getTagManager()->getEntity(target);
		PosComponent* targetPositon = (PosComponent*)e.getComponent<PosComponent>();
		BoundComponent* targetBound = (BoundComponent*)e.getComponent<BoundComponent>();
		if (targetBound && targetPositon){
			Rect targetRect = Rect(targetPositon->x + targetBound->x1, targetPositon->y + targetBound->y1, targetBound->getWidth(), targetBound->getHeight());
			if (targetRect.intersectsCircle(node->getPosition(), 10)){
				StateComponent* targetState = (StateComponent*)e.getComponent<StateComponent>();
				CharacterInfoComponent* targetInfo = (CharacterInfoComponent*)e.getComponent<CharacterInfoComponent>();
				targetInfo->blood -= powerOfAttack;
				targetState->direction = (node->getPositionX() - 10 < targetPositon->x + targetBound->x1) ? R::Direction::RIGHT : R::Direction::LEFT;
				if (targetInfo->blood <= 0){
					targetState->setState(R::CharacterState::DIE);
				}
				else{
					targetState->setState(R::CharacterState::DEFENSE);
					targetState->defense = R::Defense::TRUNG_DON_NGA;
				}
				Node* nodeEffect = RenderLayer::getInstance()->createGameNode();
				nodeEffect->setPosition(node->getPosition());
				HitEffect* hitEffect = new HitEffect(nodeEffect);
				hitEffect->setHitStyle(whoAttack);
				hitEffect->start();
				bienMat();
			}
		}
	}

	if (state == STATE_FLYING && (node->getPositionX() <= 0 || node->getPositionX() >= R::Constants::MAX_SCREEN_WIDTH)){
		bienMat();
	}
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


IntroduceMessage::IntroduceMessage(Node* node)  { this->node = node; }

void IntroduceMessage::start(std::string message){
	node->setAnchorPoint(Vec2(.5, .5));
	node->ignoreAnchorPointForPosition(false);

	ui::Text* text = ui::Text::create(message, "fonts/courbd.ttf", 20);
	text->setColor(Color3B::WHITE);
	text->enableOutline(Color4B::BLACK, 2);
	text->setAnchorPoint(Vec2(.5, .5));
	auto fadeIn = FadeIn::create(.4);
	auto fadeOut = FadeOut::create(.4);
	text->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeOut, nullptr)));
	text->setPosition(Vec2(0,0));
	node->addChild(text);
}


PlayerInfoLeft::PlayerInfoLeft(Node* container, std::string name, std::string avatar) : _name(name), _avatar(avatar){
	this->node = container;
	ui::ImageView* imgAvatar = ui::ImageView::create(_avatar);
	ui::ImageView* board_bg = ui::ImageView::create("textures/board_info.png");
	ui::Text* playerName = ui::Text::create(name, "fonts/courbd.ttf", 16);
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
	ui::Text* playerName = ui::Text::create(name, "fonts/courbd.ttf", 14);
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
	if (_blood <= 0){
		blood->setVisible(false);
	}
	else{
		blood->setVisible(true);
	}

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
	if (_blood <= 0){
		blood->setVisible(false);
	}
	else{
		blood->setVisible(true);
	}

}



PauseScene::PauseScene(Node* node) : isShowing(false) {
	this->node = node;
	node->setAnchorPoint(Vec2(.5f, .5f));
	node->ignoreAnchorPointForPosition(false);
	background = ui::ImageView::create("menu/frame.png");
	background->setAnchorPoint(Vec2(.5f, .5f));
	node->setContentSize(background->getContentSize());



	btnMenu = ui::ImageView::create("menu/btn-menu.png");
	btnMenu->setAnchorPoint(Vec2(.5f, .5f));
	btnNext = ui::ImageView::create("menu/btn-next.png");
	btnNext->setAnchorPoint(Vec2(.5f, .5f));
	btnReplay = ui::ImageView::create("menu/btn-replay.png");
	btnReplay->setAnchorPoint(Vec2(.5f, .5f));
	btnContinue = ui::ImageView::create("menu/btn-continue.png");
	btnContinue->setAnchorPoint(Vec2(.5f, .5f));

	btnMusic = ui::ImageView::create("menu/music_on.png");
	btnMusic->setAnchorPoint(Vec2(.5f, .5f));
	btnSound = ui::ImageView::create("menu/sound_on.png");
	btnSound->setAnchorPoint(Vec2(.5f, .5f));
	btnGuide = ui::ImageView::create("menu/guide.png");
	btnGuide->setAnchorPoint(Vec2(.5f, .5f));


	text = ui::Text::create("YOU WIN !", "fonts/courbd.ttf", 30);
	text->setColor(Color3B::WHITE);
	text->enableOutline(Color4B::BLACK, 2);
	text->setAnchorPoint(Vec2(.5, .5));

}
void PauseScene::setContinueCallBack(const std::function<void()> &callback){
	this->continueCallBack = callback;
}
void PauseScene::setReplayCallback(const std::function<void()> &callback){
	this->replayCallBack = callback;
}
void PauseScene::setMenuCallBack(const std::function<void()> &callback){
	this->menuCallBack = callback;
}
void PauseScene::setNextMatchCallBack(const std::function<void()> &callback){
	this->nextMatchCallBack = callback;
}



void PauseScene::showPauseScene(){
	// continue,  replay,  menu,sound, music, guide

	background->setPosition(node->getContentSize() / 2);
	btnSound->setPosition(Vec2(node->getContentSize().width / 2 - btnSound->getContentSize().width - 20, 30 + btnSound->getContentSize().height));
	btnMusic->setPosition(Vec2(node->getContentSize().width / 2, 30 + btnSound->getContentSize().height));
	btnGuide->setPosition(Vec2(node->getContentSize().width / 2 + btnGuide->getContentSize().width + 20, 30 + btnSound->getContentSize().height));

	btnMenu->setPosition(Vec2(node->getContentSize().width / 2, btnMusic->getPositionY() + btnMusic->getContentSize().height + 30));
	btnReplay->setPosition(Vec2(node->getContentSize().width / 2, btnMenu->getPositionY() + btnMenu->getContentSize().height + 30));
	btnContinue->setPosition(Vec2(node->getContentSize().width / 2, btnReplay->getPositionY() + btnReplay->getContentSize().height + 30));

	node->addChild(background);
	node->addChild(btnSound);
	node->addChild(btnMusic);
	node->addChild(btnGuide);
	node->addChild(btnMenu);
	node->addChild(btnReplay);
	node->addChild(btnContinue);


	btnSound->setTouchEnabled(true);
	btnMusic->setTouchEnabled(true);
	btnGuide->setTouchEnabled(true);
	btnMenu->setTouchEnabled(true);
	btnReplay->setTouchEnabled(true);
	btnContinue->setTouchEnabled(true);

	btnSound->addClickEventListener([=](Ref* sender){
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
		}
		R::Constants::soundEnable = !R::Constants::soundEnable;

		btnSound->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnSound->setTouchEnabled(true);
			btnSound->loadTexture(R::Constants::soundEnable ? "menu/sound_on.png" : "menu/sound_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		btnSound->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnMusic->addClickEventListener([=](Ref* sender){
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(R::Constants::musicVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
		}
		R::Constants::musicEnable = !R::Constants::musicEnable;
		if (R::Constants::musicEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else{
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		btnMusic->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnMusic->setTouchEnabled(true);
			btnMusic->loadTexture(R::Constants::musicEnable ? "menu/music_on.png" : "menu/music_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		btnMusic->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnGuide->addClickEventListener([=](Ref* sender){
		btnGuide->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			btnGuide->setTouchEnabled(true);
		});
		btnGuide->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnContinue->addClickEventListener([=](Ref* sender){
		btnContinue->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnContinue->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (continueCallBack) continueCallBack();
		});
		btnContinue->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});


	btnMenu->addClickEventListener([=](Ref* sender){
		btnMenu->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnMenu->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (menuCallBack) menuCallBack();
		});
		btnMenu->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnReplay->addClickEventListener([=](Ref* sender){
		btnReplay->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnReplay->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (replayCallBack) replayCallBack();
		});
		btnReplay->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
}



Winscene::Winscene(Node* node) : isShowing(false) {
	this->node = node;
	node->setAnchorPoint(Vec2(.5f, .5f));
	node->ignoreAnchorPointForPosition(false);
	background = ui::ImageView::create("menu/frame.png");
	background->setAnchorPoint(Vec2(.5f, .5f));
	node->setContentSize(background->getContentSize());



	btnMenu = ui::ImageView::create("menu/btn-menu.png");
	btnMenu->setAnchorPoint(Vec2(.5f, .5f));
	btnNext = ui::ImageView::create("menu/btn-next.png");
	btnNext->setAnchorPoint(Vec2(.5f, .5f));
	btnReplay = ui::ImageView::create("menu/btn-replay.png");
	btnReplay->setAnchorPoint(Vec2(.5f, .5f));



	text = ui::Text::create("YOU WIN !", "fonts/courbd.ttf", 60);
	text->setColor(Color3B::WHITE);
	text->enableOutline(Color4B::BLACK, 2);
	text->setAnchorPoint(Vec2(.5, .5));


}

void Winscene::setReplayCallback(const std::function<void()> &callback){
	this->replayCallBack = callback;
}
void Winscene::setMenuCallBack(const std::function<void()> &callback){
	this->menuCallBack = callback;
}
void Winscene::setNextMatchCallBack(const std::function<void()> &callback){
	this->nextMatchCallBack = callback;
}



void Winscene::showWinScene(){
	// continue,  replay,  menu,sound, music, guide

	background->setPosition(node->getContentSize() / 2);


	btnMenu->setPosition(Vec2(node->getContentSize().width / 2, 40 + btnMenu->getContentSize().height / 2));
	btnReplay->setPosition(Vec2(node->getContentSize().width / 2, btnMenu->getPositionY() + btnMenu->getContentSize().height + 30));
	btnNext->setPosition(Vec2(node->getContentSize().width / 2, btnReplay->getPositionY() + btnReplay->getContentSize().height + 30));
	text->setPosition(Vec2(node->getContentSize().width / 2, btnNext->getPositionY() + btnNext->getContentSize().height / 2 + 40 + text->getContentSize().height / 2));


	node->addChild(background);
	node->addChild(btnMenu);
	node->addChild(btnReplay);
	node->addChild(btnNext);
	node->addChild(text);




	btnMenu->setTouchEnabled(true);
	btnReplay->setTouchEnabled(true);
	btnNext->setTouchEnabled(true);





	btnMenu->addClickEventListener([=](Ref* sender){
		btnMenu->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnMenu->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (menuCallBack) menuCallBack();
		});
		btnMenu->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnReplay->addClickEventListener([=](Ref* sender){
		btnReplay->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnReplay->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (replayCallBack) replayCallBack();
		});
		btnReplay->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnNext->addClickEventListener([=](Ref* sender){
		btnNext->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnNext->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (nextMatchCallBack) nextMatchCallBack();
		});
		btnNext->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
}




LoseScene::LoseScene(Node* node) : isShowing(false) {
	this->node = node;
	node->setAnchorPoint(Vec2(.5f, .5f));
	node->ignoreAnchorPointForPosition(false);
	background = ui::ImageView::create("menu/frame.png");
	background->setAnchorPoint(Vec2(.5f, .5f));
	node->setContentSize(background->getContentSize() / 1.2f);



	btnMenu = ui::ImageView::create("menu/btn-menu.png");
	btnMenu->setAnchorPoint(Vec2(.5f, .5f));

	btnReplay = ui::ImageView::create("menu/btn-replay.png");
	btnReplay->setAnchorPoint(Vec2(.5f, .5f));



	text = ui::Text::create("YOU LOSE !", "fonts/courbd.ttf", 60);
	text->setColor(Color3B::WHITE);
	text->enableOutline(Color4B::BLACK, 2);
	text->setAnchorPoint(Vec2(.5, .5));


}

void LoseScene::setReplayCallback(const std::function<void()> &callback){
	this->replayCallBack = callback;
}
void LoseScene::setMenuCallBack(const std::function<void()> &callback){
	this->menuCallBack = callback;
}




void LoseScene::showLoseScene(){
	// continue,  replay,  menu,sound, music, guide

	background->setPosition(node->getContentSize() / 2);


	btnMenu->setPosition(Vec2(node->getContentSize().width / 2, 40 + btnMenu->getContentSize().height / 2));
	btnReplay->setPosition(Vec2(node->getContentSize().width / 2, btnMenu->getPositionY() + btnMenu->getContentSize().height + 30));

	text->setPosition(Vec2(node->getContentSize().width / 2, btnReplay->getPositionY() + btnReplay->getContentSize().height / 2 + 40 + text->getContentSize().height / 2));


	node->addChild(background);
	node->addChild(btnMenu);
	node->addChild(btnReplay);
	node->addChild(text);

	btnMenu->setTouchEnabled(true);
	btnReplay->setTouchEnabled(true);

	btnMenu->addClickEventListener([=](Ref* sender){
		btnMenu->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnMenu->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (menuCallBack) menuCallBack();
		});
		btnMenu->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});

	btnReplay->addClickEventListener([=](Ref* sender){
		btnReplay->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* call = CallFunc::create([=](){
			btnReplay->setTouchEnabled(true);
			RemoveSelf* removeSelf = RemoveSelf::create(true);
			node->runAction(removeSelf);
			if (replayCallBack) replayCallBack();
		});
		btnReplay->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
}