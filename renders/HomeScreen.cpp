#include "HomeScreen.h"



Scene* HomeScreen::createScene() {
	auto scene = Scene::create();
	auto layer = HomeScreen::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool HomeScreen::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B::BLACK)) {
		return false;
	}



	this->showingStartLayer = false;
	this->layoutSelectCreated = false;
	this->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	this->setSwallowsTouches(false);
	showStartLayer();

	this->scheduleUpdate();
	
	return true;
}
void HomeScreen::update(float delta){
	if (showingStartLayer && gokuAnimation){
		if (!gokuAnimation->isVisible()) {
			gokuAnimation->setVisible(true);
			gokuAnimation->setAnimation(0, "Run", true);
		}
		float destination = 160;
		float speed = 300;
		if (gokuAnimation->getPositionX() < destination){
			gokuAnimation->setPositionX(gokuAnimation->getPositionX() + speed*delta);
		}
		else{
			buttonPlay->setVisible(true);
			gokuAnimation->setPositionX(destination);
			gokuAnimation->setToSetupPose();
			gokuAnimation->setAnimation(0, "Stand", true);
			showingStartLayer = false;
		}
	}
}



void HomeScreen::showStartLayer(){
	if (R::Constants::musicEnable) {
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(R::Constants::musicVolumn);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/menu.mp3", true);
	}

	Sprite* background = Sprite::create("backgrounds/bg1.png");
	background->ignoreAnchorPointForPosition(false);
	background->setPositionX(500);
	background->setPositionY(getContentSize().height / 2);
	this->addChild(background);



	showingStartLayer = true;
	layoutPlay = ui::Layout::create();
	layoutPlay->setContentSize(getContentSize());
	layoutPlay->setLayoutType(ui::Layout::Type::ABSOLUTE);


	pageView = ui::PageView::create();
	pageView->setContentSize(getContentSize());
	pageView->addPage(layoutPlay);
	pageView->setUsingCustomScrollThreshold(true);
	pageView->setCustomScrollThreshold(80);
	pageView->setTouchEnabled(false);
	this->addChild(pageView);


	Sprite* nameGame = Sprite::create("menu/namegame.png");
	nameGame->ignoreAnchorPointForPosition(false);
	nameGame->setScale(.6f);
	nameGame->setPosition(Vec2(200, 150));
	layoutPlay->addChild(nameGame);

	buttonPlay = ui::ImageView::create("menu/button_play.png");
	buttonPlay->setScale9Enabled(true);
	buttonPlay->setScale(.6);
	buttonPlay->ignoreAnchorPointForPosition(false);
	buttonPlay->setPosition(Vec2(330, 290));
	layoutPlay->addChild(buttonPlay);
	buttonPlay->setTouchEnabled(true);

	buttonPlay->addClickEventListener([=](Ref* sender){
		onSelectPlayGame();
	});

	ui::ImageView* music = ui::ImageView::create(R::Constants::musicEnable ? "menu/music_on.png" : "menu/music_off.png");
	music->setScale9Enabled(true);
	music->setScale(.6);
	music->ignoreAnchorPointForPosition(false);
	music->setPosition(Vec2(400, 210));
	music->setTouchEnabled(true);
	music->addClickEventListener([this, music](Ref* sender){

		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
		}
		R::Constants::musicEnable = !R::Constants::musicEnable;

		if (!R::Constants::musicEnable){
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		else{
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}

		music->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
		ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
		CallFunc* call = CallFunc::create([this, music](){
			onMusicClick();
			music->setTouchEnabled(true);
			music->loadTexture(R::Constants::musicEnable ? "menu/music_on.png" : "menu/music_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		music->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
	layoutPlay->addChild(music);

	ui::ImageView* sound = ui::ImageView::create(R::Constants::soundEnable ? "menu/sound_on.png" : "menu/sound_off.png");
	sound->setScale9Enabled(true);
	sound->setScale(.6);
	sound->ignoreAnchorPointForPosition(false);
	sound->setPosition(Vec2(400, 150));
	sound->setTouchEnabled(true);
	sound->addClickEventListener([this, sound](Ref* sender){

		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
		}
		R::Constants::soundEnable = !R::Constants::soundEnable;

		sound->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
		ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
		CallFunc* call = CallFunc::create([this, sound](){
			onSoundClick();
			sound->setTouchEnabled(true);
			sound->loadTexture(R::Constants::soundEnable ? "menu/sound_on.png" : "menu/sound_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		sound->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
	layoutPlay->addChild(sound);

	ui::ImageView* guide = ui::ImageView::create("menu/guide.png");
	guide->setScale9Enabled(true);
	guide->setScale(.6);
	guide->ignoreAnchorPointForPosition(false);
	guide->setPosition(Vec2(400, 90));
	guide->setTouchEnabled(true);
	guide->addClickEventListener([this, guide](Ref* sender){
		guide->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
		ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
		CallFunc* call = CallFunc::create([this, guide](){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
			}
			onGuildClick();
			guide->setTouchEnabled(true);
		});
		guide->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
	layoutPlay->addChild(guide);

	gokuAnimation =
		spine::SkeletonAnimation::createWithFile("spine/Goku.json",
		"spine/Goku.atlas");
	gokuAnimation->setAnimation(0, "Stand", true);
	gokuAnimation->setSkin("goku");
	gokuAnimation->setScale(1);
	gokuAnimation->setPosition(Vec2(160, 290));
	layoutPlay->addChild(gokuAnimation);

	buttonPlay->setVisible(false);
	gokuAnimation->setVisible(false);
	gokuAnimation->setPositionX(-100);
}

void HomeScreen::onMusicClick(){
}

void HomeScreen::onSoundClick(){
}

void HomeScreen::onGuildClick(){
}

void HomeScreen::onSelectPlayGame(){
	if (R::Constants::soundEnable) {
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
	}
	buttonPlay->setTouchEnabled(false);
	ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
	ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
	CallFunc* callBack = CallFunc::create([=](){
		switchToSelectScreen();
		buttonPlay->setTouchEnabled(true);
	});
	buttonPlay->runAction(Sequence::create(scaleIn, scaleout, callBack, nullptr));
}

void HomeScreen::switchToSelectScreen(){
	/*auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
	return;*/

	if (layoutSelectCreated){
		pageView->scrollToPage(1);
		return;
	}



	layoutSelect = ui::Layout::create();
	layoutSelect->setContentSize(getContentSize());
	layoutSelect->setLayoutType(ui::Layout::Type::ABSOLUTE);

	pageView->addPage(layoutSelect);
	pageView->scrollToPage(1);
	pageView->setTouchEnabled(true);



	ui::Text* text = ui::Text::create("UNLOCK ENEMIES", "fonts/courbd.ttf", 40);
	text->enableOutline(Color4B::BLACK, 2);
	text->setPosition(Vec2(getContentSize().width / 2, getContentSize().height - 100));
	layoutSelect->addChild(text);



	addCharacterItem(0, "BEAR", "select_screen/bear1.png", "select_screen/bear2.png", [=](){
		goToGame(R::Match_Type::GOKU_BEAR);
	});
	addCharacterItem(1, "YAMCHA", "select_screen/yamcha1.png", "select_screen/yamcha2.png", [=](){
		goToGame(R::Match_Type::GOKU_TEGIAC);
	});
	addCharacterItem(2, "KARILLIN", "select_screen/karillin1.png", "select_screen/karillin2.png", [=](){
		goToGame(R::Match_Type::GOKU_KARILLIN);
	});
	addCharacterItem(3, "GIRAN", "select_screen/giran1.png", "select_screen/giran2.png", [=](){
		goToGame(R::Match_Type::GOKU_GIRAN);
	});
	addCharacterItem(4, "NAM", "select_screen/nam1.png", "select_screen/nam2.png", [=](){
		goToGame(R::Match_Type::GOKU_RUA);
	});
	addCharacterItem(5, "SHINHAN", "select_screen/shinhan1.png", "select_screen/shinhan2.png", [=](){
		goToGame(R::Match_Type::GOKU_CAMAP);
	});
	addCharacterItem(6, "JACKIECHUN", "select_screen/jackiechun1.png", "select_screen/jackiechun2.png", [=](){
		goToGame(R::Match_Type::GOKU_JACKIECHUN);
	});
	addCharacterItem(7, "PICOLO", "select_screen/picolo1.png", "select_screen/picolo2.png", [=](){
		goToGame(R::Match_Type::GOKU_PICOLO);
	});
	layoutSelectCreated = true;
}

void HomeScreen::goToGame(R::Match_Type type){
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	auto scene = HelloWorld::createScene(type);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
	return;
}

// state = 0 : lock
// state = 1 : to unlock
// state = 2 : unlocked
void HomeScreen::addCharacterItem(int index, int state, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback){

	CharacterItem* character = new CharacterItem(state, name, unlock, lock);

	character->setTouchEnabled(state != 0);
	character->addClickEventListener([character, callback](Ref* sender){
		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/button_click.mp3", false, 1, 0, 1);
		}
		character->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .96f);
		ScaleTo* scaleout = ScaleTo::create(.1f, 1);
		CallFunc* resetTouch = CallFunc::create([character](){
			character->setTouchEnabled(true);
		});
		CallFunc* call = CallFunc::create(callback);
		character->runAction(Sequence::create(scaleIn, scaleout, call, resetTouch, nullptr));
	});
	layoutSelect->addChild(character);

	float centerX = getContentSize().width / 2;
	float centerY = 3 * getContentSize().height / 4 - 60;
	character->setPosition(Vec2(centerX + ((index % 2 == 0) ? (-120) : 120), centerY - (index / 2) * 120));

}


// state = 0 : lock
// state = 1 : to unlock
// state = 2 : unlocked
void HomeScreen::addCharacterItem(int index, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback){
	int state = 0;
	if (index < R::Constants::unlocked) state = 2;
	else if (index == R::Constants::unlocked) state = 1;
	else state = 0;
	addCharacterItem(index, state, name, unlock, lock, callback);
}
