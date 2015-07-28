#include "RenderComponent.h"


RenderComponent::RenderComponent() : renderTime(0), timeToApper(0), removeFromWorld(false)
{
	renderType = R::RenderType::NONE;
	node = RenderLayer::getInstance()->createGameNode();
}


RenderComponent::~RenderComponent(){ 
	
}

BloodComponent::BloodComponent(){
	BloodComponent::RenderComponent();
	renderType = R::RenderType::BLOOD;
}

