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
		RELEASE, BEGIN,END
	}; 

	enum TouchType{
		NONE, 

		LEFT,
		LEFT_A,LEFT_X,LEFT_Y,LEFT_B,
			
		RIGHT,
		RIGHT_A,RIGHT_B,RIGHT_X,RIGHT_Y,
		
		UP,
		UP_A,UP_B,UP_X,UP_Y,
		
		DOWN,
		DOWN_A,DOWN_B,DOWN_X,DOWN_Y,

		A,B,X,Y
	};

	GameHud(Size visibleSize);
	~GameHud();
	void buildComponent();

	void notifyEvent();

	void setState(HudState state);
	HudState getState();


protected:
	MenuItemLabel* labelNotify;
	MenuItemImage* knob;
	MenuItemImage* background;

	MenuItemImage* btnA;
	MenuItemImage* btnX;
	MenuItemImage* btnY;
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





