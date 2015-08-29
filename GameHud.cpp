#include "GameHud.h"

GameHud::GameHud(Size visibleSize) :touch(nullptr), state(HudState::HIDE), touchType(TouchType::NONE), eventType(EventType::RELEASE), timeTouch(0), isPan(false), ignoreInput(false), timeCatchEvent(0) {
	this->setContentSize(visibleSize);
	this->scheduleUpdate();
}
void GameHud::buildComponent() {
	Size size = getContentSize();
	this->setSwallowsTouches(true);

	EventListenerTouchOneByOne* listenerMove =	EventListenerTouchOneByOne::create();
	listenerMove->onTouchBegan = [this](Touch* touchElement, Event* event) {
		timeTouch = 0;
		timeCatchEvent = -1;
		isFling = false;
		isPan = false;
		ignoreInput = false;
		eventType = EventType::BEGIN;
		touchType = TouchType::NONE;
		this->touch = touchElement;
		return true;

	};
	listenerMove->onTouchMoved =
		[this](Touch* touch, Event* event) {
		this->touch = touch;
		if (ignoreInput) return;
		Vec2 delta = touch->getDelta();
		float length = touch->getDelta().getLength();
		float angle = 180 * atan2f(touch->getDelta().y, touch->getDelta().x) / M_PI;
		if (timeTouch<.2 && length >60){
			if (!isFling){
				CCLOG("Fling");

				if (135 <= angle && angle <= 225) {
					touchType = TouchType::TOP_LEFT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
				else if ((315 <= angle && angle < 360) || angle < 45) {
					touchType = TouchType::TOP_RIGHT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
				ignoreInput = true;
				isFling = true;

			}
		}
		if (isFling){
			return;
		}


		if (angle < 0) angle += 360;
		if (isPan && length>4) {
			if (157 <= angle && angle <= 202) {
				if (touchType == TouchType::RIGHT) {
					touchType = TouchType::LEFT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
			else if ((337 <= angle && angle < 360) || angle < 22) {
				if (touchType == TouchType::LEFT) {
					touchType = TouchType::RIGHT;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
			else if (45 <= angle && angle <= 135 && length >10) {
				if (touchType != TouchType::TOP) {
					touchType = TouchType::TOP;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
		}

		if (!isPan && length > 6) {
			if (22 <= angle && angle <= 80) {
				if (length > 20) {
					touchType = TOP_RIGHT;
				}
				else {
					touchType = RIGHT;
				}
			}
			else if (80 <= angle && angle <= 110) {
				touchType = TouchType::TOP;

			}
			else if (112 <= angle && angle <= 157) {
				if (length > 20) {
					touchType = TOP_LEFT;
				}
				else {
					touchType = LEFT;
				}
			}
			else if (157 <= angle && angle <= 202) {
				touchType = TouchType::LEFT;
			}
			else if (202 <= angle && angle <= 247) {
				touchType = TouchType::BOTTOM_LEFT;
			}
			else if (247 <= angle && angle <= 292) {
				touchType = TouchType::BOTTOM;
			}
			else if (292 <= angle && angle <= 337) {
				touchType = TouchType::BOTTOM_RIGHT;
			}
			else {
				touchType = TouchType::RIGHT;
			}

			isPan = true;
			timeCatchEvent = timeTouch;
			eventType = GameHud::EventType::BEGIN;
			notifyEvent();
		}
	};

	listenerMove->onTouchEnded = [this](Touch* touch, Event* event) {
		this->touch = touch;
		if (ignoreInput) return;
		if (!isPan) {

			if (timeTouch < .2) {
				//notify tap
				eventType = GameHud::EventType::BEGIN;
				touchType = TouchType::TAP;
				notifyEvent();
				eventType = GameHud::EventType::END;
			}
			else {

				// notify longpress
				if (eventType != EventType::END){
					touchType = TouchType::LONG_PRESS;
					eventType = EventType::BEGIN;
					notifyEvent();
				}
			}
		}
		else {
			eventType = GameHud::EventType::END;
			notifyEvent();
			touchType = TouchType::NONE;
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMove,
		this);

}

void GameHud::update(float delta) {
	timeTouch += delta;
	if (eventType != EventType::END) {
		if (isPan && timeTouch != timeCatchEvent) {
			if (touchType == TouchType::LEFT || touchType == TouchType::RIGHT) {
				eventType = EventType::HOLD;
				notifyEvent();
			}
		}

		if (!isPan && timeTouch >= .2) {
			//catch Long press
			eventType = EventType::BEGIN;
			touchType = TouchType::LONG_PRESS;
			ignoreInput = true;
			notifyEvent();
			eventType = EventType::END;
		}
	}
}
void GameHud::setCallBack(
	const std::function<void(Touch* touch, GameHud::EventType, GameHud::TouchType)> &callBack) {
	this->callBack = callBack;
}
void GameHud::notifyEvent() {
	if (callBack) {
		callBack(touch, eventType, touchType);
	}
}

void GameHud::setState(HudState state) {
	this->state = state;
}

GameHud::HudState GameHud::getState() {
	return this->state;
}

GameHud::~GameHud() {

}

