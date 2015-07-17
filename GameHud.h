#pragma once
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIImageView.h"

USING_NS_CC;



class GameHud : public cocos2d::Layer
{
public:
	enum HudState{
		SHOW, HIDE, ANIMATING, PRESSING
	};

	enum EventType{
		RELEASE, BEGIN,HOLD,END
	}; 

	enum TouchType{
		NONE, 

		LEFT,
		LEFT_A,LEFT_X,LEFT_B,
			
		RIGHT,
		RIGHT_A,RIGHT_B,RIGHT_X,

		UP,
		UP_A,UP_B,UP_X,
		
		DOWN,
		DOWN_A,DOWN_B,DOWN_X,

		A,B,X
	};
	
	GameHud(Size visibleSize);
	~GameHud();

	void buildComponent();
	void notifyEvent();
	void setCallBack(const std::function<void(GameHud::EventType, GameHud::TouchType)> &callBack);
	void setState(HudState state);
	HudState getState();


protected:
	std::function<void(GameHud::EventType, GameHud::TouchType)> callBack;
	MenuItemLabel* labelNotify;
	MenuItemImage* knob;
	MenuItemImage* background;

	MenuItemImage* btnA;
	MenuItemImage* btnX;
	MenuItemImage* btnB;

	Menu* menu;
	
	HudState state;

private :
	TouchType touchPadType;
	TouchType skillType;
	TouchType touchType;
	EventType  eventType;
	bool changeType ;

};





