#include "RenderSystem.h"


RenderSystem::RenderSystem()
{
	addComponentType<RenderComponent>();
	addComponentType<PosComponent>();
}


RenderSystem::~RenderSystem(){}

void RenderSystem::initialize(){
	renderMapper.init(*world);
	posMapper.init(*world);
}
void RenderSystem::processEntity(artemis::Entity &e){
	/*try{
		RenderComponent* renderComponent = renderMapper.get(e);
		PosComponent* posComponent = posMapper.get(e);
		if (renderComponent && renderComponent->node && posComponent){
			if (renderComponent->renderType == R::RenderType::DYNAMIC){
				renderComponent->node->setPosition(Vec2(posComponent->x, posComponent->y));
			}
			if (renderComponent->renderType == R::RenderType::BLOOD){
				renderBlood(e, (BloodComponent*)renderComponent, posComponent);
			}
		}
	}*/
}

void RenderSystem::renderBlood(artemis::Entity &e, BloodComponent* bloodComponent, PosComponent* posCompoent){
	float percent = bloodComponent->blood / bloodComponent->MAX_BLOOD;
	bloodComponent->node->setPosition(Vec2(posCompoent->x, posCompoent->y));
}


