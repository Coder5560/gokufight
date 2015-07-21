#include "AttackSystem.h"


AttackSystem::AttackSystem()
{
	addComponentType<PosComponent>();
	addComponentType<AttackComponent>();
	addComponentType<BoundComponent>();

}
void AttackSystem::initialize(){

}


