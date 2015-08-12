#include "NodeInfo.h"


NodeInfo::NodeInfo() : characterTag(""), isCreated(false), avatar(nullptr), blood(nullptr), power(nullptr), name(nullptr), timeNotify(0)
{
}

NodeInfo::~NodeInfo()
{
}

void NodeInfo::createNode(CharacterInfoComponent* characterInfo){
	isCreated = true;
	
	Size visibleSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
	Size size(200, 100);
	bool isLeft = characterInfo->isMainCharacter;

	this->setLayoutType(ui::Layout::Type::ABSOLUTE);
	this->setContentSize(size);
	this->setPosition(Vec2(isLeft ? 10: visibleSize.width - this->getContentSize().width , visibleSize.height - this->getContentSize().height - 20));
	RenderLayer::getInstance()->getHudLayer()->addChild(this);
	
	
	//std::string directory = "textures/" + characterInfo->tag + ".png";
	avatar = ui::ImageView::create("textures/goku.png", ui::Widget::TextureResType::LOCAL);
	avatar->setAnchorPoint(Vec2(0.5, 0.5));
	avatar->setScaleX(isLeft?1:-1);
	avatar->setScale9Enabled(true);
	avatar->setTouchEnabled(false);
	
	avatar->setPosition(Vec2(isLeft ? (10 + avatar->getContentSize().width / 2) : (size.width - avatar->getContentSize().width / 2 - 20), size.height - avatar->getContentSize().height / 2 - 10));
	avatar->ignoreContentAdaptWithSize(false);
	avatar->ignoreAnchorPointForPosition(false);

	

	blood = ui::Layout::create();
	blood->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	blood->setBackGroundColor(Color3B::RED);
	blood->setContentSize(Size(100, 4));
	blood->setTouchEnabled(false);
	blood->setPosition(Vec2(isLeft ? (avatar->getPositionX() + avatar->getContentSize().width/2 + 10) : (avatar->getPositionX()- avatar->getContentSize().width/2 - blood->getContentSize().width - 10), avatar->getPositionY() - 10));

	power = ui::Layout::create();
	power->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	power->setBackGroundColor(Color3B::GREEN);
	power->setContentSize(Size(100, 4));
	power->setTouchEnabled(false);
	power->setPosition(Vec2(isLeft ? (avatar->getPositionX() + avatar->getContentSize().width / 2 + 10) : (avatar->getPositionX() - avatar->getContentSize().width / 2 - power->getContentSize().width - 10), blood->getPositionY() + blood->getContentSize().height + 2));

	name = ui::Text::create(isLeft?"Goku": "Giran", "fonts/Marker Felt.ttf", 14);
	name->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	name->setColor(Color3B::BLACK);
	name->setPosition(Vec2(avatar->getPositionX() , avatar->getPositionY() - avatar->getContentSize().height/2- 4 - name->getContentSize().height / 2));

	this->addChild(avatar);
	this->addChild(blood);
	this->addChild(power);
	this->addChild(name);

	this->setCameraMask((unsigned short)CameraFlag::USER1);

}
void NodeInfo::process(CharacterInfoComponent* characterInfo){
	if (!isCreated) return;
	float percentBlood = (characterInfo->blood / 100);
	float percentPower = (characterInfo->power / 100);
	blood->setContentSize(Size(100 * (percentBlood < 0 ? 0 : percentBlood), blood->getContentSize().height));
	power->setContentSize(Size(100 * (percentPower < 0 ? 0 : percentPower), power->getContentSize().height));
	blood->setPosition(Vec2(characterInfo->isMainCharacter ? (avatar->getPositionX() + avatar->getContentSize().width / 2 + 10) : (avatar->getPositionX() - avatar->getContentSize().width / 2 - blood->getContentSize().width - 10), avatar->getPositionY() - 10));
	power->setPosition(Vec2(characterInfo->isMainCharacter ? (avatar->getPositionX() + avatar->getContentSize().width / 2 + 10) : (avatar->getPositionX() - avatar->getContentSize().width / 2 - power->getContentSize().width - 10), blood->getPositionY() + blood->getContentSize().height + 2));
	characterInfo->power += .05;
	characterInfo->power = (characterInfo->power < characterInfo->MAX_POWER) ? characterInfo->power : characterInfo->MAX_POWER;

	if (characterInfo->notifyMana){
		timeNotify++;
		power->setVisible(timeNotify%4 < 2);
		if (timeNotify > 20){
			timeNotify = 0;
			characterInfo->notifyMana = false;
			power->setVisible(true);
		}
	}
}




