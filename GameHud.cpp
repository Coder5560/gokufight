#include "GameHud.h"


GameHud::GameHud(Size visibleSize) : state(HudState::HIDE), knob(nullptr), labelNotify(nullptr), background(nullptr), btnA(nullptr), btnB(nullptr), btnX(nullptr), menu(nullptr), touchPadType(TouchType::NONE), skillType(TouchType::NONE), touchType(TouchType::NONE), eventType(EventType::RELEASE), changeType(false)
{
	this->setContentSize(visibleSize);
}
void GameHud::buildComponent(){
	Size size = getContentSize();
	CCLOG("%f %f", size.width, size.height);


	this->setSwallowsTouches(true);
	labelNotify = MenuItemLabel::create(CCLabelTTF::create("Notify Input", "fonts/Marker Felt.ttf", 30, Size(200, 60), TextHAlignment::CENTER, TextVAlignment::CENTER), nullptr);
	labelNotify->setColor(Color3B::BLACK);
	btnA = MenuItemImage::create("btns/a_normal.png", "btns/a_selected.png", [=](Ref* sender){
		touchPadType = TouchType::NONE;
		skillType = TouchType::A;
		eventType = GameHud::EventType::BEGIN;
		notifyEvent();
		skillType = TouchType::NONE;
	});
	btnB = MenuItemImage::create("btns/b_normal.png", "btns/b_selected.png", [=](Ref* sender){
		touchPadType = TouchType::NONE;
		skillType = TouchType::B;
		eventType = GameHud::EventType::BEGIN;
		notifyEvent();
		skillType = TouchType::NONE;
	});
	btnX = MenuItemImage::create("btns/x_normal.png", "btns/x_selected.png", [=](Ref* sender){
		touchPadType = TouchType::NONE;
		skillType = TouchType::X;
		eventType = GameHud::EventType::BEGIN;
		notifyEvent();
		skillType = TouchType::NONE;
	});
	
	labelNotify->setAnchorPoint(Vec2(.5, .5));
	labelNotify->setPosition(Size(getContentSize().width/2, getContentSize().height-60));


	btnA->setPosition(Vec2(size.width - btnA->getContentSize().width, 80));
	btnA->setAnchorPoint(Vec2(.5,.5));
	btnA->ignoreAnchorPointForPosition(false);
	btnB->setPosition(Vec2(size.width - 60 - btnB->getContentSize().width, 120));
	btnB->setAnchorPoint(Vec2(.5, .5));
	btnB->ignoreAnchorPointForPosition(false);
	btnX->setPosition(Vec2(size.width - 100 - btnB->getContentSize().width, 40));
	btnX->setAnchorPoint(Vec2(.5, .5));
	btnX->ignoreAnchorPointForPosition(false);

	menu = Menu::create(labelNotify,btnA, btnB, btnX, NULL);
	menu->ignoreAnchorPointForPosition(false);
	this->addChild(menu, 1);




	EventListenerTouchOneByOne* listenerMove = EventListenerTouchOneByOne::create();
	listenerMove->onTouchBegan = [this](Touch* touch, Event* event){

		return true;

	};
	listenerMove->onTouchMoved = [this](Touch* touch, Event* event){

		Vec2 delta = touch->getDelta();
		bool horizontal = (abs(delta.x) > abs(delta.y));
		bool left = horizontal&&delta.x<-5;
		bool right = horizontal&&delta.x>5;
		bool up = !horizontal&&delta.y>10;
		bool down = !horizontal&&delta.y<-5;


		if (up){
			if (touchPadType != TouchType::UP){
				touchPadType = TouchType::UP;
				eventType = GameHud::EventType::BEGIN;
				notifyEvent();
			}
		}
		if (left){
			if (touchPadType != TouchType::LEFT){
				touchPadType = TouchType::LEFT;
				eventType = GameHud::EventType::BEGIN;
				notifyEvent();
			}
			else{
				touchPadType = TouchType::LEFT;
				eventType = GameHud::EventType::HOLD;
				notifyEvent();
			}
		}
		else if (down){
			if (touchPadType != TouchType::DOWN){
				touchPadType = TouchType::DOWN;
				eventType = GameHud::EventType::BEGIN;
				notifyEvent();
			}
		}
		else if (right) {
			if (touchPadType != TouchType::RIGHT){
				touchPadType = TouchType::RIGHT;
				eventType = GameHud::EventType::BEGIN;
				notifyEvent();
			}
			else{
				touchPadType = TouchType::RIGHT;
				eventType = GameHud::EventType::HOLD;
				notifyEvent();
			}

		}
	};

	listenerMove->onTouchEnded = [this](Touch* touch, Event* event){
		eventType = GameHud::EventType::END;
		notifyEvent();
		touchPadType = TouchType::NONE;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMove, this);

}
void GameHud::setCallBack(const std::function<void(GameHud::EventType, GameHud::TouchType)> &callBack){
	this->callBack = callBack;
}
void GameHud::notifyEvent(){
	if (callBack){
		std::string str = "No Input";
		TouchType touch = TouchType::NONE;
		if (touchPadType == TouchType::NONE && skillType == TouchType::A){
			str = "A";
			touch = TouchType::A;
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::B){
			str = "B";
			touch = TouchType::B;
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::X){
			str = "X";
			touch = TouchType::X;
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::A){
			str = "A_LEFT";
			touch = TouchType::LEFT_A;
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::B){
			str = "B_LEFT";
			touch = TouchType::LEFT_B;
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::X){
			str = "X_LEFT";
			touch = TouchType::LEFT_X;
		}
	
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::NONE){
			str = "LEFT";
			touch = TouchType::LEFT;
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::A){
			str = "A_RIGHT";
			touch = TouchType::RIGHT_A;
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::B){
			str = "B_RIGHT";
			touch = TouchType::RIGHT_B;
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::X){
			str = "X_RIGHT";
			touch = TouchType::RIGHT_X;
		}
		
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::NONE){
			str = "RIGHT";
			touch = TouchType::RIGHT;
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::A){
			str = "A_UP";
			touch = TouchType::UP_A;
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::B){
			str = "B_UP";
			touch = TouchType::UP_B;
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::X){
			str = "X_UP";
			touch = TouchType::UP_X;
		}
	
		else if (touchPadType == TouchType::UP && skillType == TouchType::NONE){
			str = "UP";
			touch = TouchType::UP;
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::A){
			str = "A_DOWN";
			touch = TouchType::DOWN_A;
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::B){
			str = "B_DOWN";
			touch = TouchType::DOWN_B;
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::X){
			str = "X_DOWN";
			touch = TouchType::DOWN_X;

		}
	
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::NONE){
			str = "DOWN";
			touch = TouchType::DOWN;
		}

		callBack(eventType, touch);
		labelNotify->setString(str);

		eventType = EventType::RELEASE;
	}
}

void GameHud::setState(HudState state){
	this->state = state;
}

GameHud::HudState GameHud::getState(){
	return this->state;
}

GameHud::~GameHud()
{




}

