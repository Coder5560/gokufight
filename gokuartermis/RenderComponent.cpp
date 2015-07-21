#include "RenderComponent.h"


RenderComponent::RenderComponent()
{
	renderType = R::RenderType::NONE;
	node = RenderLayer::getInstance()->createGameNode();
}


RenderComponent::~RenderComponent(){}

BloodComponent::BloodComponent(){
	BloodComponent::RenderComponent();
	renderType = R::RenderType::BLOOD;
}

