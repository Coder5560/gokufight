#include "HomeScreen.h"

Scene* HomeScreen::createScene() {
	auto scene = Scene::create();
	auto layer = HomeScreen::create();
	scene->addChild(layer);
	return scene;
}
bool HomeScreen::init() {

	if (!LayerColor::initWithColor(Color4B::BLACK)) {
		return false;
	}
	exitEnable = false;
	isShowingExitDialog = false;
	this->showingStartLayer = false;
	this->layoutSelectCreated = false;
	this->setContentSize(Size(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN));
	this->setSwallowsTouches(false);
	this->setKeypadEnabled(true);
	showStartLayer();

	this->scheduleUpdate();
	
	return true;
}

void HomeScreen::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (exitEnable&& keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		if (!isShowingExitDialog) {
				isShowingExitDialog = true;
				dialog = new DialogComfirm();
				dialog->setMessage("Exit game?");
				dialog->setNegative("No", [=]() {
					isShowingExitDialog = false;
				});
				dialog->setPositive("Yes", [=]() {Director::getInstance()->end();
				});
				isShowingExitDialog = true;
			} else {
				if (dialog) {
					dialog->negativeCallback();
				}
			}
	}
}
void HomeScreen::inviteCallback(int result)
{
	cocos2d::log("MONKEYFIGHT invite result = %d", result);
	if (result == 1){
		R::Constants::remaininglife += 3;
		if (R::Constants::remaininglife > R::Constants::MAX_LIFE){
			R::Constants::remaininglife = R::Constants::MAX_LIFE;
		}
		R::Constants::updateVariable();
		auto scene = HomeScreen::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
		return;
	}
	else{
		auto scene = HomeScreen::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
	}

}

