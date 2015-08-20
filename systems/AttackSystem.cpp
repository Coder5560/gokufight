#include "AttackSystem.h"
#include "gokuartemis/RenderComponent.h"

AttackSystem::AttackSystem()
{
	addComponentType<PosComponent>();
	addComponentType<WallSensorComponent>();
	addComponentType<PhysicComponent>();
	addComponentType<BoundComponent>();
	addComponentType<PhysicComponent>();
	addComponentType<AttackComponent>();
}
void AttackSystem::initialize(){
	attackMapper.init(*world);
	posMapper.init(*world);
	boundMapper.init(*world);
	physicMapper.init(*world);
	gravityMapper.init(*world);
	wallSensorMapper.init(*world);
}

void AttackSystem::begin(){}

void AttackSystem::processEntity(artemis::Entity &e){
	AttackComponent* attack = attackMapper.get(e);
	PosComponent* position = (PosComponent*)e.getComponent<PosComponent>();
	WallSensorComponent* wallSensor = wallSensorMapper.get(e);
	if (!(attack && position && wallSensor)) return;
	if (attack->expire){
		EntityUtils::getInstance()->removeEntity(e);
		return;
	}
	// nếu chạm vào thành
	if (wallSensor->onAnysurface()){
		EntityUtils::getInstance()->removeEntity(e);
		return;
	}
	// nếu vượt qua phạm vi tấn công.
	if (position->x < attack->minX || position->y < attack->minY || position->x > attack->maxX || position->y > attack->maxY){
		EntityUtils::getInstance()->removeEntity(e);
		return;
	}


	if (attack->whoAttack == R::CharacterType::GOKU){
		artemis::Entity &entity = world->getTagManager()->getEntity("enemy");
		if (attackToEntity(e, entity)) {
			EntityUtils::getInstance()->removeEntity(e);
		}
	}
	else{
		artemis::Entity &goku = world->getTagManager()->getEntity("goku");
		if (attackToEntity(e, goku)){
			EntityUtils::getInstance()->removeEntity(e);
		}
	}

}
void AttackSystem::end(){}


bool AttackSystem::attackToEntity(artemis::Entity& attackEntity, artemis::Entity &entity){
	AttackComponent* attack = attackMapper.get(attackEntity);
	PosComponent* attackPosition = posMapper.get(attackEntity);
	BoundComponent* attackBound = boundMapper.get(attackEntity);


	PosComponent* defensePosition = posMapper.get(entity);
	BoundComponent* defenseBound = boundMapper.get(entity);
	CharacterInfoComponent* defenseInfo = (CharacterInfoComponent*)entity.getComponent<CharacterInfoComponent>();

	Rect attackRect = Rect(attackPosition->x + attackBound->x1, attackPosition->y + attackBound->y1, attackBound->getWidth(), attackBound->getHeight());
	Rect defenseRect = Rect(defensePosition->x + defenseBound->x1, defensePosition->y + defenseBound->y1, defenseBound->getWidth(), defenseBound->getHeight());

	if (attackRect.intersectsRect(defenseRect)){
		StateComponent* defenseState = (StateComponent*)entity.getComponent<StateComponent>();
		bool isLeft = defensePosition->x < attackPosition->x;
		defenseInfo->blood -= attack->powerOfAttack;
		defenseState->direction = isLeft ? R::Direction::LEFT : R::Direction::RIGHT;
		if (defenseInfo->blood <= 0){
			defenseState->setState(R::CharacterState::DIE);
		}
		else{
			defenseState->setState(R::CharacterState::DEFENSE);
			attack->isSpecialSkill ? (defenseState->defense = R::Defense::TRUNG_DON_NGA) : (defenseState->defense = R::Defense::TRUNG_DON);
		}
		float distance = attackRect.getMidX() - defenseRect.getMidX();
		Node* node = RenderLayer::getInstance()->createGameNode();
		if (attack->whoAttack == R::CharacterType::GOKU)node->setPosition(Vec2(attackRect.getMidX() / 2 + defenseRect.getMidX() / 2, defenseRect.getMidY() / 2 + attackRect.getMidY() / 2));
		else
			node->setPosition(Vec2(attackRect.getMidX() / 4 + 3 * defenseRect.getMidX() / 4, 3 * defenseRect.getMidY() / 4 + attackRect.getMidY() / 4));
		HitEffect* hitEffect = new HitEffect(node);
		hitEffect->setHitStyle(attack->whoAttack);
		hitEffect->start();
		attack->expire = true;
		// remove all attack component
		artemis::ImmutableBag<artemis::Entity*> *entities = world->getGroupManager()->getEntities((attack->whoAttack == R::CharacterType::GOKU) ? "enemies" : "gokus");
		for (int i = 0; i < entities->getCount(); i++){
			artemis::Entity* et = entities->get(i);
			if (et) {
				RenderComponent* render = (RenderComponent*)et->getComponent<RenderComponent>();
				if (render)	render->node->removeFromParent();
				et->remove();
			}
		}
		return true;
	}

	return false;
}

