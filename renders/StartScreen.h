#pragma once
#include "cocos2d.h"
#include "R.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "spine/spine-cocos2dx.h"
#include "HelloWorldScene.h"
#include "ui/UIPageView.h"
#include "AdsManager.h"
#include "renders/Effects.h"
#include "FacebookManager.h"
#include "CppJavaMethodManager.h"
USING_NS_CC;

class CharacterItem;
class FacebookManager;

class StartScreen : public cocos2d::LayerColor
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(StartScreen);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual void update(float delta);
	void showFlashImage();
	void showStartLayer();
	void onSelectPlayGame();
	void onSelectContinueGame();
	void switchToSelectScreen();
	void switchToContinueGame();
	void addCharacterItem(int index, int state, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void addCharacterItem(int index, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void goToGame(R::Match_Type matchType);
	
	void onSoundClick();
	void onMusicClick();
	void onGuildClick();

	void inviteCallback(int result);

	ui::Layout* containerForLayoutSelect;
	ui::Layout* layoutPlay;
	ui::Layout* layoutSelect;
	ui::PageView* pageView;


	bool showingStartLayer;
	bool layoutSelectCreated;

	spine::SkeletonAnimation* gokuAnimation;
	ui::ImageView* buttonPlay;
	ui::ImageView* buttonContinue;
	ui::ImageView* sound;
	ui::ImageView* music;
	ui::ImageView* guide;
	Sprite* nameGame;
	Sprite* background;
	RemainingLife* remainingLife;
	bool exitEnable;
	bool isShowingExitDialog;
	DialogComfirm* dialog;
};

class CharacterItem : public ui::Layout{
public:
	CharacterItem(int state, std::string name, std::string textureUnlock, std::string textureLock);
};
