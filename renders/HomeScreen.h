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
#include "renders/StartScreen.h"
#include "renders/Effects.h"
#include "FacebookManager.h"
#include "CppJavaMethodManager.h"
USING_NS_CC;
class HomeScreen : public cocos2d::LayerColor
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HomeScreen);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual void update(float delta);

	void showStartLayer();
	void onSelectPlayGame();
	void onSelectContinue();
	void switchToSelectScreen();
	void switchToContinueGame();
	void inviteCallback(int result);

	void addCharacterItem(int index, int state, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void addCharacterItem(int index, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void goToGame(R::Match_Type matchType);

	void onSoundClick();
	void onMusicClick();
	void onGuildClick();
	
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
