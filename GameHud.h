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
		TAP, 
		
		LONG_PRESS,

		NONE, 

		LEFT,
					
		RIGHT,
		
		TOP,
		
		BOTTOM,
	
		TOP_LEFT,

		TOP_RIGHT,

		BOTTOM_LEFT,

		BOTTOM_RIGHT

	
	};
	
	GameHud(Size visibleSize);
	~GameHud();

	void buildComponent();
	void notifyEvent();
	void setCallBack(const std::function<void(GameHud::EventType, GameHud::TouchType)> &callBack);
	void setState(HudState state);
	void update(float delta);
	HudState getState();


protected:
	std::function<void(GameHud::EventType, GameHud::TouchType)> callBack;
		
	HudState state;
	
	float timeCatchEvent;
	float timeTouch;
	bool isTouchMove;
	bool ignoreInput;
private :
	TouchType touchType;
	EventType  eventType;
	

};