void HomeScreen::update(float delta){
	if (showingStartLayer && gokuAnimation){
		if (!gokuAnimation->isVisible()) {
			gokuAnimation->setVisible(true);
			gokuAnimation->setAnimation(0, "Run", true);


			const Vec2 destination = nameGame->getPosition();

			nameGame->setPositionY( - 100 - nameGame->getContentSize().height);
			nameGame->setVisible(true);
			MoveTo* moveTo = MoveTo::create(.4f,destination);
			CallFunc* callBack = CallFunc::create([=](){
				music->setVisible(true);
				music->setOpacity(0);

				guide->setVisible(true);
				guide->setOpacity(0);

				sound->setVisible(true);
				sound->setOpacity(0);

				music->runAction(FadeIn::create(.2f));
				sound->runAction(FadeIn::create(.3f));
				guide->runAction(FadeIn::create(.4f));
			});

			nameGame->runAction(Sequence::create(moveTo, callBack, nullptr));
		}
		float destination = 160;
		float speed = 300;
		if (gokuAnimation->getPositionX() < destination){
			gokuAnimation->setPositionX(gokuAnimation->getPositionX() + speed*delta);
		}
		else{
			this->exitEnable = true;
			if (R::Constants::unlocked>0) buttonContinue->setVisible(true);
			buttonPlay->setVisible(true);
			background->setVisible(true);
			sound->setVisible(true);
			music->setVisible(true);
			guide->setVisible(true);
			nameGame->setVisible(true);
			remainingLife->show();
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(R::Constants::MENU, true);
	}

	background = Sprite::create("backgrounds/bg1.png");
	background->ignoreAnchorPointForPosition(false);
	background->setPositionX(500);
	background->setPositionY(getContentSize().height / 2);
	this->addChild(background);



	showingStartLayer = true;
	layoutPlay = ui::Layout::create();
	layoutPlay->setContentSize(Size(480, 800));
	layoutPlay->setLayoutType(ui::Layout::Type::ABSOLUTE);


	layoutPlay->setScale(getContentSize().width / 480);
	layoutPlay->setAnchorPoint(Vec2(.5, .5));
	layoutPlay->setPosition(getContentSize() / 2);
	containerForLayoutSelect = ui::Layout::create();
	containerForLayoutSelect->setContentSize(getContentSize());
	containerForLayoutSelect->setLayoutType(ui::Layout::Type::ABSOLUTE);
	containerForLayoutSelect->addChild(layoutPlay);

	

	nameGame = Sprite::create("menu/namegame.png");
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

	buttonContinue = ui::ImageView::create("menu/btn-continue.png");
	buttonContinue->setScale9Enabled(true);
	buttonContinue->setScale(.6);
	buttonContinue->ignoreAnchorPointForPosition(false);
	buttonContinue->setPosition(Vec2(350, 350));
	layoutPlay->addChild(buttonContinue);
	buttonContinue->setVisible(false);
	buttonContinue->setTouchEnabled(true);
	buttonContinue->addClickEventListener([=](Ref* sender){
		onSelectContinue();
	});


	music = ui::ImageView::create(R::Constants::musicEnable ? "menu/music_on.png" : "menu/music_off.png");
	music->setScale9Enabled(true);
	music->setScale(.6);
	music->ignoreAnchorPointForPosition(false);
	music->setPosition(Vec2(400, 210));
	music->setTouchEnabled(true);
	music->addClickEventListener([=](Ref* sender){

		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
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
		CallFunc* call = CallFunc::create([=](){
			onMusicClick();
			music->setTouchEnabled(true);
			music->loadTexture(R::Constants::musicEnable ? "menu/music_on.png" : "menu/music_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		music->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
	layoutPlay->addChild(music);

	 sound = ui::ImageView::create(R::Constants::soundEnable ? "menu/sound_on.png" : "menu/sound_off.png");
	sound->setScale9Enabled(true);
	sound->setScale(.6);
	sound->ignoreAnchorPointForPosition(false);
	sound->setPosition(Vec2(400, 150));
	sound->setTouchEnabled(true);
	sound->addClickEventListener([=](Ref* sender){

		if (R::Constants::soundEnable) {
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
		}
		R::Constants::soundEnable = !R::Constants::soundEnable;

		sound->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
		ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
		CallFunc* call = CallFunc::create([=](){
			onSoundClick();
			sound->setTouchEnabled(true);
			sound->loadTexture(R::Constants::soundEnable ? "menu/sound_on.png" : "menu/sound_off.png", ui::ImageView::TextureResType::LOCAL);
		});
		sound->runAction(Sequence::create(scaleIn, scaleout, call, nullptr));
	});
	layoutPlay->addChild(sound);

	 guide = ui::ImageView::create("menu/guide.png");
	guide->setScale9Enabled(true);
	guide->setScale(.6);
	guide->ignoreAnchorPointForPosition(false);
	guide->setPosition(Vec2(400, 90));
	guide->setTouchEnabled(true);
	guide->addClickEventListener([=](Ref* sender){
		guide->setTouchEnabled(false);
		ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
		ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
		CallFunc* call = CallFunc::create([=](){
			if (R::Constants::soundEnable) {
				CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
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


	
	Node* node = Node::create();
	node->setPosition(30, getContentSize().height - 30);
	remainingLife = new RemainingLife(node);
	layoutPlay->addChild(node);



	music->setVisible(false);
	sound->setVisible(false);
	guide->setVisible(false);
	buttonPlay->setVisible(false);
	buttonContinue->setVisible(false);
	gokuAnimation->setVisible(false);
	gokuAnimation->setPositionX(-100);
	nameGame->setVisible(false);
	remainingLife->hide();


	pageView = ui::PageView::create();
	pageView->setContentSize(getContentSize());
	pageView->addPage(containerForLayoutSelect);
	pageView->setUsingCustomScrollThreshold(true);
	pageView->setCustomScrollThreshold(80);
	pageView->setTouchEnabled(false);
	this->addChild(pageView);


}

void HomeScreen::onMusicClick(){

}

void HomeScreen::onSoundClick(){
}

void HomeScreen::onGuildClick(){
	goToGame(R::Match_Type::GOKU_BEAR_INTRODUCE);
}
void HomeScreen::onSelectContinue(){
	if (R::Constants::soundEnable) {
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
	}
	buttonContinue->setTouchEnabled(false);
	ScaleTo* scaleIn = ScaleTo::create(.1f, .56f);
	ScaleTo* scaleout = ScaleTo::create(.1f, .6f);
	CallFunc* callBack = CallFunc::create([=](){
		switchToContinueGame();
		buttonContinue->setTouchEnabled(true);
	});
	buttonContinue->runAction(Sequence::create(scaleIn, scaleout, callBack, nullptr));
}

void HomeScreen::onSelectPlayGame(){
	if (R::Constants::soundEnable) {
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(R::Constants::soundVolumn);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
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
void HomeScreen::switchToContinueGame(){
	// find the last gameplay and play that game
	R::Match_Type type = R::Match_Type::GOKU_BEAR;
	switch (R::Constants::lastPlay)
	{
	case 0:
		type = R::Match_Type::GOKU_BEAR;
		break;
	case 1:
		type = R::Match_Type::GOKU_TEGIAC;
		break;
	case 2:
		type = R::Match_Type::GOKU_KARILLIN;
		break;
	case 3:
		type = R::Match_Type::GOKU_GIRAN;
		break;
	case 4:
		type = R::Match_Type::GOKU_RUA;
		break;
	case 5:
		type = R::Match_Type::GOKU_CAMAP;
		break;
	case 6:
		type = R::Match_Type::GOKU_JACKIECHUN;
		break;
	case 7:
		type = R::Match_Type::GOKU_PICOLO;
		break;
	default:
		break;
	}
	goToGame(type);
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
	addCharacterItem(1, "RHINO", "select_screen/yamcha1.png", "select_screen/yamcha2.png", [=](){
		goToGame(R::Match_Type::GOKU_TEGIAC);
	});
	addCharacterItem(2, "CHIMPANZEE", "select_screen/karillin1.png", "select_screen/karillin2.png", [=](){
		goToGame(R::Match_Type::GOKU_KARILLIN);
	});
	addCharacterItem(3, "LACOSTE", "select_screen/giran1.png", "select_screen/giran2.png", [=](){
			goToGame(R::Match_Type::GOKU_GIRAN);
		});
	addCharacterItem(4, "TURTLE", "select_screen/nam1.png", "select_screen/nam2.png", [=](){
		goToGame(R::Match_Type::GOKU_RUA);
	});
	addCharacterItem(5, "SHARK", "select_screen/shinhan1.png", "select_screen/shinhan2.png", [=](){
		goToGame(R::Match_Type::GOKU_CAMAP);
	});
	addCharacterItem(6, "BLACK MONKEY", "select_screen/jackiechun1.png", "select_screen/jackiechun2.png", [=](){
		goToGame(R::Match_Type::GOKU_JACKIECHUN);
	});
	addCharacterItem(7, "BAT", "select_screen/picolo1.png", "select_screen/picolo2.png", [=](){
		goToGame(R::Match_Type::GOKU_PICOLO);
	});
	layoutSelectCreated = true;
}

void HomeScreen::goToGame(R::Match_Type type){

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

	if(type == R::Match_Type::GOKU_BEAR_INTRODUCE){
		auto scene = HelloWorld::createScene(type);
		Director::getInstance()->replaceScene(
				TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
		return;
	}

	if (R::Constants::remaininglife == 0){
		DialogComfirm* dialogComfirm = new DialogComfirm();
		dialogComfirm->setNegative("More lives", [=](){

			DialogComfirm* subDialog = new DialogComfirm();
			subDialog->setMessage("Invite friends to get more lives", 20);
			subDialog->setNegative("No", [=](){
				auto scene = HomeScreen::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
			});
			subDialog->setPositive("Yes", [=](){
				FacebookManager::inviteFriends(JavaCppBridge::put(CC_CALLBACK_1(HomeScreen::inviteCallback, this)));
//				R::Constants::remaininglife += 3;
//				if (R::Constants::remaininglife > R::Constants::MAX_LIFE){
//					R::Constants::remaininglife = R::Constants::MAX_LIFE;
//				}
//				R::Constants::updateVariable();
//				auto scene = HomeScreen::createScene();
//				Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
			});

		});

		dialogComfirm->setPositive("Reset", [=](){
			DialogComfirm* subDialog = new DialogComfirm();
			subDialog->setMessage("Restart from the first levels with 5 lives", 20);
			subDialog->setNegative("No", [=](){
				auto scene = HomeScreen::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
			});
			subDialog->setPositive("Yes", [=](){
				R::Constants::resetVariable();
				auto scene = HomeScreen::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(0.3, scene, Color3B(0, 0, 0)));
			});
		});
		return;
	}

	R::Constants::updateVariable();
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
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R::Constants::CLICK, false, 1, 0, 1);
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
