#include "GameHud.h"

GameHud::GameHud(Size visibleSize) :
state(HudState::HIDE), touchType(TouchType::NONE), eventType(
EventType::RELEASE), timeTouch(0), isTouchMove(false), ignoreInput(
false), timeCatchEvent(0) {
	this->setContentSize(visibleSize);
	this->scheduleUpdate();
}
void GameHud::buildComponent() {
	Size size = getContentSize();
	this->setSwallowsTouches(true);

	EventListenerTouchOneByOne* listenerMove =
		EventListenerTouchOneByOne::create();
	listenerMove->onTouchBegan = [this](Touch* touch, Event* event) {
		timeTouch = 0;
		timeCatchEvent = -1;
		isTouchMove = false;
		ignoreInput = false;
		eventType = EventType::BEGIN;
		touchType = TouchType::NONE;
		return true;

	};
	listenerMove->onTouchMoved =
		[this](Touch* touch, Event* event) {
		if (ignoreInput) return;
		Vec2 delta = touch->getDelta();
		float length = touch->getDelta().getLength();
		float angle = 180 * atan2f(touch->getDelta().y, touch->getDelta().x) / M_PI;
		if (angle < 0) angle += 360;
		if (isTouchMove && length>4) {
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

		if (!isTouchMove && length > 6) {
			if (22 <= angle && angle <= 77) {
				if (length > 20) {
					touchType = TOP_RIGHT;
				}
				else {
					touchType = RIGHT;
				}
			}
			else if (77 <= angle && angle <= 112) {
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

			isTouchMove = true;
			timeCatchEvent = timeTouch;
			eventType = GameHud::EventType::BEGIN;
			notifyEvent();
		}
	};

	listenerMove->onTouchEnded = [this](Touch* touch, Event* event) {
		if (ignoreInput) return;

		if (!isTouchMove) {

			if (timeTouch < .2) {

				//notify tap
				eventType = GameHud::EventType::BEGIN;
				touchType = TouchType::TAP;
				notifyEvent();
				eventType = GameHud::EventType::END;
			}
			else {

				// notify longpress
				touchType = TouchType::LONG_PRESS;
				eventType = EventType::BEGIN;
				notifyEvent();
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
		if (isTouchMove && timeTouch != timeCatchEvent) {
			if (touchType == TouchType::LEFT || touchType == TouchType::RIGHT) {
				eventType = EventType::HOLD;
				notifyEvent();
			}
		}

		if (!isTouchMove && timeTouch >= .2) {
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
	const std::function<void(GameHud::EventType, GameHud::TouchType)> &callBack) {
	this->callBack = callBack;
}
void GameHud::notifyEvent() {
	if (callBack) {
		callBack(eventType, touchType);
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

