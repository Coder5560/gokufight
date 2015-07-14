#include "GameHud.h"


GameHud::GameHud(Size visibleSize) : state(HudState::HIDE), knob(nullptr), labelNotify(nullptr), background(nullptr), btnA(nullptr), btnB(nullptr), btnX(nullptr), btnY(nullptr), menu(nullptr), touchPadType(TouchType::NONE), skillType(TouchType::NONE), touchType(TouchType::NONE), eventType(EventType::RELEASE), changeType(false)
{
	this->setContentSize(visibleSize);
}
void GameHud::buildComponent(){
	Size size = getContentSize();
	this->setSwallowsTouches(true);
	labelNotify = MenuItemLabel::create(CCLabelTTF::create("Notify Input", "Marker Felt.ttf", 30, Size(200, 60), TextHAlignment::CENTER, TextVAlignment::CENTER), nullptr);
	labelNotify->setColor(Color3B::BLACK);
	btnA = MenuItemImage::create("btns/a_normal.png", "btns/a_selected.png", [](Ref* sender){

	});
	btnB = MenuItemImage::create("btns/b_normal.png", "btns/b_selected.png", [](Ref* sender){

	});
	btnX = MenuItemImage::create("btns/x_normal.png", "btns/x_selected.png", [](Ref* sender){

	});
	btnY = MenuItemImage::create("btns/y_normal.png", "btns/y_selected.png", [](Ref* sender){

	});

	background = MenuItemImage::create("btns/touch_pad_normal.png", "btns/touch_pad_selected.png", [=](Ref* sender){
	});

	knob = MenuItemImage::create("btns/knob.png", "btns/knob.png", [](Ref* sender){

	});
	knob->setEnabled(false);
	btnA->setEnabled(false);
	btnY->setEnabled(false);
	btnB->setEnabled(false);
	btnX->setEnabled(false);

	EventListenerTouchOneByOne* listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan = [this](Touch* touch, Event* event){
		if (touch->getLocation().distance(this->btnA->getPosition()) < this->btnA->getContentSize().width / 2){
			btnA->selected();
			eventType = EventType::BEGIN;
			skillType = TouchType::A;
			notifyEvent();

			return true;
		}
		return false;
	};

	listener1->onTouchEnded = [this](Touch* touch, Event* event){
		eventType = EventType::END;
		skillType = TouchType::NONE;
		btnA->unselected();
		notifyEvent();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, btnA);

	EventListenerTouchOneByOne* listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = [this](Touch* touch, Event* event){
		if (touch->getLocation().distance(this->btnY->getPosition()) < this->btnY->getContentSize().width / 2){
			btnY->selected();
			eventType = EventType::BEGIN;
			skillType = TouchType::Y;
			notifyEvent();
			return true;
		}
		return false;
	};

	listener2->onTouchEnded = [this](Touch* touch, Event* event){
		btnY->unselected();
		eventType = EventType::END;
		skillType = TouchType::NONE;
		notifyEvent();

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, btnY);



	EventListenerTouchOneByOne* listener3 = EventListenerTouchOneByOne::create();
	listener3->onTouchBegan = [this](Touch* touch, Event* event){
		if (touch->getLocation().distance(this->btnB->getPosition()) < this->btnB->getContentSize().width / 2){
			btnB->selected();
			eventType = EventType::BEGIN;
			skillType = TouchType::B;
			notifyEvent();
			return true;
		}
		return false;
	};

	listener3->onTouchEnded = [this](Touch* touch, Event* event){
		btnB->unselected();
		eventType = EventType::END;
		skillType = TouchType::NONE;
		notifyEvent();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, btnB);


	EventListenerTouchOneByOne* listener4 = EventListenerTouchOneByOne::create();
	listener4->onTouchBegan = [this](Touch* touch, Event* event){
		if (touch->getLocation().distance(this->btnX->getPosition()) < this->btnX->getContentSize().width / 2){
			btnX->selected();
			eventType = EventType::BEGIN;
			skillType = TouchType::X;
			notifyEvent();
			return true;
		}
		return false;
	};

	listener4->onTouchEnded = [this](Touch* touch, Event* event){
		btnX->unselected();
		eventType = EventType::END;
		skillType = TouchType::NONE;
		notifyEvent();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener4, btnX);



	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event){
		float distance = touch->getLocation().distance(this->background->getPosition());
		if (distance < this->background->getContentSize().width / 2){
			knob->setPosition(touch->getLocation());
			if (distance > this->background->getContentSize().width / 6){
				float angle = 180 * atan2f(touch->getLocation().y - this->background->getPosition().y, touch->getLocation().x - this->background->getPosition().x) / 3.14;
				if (angle < 0) angle += 360;
				if (45 <= angle && angle <= 135){
					if (touchPadType != TouchType::UP){
						touchPadType = TouchType::UP;
						eventType = EventType::BEGIN;
						notifyEvent();
					}
				}
				else if (135 <= angle && angle <= 225){
					if (touchPadType != TouchType::LEFT){
						touchPadType = TouchType::LEFT;
						eventType = EventType::BEGIN;
						notifyEvent();
					}
				}
				else if (225 <= angle && angle <= 315){
					if (touchPadType != TouchType::DOWN){
						touchPadType = TouchType::DOWN;
						eventType = EventType::BEGIN;
						notifyEvent();
					}
				}
				else{
					if (touchPadType != TouchType::RIGHT){
						touchPadType = TouchType::RIGHT;
						eventType = EventType::BEGIN;
						notifyEvent();
					}
				}
				eventType = EventType::BEGIN;

			}
			return true;
		}
		return false;
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event){
		float distance = touch->getLocation().distance(this->background->getPosition());
		float r = background->getContentSize().width / 2;

		if (distance > r / 3){
			float angle = 180 * atan2f(touch->getLocation().y - this->background->getPosition().y, touch->getLocation().x - this->background->getPosition().x) / 3.14;
			if (angle < 0) angle += 360;

			if (45 <= angle && angle <= 135){
				if (touchPadType != TouchType::UP){
					touchPadType = TouchType::UP;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
			else if (135 <= angle && angle <= 225){
				if (touchPadType != TouchType::LEFT){
					touchPadType = TouchType::LEFT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
			else if (225 <= angle && angle <= 315){
				if (touchPadType != TouchType::DOWN){
					touchPadType = TouchType::DOWN;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
			else{
				if (touchPadType != TouchType::RIGHT){
					touchPadType = TouchType::RIGHT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
		}
		else if (eventType == EventType::BEGIN){
			eventType = EventType::END;
			notifyEvent();
		}


		if (distance < r){
			knob->setPosition(touch->getLocation());
		}
		else {
			float distance = touch->getLocation().getDistance(background->getPosition());
			float x = background->getPosition().x + (touch->getLocation().x - background->getPosition().x)*(r / distance);
			float y = background->getPosition().y + (touch->getLocation().y - background->getPosition().y)*(r / distance);
			knob->setPosition(Vec2(x, y));
			background->selected();
		}
	};

	listener->onTouchEnded = [this](Touch* touch, Event* event){
		knob->setPosition(background->getPosition());
		background->unselected();
		eventType = EventType::END;
		touchPadType = TouchType::NONE;
		notifyEvent();

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, background);

	background->setAnchorPoint(Vec2(.5, .5));
	knob->setAnchorPoint(Vec2(.5, .5));
	labelNotify->setAnchorPoint(Vec2(.5, .5));
	background->setPosition(Vec2(120, 120));
	knob->setPosition(background->getPosition());
	labelNotify->setPosition(getContentSize() / 2);

	btnA->setPosition(Vec2(size.width - 10 - btnA->getContentSize().width, 60));
	btnB->setPosition(Vec2(size.width - 100 - btnB->getContentSize().width, 40));
	btnX->setPosition(Vec2(size.width - 10 - btnX->getContentSize().width, 160));
	btnY->setPosition(Vec2(size.width - 100 - btnY->getContentSize().width, 140));

	menu = Menu::create(labelNotify, background, knob, btnA, btnB, btnX, btnY, NULL);
	menu->setAnchorPoint(Vec2(.5, .5));
	menu->setPosition(Vec2::ZERO);


	this->addChild(menu, 1);

}
void GameHud::setCallBack(const std::function<void(GameHud::EventType, GameHud::TouchType)> &callBack){
	this->callBack = callBack;
}
void GameHud::notifyEvent(){
	if (callBack){

		std::string str = "No Input";

		if (touchPadType == TouchType::NONE && skillType == TouchType::NONE){
			str = "NO Input";
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::A){
			str = "A";
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::B){
			str = "B";
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::X){
			str = "X";
		}
		else if (touchPadType == TouchType::NONE && skillType == TouchType::Y){
			str = "Y";
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::A){
			str = "A_LEFT";
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::B){
			str = "B_LEFT";
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::X){
			str = "X_LEFT";
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::Y){
			str = "Y_LEFT";
		}
		else if (touchPadType == TouchType::LEFT && skillType == TouchType::NONE){
			str = "LEFT";
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::A){
			str = "A_RIGHT";
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::B){
			str = "B_RIGHT";
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::X){
			str = "X_RIGHT";
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::Y){
			str = "Y_RIGHT";
		}
		else if (touchPadType == TouchType::RIGHT && skillType == TouchType::NONE){
			str = "RIGHT";
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::A){
			str = "A_UP";
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::B){
			str = "B_UP";
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::X){
			str = "X_UP";
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::Y){
			str = "Y_UP";
		}
		else if (touchPadType == TouchType::UP && skillType == TouchType::NONE){
			str = "UP";
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::A){
			str = "A_DOWN";
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::B){
			str = "B_DOWN";
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::X){
			str = "X_DOWN";
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::Y){
			str = "Y_DOWN";
		}
		else if (touchPadType == TouchType::DOWN && skillType == TouchType::NONE){
			str = "DOWN";
		}
		labelNotify->setString(str);

		callBack(EventType::BEGIN, TouchType::UP);
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

