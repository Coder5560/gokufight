#pragma once
#include "cocos2d.h"
#include "ui/UIText.h"
#include "gokuartemis/RenderLayer.h"
#include "R.h"
#include "ui/UIImageView.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "spine/spine-cocos2dx.h"
#include "artemis/Artemis.h"
#include "gokuartemis/Components.h"

USING_NS_CC;
class MatchInfo;
class HitEffect;
class KameKameHa;
class PauseScene;
class RemainingLife;
class DialogComfirm;
class Effects;
class HowToPlay;

	

class Effects{
public:
	Effects();
	~Effects();
	Node* node;
};

class HitEffect : public Effects{
public:
	HitEffect(Node* node);
	void start();
	void dismiss();
	void setHitStyle(R::CharacterType characterType);

	R::CharacterType hitType;
	bool safeToRemove;

};
class NotEnoughManaEffect : public Effects {
public:
	NotEnoughManaEffect(Node* node);
	void start();
	void dismiss();
};

class Message : public Effects {
public:
	Message(Node* node);
	void start(std::string message);
	void dismiss();
};

class IntroduceMessage : public Effects {
public:
	IntroduceMessage(Node* node);
	void start(std::string message);
	void dismiss();
};

class KameKameHa : public Effects{
public:

	KameKameHa(Node* node, R::CharacterType whoAttack);
	void setTarget(std::string target);
	void toDan();
	void bayDi();
	void bienMat();
	void update(artemis::World* world);

	R::CharacterType whoAttack;
	int STATE_TODAN;
	int STATE_DISMISS;
	int STATE_FLYING;
	int STATE_COLLISION;
	float timeOnState;
	Vec2 giatoc;
	int state;

	Sprite* hieuUng1;
	Sprite* hieuUng2;
	Sprite* hatNhan;
	Vec2 velocity;
	int timeHit;
	float powerOfAttack;
	std::string target;
	int direction;
};

class MatchInfo : public Effects{
public:
	MatchInfo(Node* node);
};

class PlayerInfoLeft : public Effects{
public:
	PlayerInfoLeft(Node* node, std::string name, std::string avatar);
	void update(float blood, float mana);
	std::string _name;
	std::string _avatar;
	ui::Layout* mana;
	ui::Layout* blood;
	ui::ImageView* board_bg;
	Size size;
};

class PlayerInfoRight : public Effects{
public:
	PlayerInfoRight(Node* node, std::string name, std::string avatar);
	void update(float blood, float mana);
	std::string _name;
	std::string _avatar;
	ui::Layout* mana;
	ui::Layout* blood;
	Size size;
};

class PauseScene : public Effects{
public:
	PauseScene(Node* node);

	void showPauseScene();
	void setContinueCallBack(const std::function<void()> &callback);
	void setReplayCallback(const std::function<void()> &callback);
	void setMenuCallBack(const std::function<void()> &callback);
	void setNextMatchCallBack(const std::function<void()> &callback);
	void setGuideCallBack(const std::function<void()> &callback);
	std::function<void()> continueCallBack;
	std::function<void()> menuCallBack;
	std::function<void()> replayCallBack;
	std::function<void()> nextMatchCallBack;
	std::function<void()> guideCallBack;

	bool isShowing;

	ui::ImageView* background;
	ui::ImageView* btnMenu;
	ui::ImageView* btnNext;
	ui::ImageView* btnReplay;
	ui::ImageView* btnContinue;
	ui::ImageView* btnMusic;
	ui::ImageView* btnSound;
	ui::ImageView* btnGuide;

	ui::Text* text;
};


class Winscene : public Effects{
public:
	Winscene(Node* node);

	void showWinScene();

	void setReplayCallback(const std::function<void()> &callback);
	void setMenuCallBack(const std::function<void()> &callback);
	void setNextMatchCallBack(const std::function<void()> &callback);
	void setShareCallBack(const std::function<void()> &callback);
	void setRateCallBack(const std::function<void()> &callback);

	std::function<void()> menuCallBack;
	std::function<void()> replayCallBack;
	std::function<void()> nextMatchCallBack;
	std::function<void()> shareCallBack;
	std::function<void()> rateCallBack;

	bool isShowing;


	ui::ImageView* background;
	ui::ImageView* btnMenu;
	ui::ImageView* btnNext;
	ui::ImageView* btnReplay;
	ui::ImageView* btnShare;
	ui::ImageView* btnRate;

	ui::Text* text;
};


class LoseScene : public Effects{
public:
	LoseScene(Node* node);

	void showLoseScene();

	void setReplayCallback(const std::function<void()> &callback);
	void setMenuCallBack(const std::function<void()> &callback); 
	void setShareCallBack(const std::function<void()> &callback);
	void setRateCallBack(const std::function<void()> &callback);

	std::function<void()> menuCallBack;
	std::function<void()> replayCallBack;
	std::function<void()> shareCallBack;
	std::function<void()> rateCallBack;
	bool isShowing;


	ui::ImageView* background;
	ui::ImageView* btnMenu;
	ui::ImageView* btnReplay;
	ui::ImageView* btnShare;
	ui::ImageView* btnRate;
	ui::Text* text;
};

class TextEffect : public Effects{


};

class RemainingLife : public Effects{

public:
	RemainingLife(Node* node);
	~RemainingLife();
	void show();
	void hide();
	void update();
	void updatePosition();
	Node* node;
	std::vector<ui::ImageView*> hearts;
	ui::Text* text;
};

class DialogComfirm : public Effects{

public: 
	DialogComfirm();
	void setPositive(std::string text,const std::function<void()> &callback);
	void setNegative(std::string text,const std::function<void()> &callback);
	void setMessage(std::string message);
	void setMessage(std::string message,int size);

	std::function<void()> negativeCallback;
	std::function<void()> positiveCallback;

	ui::Layout* layout;
	ui::Button* negative;
	ui::Button* positive;
	ui::Text* text;
};

class DialogComfirmInGame : public Effects{

public:
	DialogComfirmInGame();
	void setPositive(std::string text, const std::function<void()> &callback);
	void setNegative(std::string text, const std::function<void()> &callback);
	void setMessage(std::string message);
	void setMessage(std::string message, int size);

	std::function<void()> negativeCallback;
	std::function<void()> positiveCallback;

	ui::Layout* layout;
	ui::Button* negative;
	ui::Button* positive;
	ui::Text* text;

};


class HowToPlay :public Effects{
public :
	HowToPlay();

	int step;
	void reset();
	void showTap(const std::function<void()> &callback);
	void showDoubleClick(const std::function<void()> &callback);
	void showSwipeLeft(const std::function<void()> &callback);
	void showSwipeUp(const std::function<void()> &callback);
	void showSwipeRight(const std::function<void()> &callback);
	void showSwipeTopRight(const std::function<void()> &callback);
	void showSwipeTopleft(const std::function<void()> &callback);
	void showLongPress(const std::function<void()> &callback);

	std::function<void()> tapCallback;
	std::function<void()> doubleTapCallback;
	std::function<void()> swpieLeftCallback;
	std::function<void()> swipeRightCallback;
	std::function<void()> swipeTopRightCallback;
	std::function<void()> swipeTopLeftCallback;
	std::function<void()> swipeUpCallback;
	std::function<void()> longpressCallback;



	ui::Layout* layout;
	ui::ImageView* arrow;	
};


