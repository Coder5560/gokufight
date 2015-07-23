#include "AttackSystem.h"
#include "gokuartermis/RenderComponent.h"

AttackSystem::AttackSystem()
{
	addComponentType<PosComponent>();
	addComponentType<BoundComponent>();
	addComponentType<AttackComponent>();

}
void AttackSystem::initialize(){
	attackMapper.init(*world);
	posMapper.init(*world);
	boundMapper.init(*world);
}

void AttackSystem::begin(){}

void AttackSystem::processEntity(artemis::Entity &e){
	AttackComponent* attack = attackMapper.get(e);
	attack->timeAlive += world->getDelta();
	if (attack->whoAttack == R::CharacterType::NONAME){ return; }
	if (attack->timeAlive > attack->maxTimeAlive) { world->getEntityManager()->remove(e);   return; }
	if (attack->powerOfAttack < 0 || attack->expire){ world->getEntityManager()->remove(e);  return; }
	if (attack->timeAlive < attack->timeAttack) return;
	if (attack->whoAttack == R::CharacterType::GOKU){
		// Lấy ra danh sách entity khác rồi xử lý chúng.
		artemis::ImmutableBag<artemis::Entity*> *bag = world->getGroupManager()->getEntities("enemy");
		if (bag->isEmpty()) { world->getEntityManager()->remove(e);   return; }
		for (int i = 0; i < bag->getCount(); i++){
			artemis::Entity* entity = bag->get(i);
			if (attackToEntity(e, *entity)) {
				RenderComponent* render = (RenderComponent*)e.getComponent<RenderComponent>();
				if (render)
					render->node->removeFromParentAndCleanup(true);
			}
		}
	}
	else{
		artemis::Entity &goku = world->getTagManager()->getEntity("goku");
		attackToEntity(e, goku);
	}
	world->getEntityManager()->remove(e);
}
void AttackSystem::end(){}


bool AttackSystem::attackToEntity(artemis::Entity& attackEntity, artemis::Entity &entity){
	AttackComponent* attack = attackMapper.get(attackEntity);
	PosComponent* attackPosition = posMapper.get(attackEntity);
	BoundComponent* attackBound = boundMapper.get(attackEntity);

	AttackComponent* defense = attackMapper.get(entity);
	PosComponent* defensePosition = posMapper.get(entity);
	BoundComponent* defenseBound = boundMapper.get(entity);
	CharacterInfoComponent* defenseInfo = (CharacterInfoComponent*)entity.getComponent<CharacterInfoComponent>();

	Rect attackRect = Rect(attackPosition->x + attackBound->x1, attackPosition->y + attackBound->y1, attackBound->getWidth(), attackBound->getHeight());
	Rect defenseRect = Rect(defensePosition->x + defenseBound->x1, defensePosition->y + defenseBound->y1, defenseBound->getWidth(), defenseBound->getHeight());

	if (attackRect.intersectsRect(defenseRect)){
		StateComponent* defenseState = (StateComponent*)entity.getComponent<StateComponent>();
		bool isLeft = defensePosition->x < attackPosition->x;
		defenseState->setState(R::CharacterState::DEFENSE);
		defenseState->defense = R::Defense::TRUNG_DON;
		defenseState->direction = isLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		defenseInfo->blood -= attack->powerOfAttack;
		attack->expire = true;
		return true;
	}


	return false;
}

