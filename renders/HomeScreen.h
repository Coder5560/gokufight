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
USING_NS_CC;
class HomeScreen : public cocos2d::LayerColor
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HomeScreen);

	virtual void update(float delta);

	void showStartLayer();
	void onSelectPlayGame();
	void switchToSelectScreen();
	void addCharacterItem(int index, int state, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void addCharacterItem(int index, std::string name, std::string unlock, std::string lock, const std::function<void()> &callback);
	void goToGame(R::Match_Type matchType);

	void onSoundClick();
	void onMusicClick();
	void onGuildClick();


	ui::Layout* layoutPlay;
	ui::Layout* layoutSelect;
	ui::PageView* pageView;


	bool showingStartLayer;
	bool layoutSelectCreated;
	spine::SkeletonAnimation* gokuAnimation;
	ui::ImageView* buttonPlay;
};