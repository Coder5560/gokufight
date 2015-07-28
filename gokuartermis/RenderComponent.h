#pragma once
#include "R.h"
#include "cocos2d.h"
#include "artemis/Artemis.h"
#include "RenderLayer.h"
USING_NS_CC;

class RenderComponent : public artemis::Component
{
public:
	
	RenderComponent();
	~RenderComponent();

	R::RenderType renderType;
	Node* node;
	
	float renderTime;
	float timeToApper;
	bool removeFromWorld;

};

class BloodComponent : public RenderComponent
{
public :
	BloodComponent();
	float blood;
	float MAX_BLOOD;
};
