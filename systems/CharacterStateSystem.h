#pragma once
#include "artemis/Artemis.h"
#include "gokuartermis/Systems.h"
#include "systems/EntityUtils.h"


class CharacterStateSystem : public artemis::EntityProcessingSystem
{
public:
	CharacterStateSystem();
	~CharacterStateSystem();

	virtual void initialize();
	virtual void processEntity(artemis::Entity &e);
protected:
	artemis::ComponentMapper<StateComponent> stateMapper;
	artemis::ComponentMapper<SkeletonComponent> skeletonMapper;
	
};

