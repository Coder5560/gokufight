#include "NodeInfo.h"


NodeInfo::NodeInfo() : characterTag(""), isCreated(false), avatar(nullptr), blood(nullptr), power(nullptr), name(nullptr)
{
}

NodeInfo::~NodeInfo()
{
}

void NodeInfo::createNode(CharacterInfoComponent* characterInfo){
	isCreated = true;
	characterTag = characterInfo->tag;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size size(200, 100);
	bool isLeft = characterInfo->isMainCharacter;

	this->setLayoutType(ui::Layout::Type::ABSOLUTE);
	this->setContentSize(size);
	this->setPosition(Vec2(isLeft ? 10 : visibleSize.width - this->getContentSize().width - 10, visibleSize.height - this->getContentSize().height - 20));
	RenderLayer::getInstance()->getHudLayer()->addChild(this);

	CCLOG("%s", characterInfo->tag);
	std::string directory = "textures/" + characterInfo->tag + ".png";
	avatar = ui::ImageView::create(directory, ui::Widget::TextureResType::LOCAL);
	avatar->setScale9Enabled(true);
	avatar->setTouchEnabled(false);
	avatar->setAnchorPoint(Vec2(0, 0));
	avatar->setPosition(Vec2(isLeft ? 10 : (size.width - avatar->getContentSize().width - 10), size.height - avatar->getContentSize().height / 2 - 10));
	avatar->ignoreContentAdaptWithSize(false);
	avatar->ignoreAnchorPointForPosition(false);


	blood = ui::Layout::create();
	blood->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	blood->setBackGroundColor(Color3B::RED);
	blood->setContentSize(Size(100, 4));
	blood->setTouchEnabled(false);
	blood->setPosition(Vec2(isLeft ? (avatar->getPositionX() + avatar->getContentSize().width + 10) : (avatar->getPositionX() - blood->getContentSize().width - 10), size.height - 10));

	power = ui::Layout::create();
	power->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	power->setBackGroundColor(Color3B::GREEN);
	power->setContentSize(Size(100, 4));
	power->setTouchEnabled(false);
	power->setPosition(Vec2(isLeft ? (avatar->getPositionX() + avatar->getContentSize().width + 10) : (avatar->getPositionX() - power->getContentSize().width - 10), blood->getPositionY() + blood->getContentSize().height + 2));

	name = ui::Text::create(characterInfo->tag, "fonts/Marker Felt.ttf", 14);
	name->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	name->setColor(Color3B::BLACK);
	name->setPosition(Vec2(avatar->getPositionX() + avatar->getContentSize().width / 2, avatar->getPositionY() - 4 - name->getContentSize().height / 2));

	this->addChild(avatar);
	this->addChild(blood);
	this->addChild(power);
	this->addChild(name);


}
void NodeInfo::process(CharacterInfoComponent* characterInfo){
	if (!isCreated) return;
	CCLOG("Blood %f", characterInfo->blood);
	float percent = (characterInfo->blood / 100);
	blood->setContentSize(Size(100 * (percent < 0 ? 0 : percent), blood->getContentSize().height));
	blood->setPosition(Vec2(characterInfo->isMainCharacter ? (avatar->getPositionX() + avatar->getContentSize().width + 10) : (avatar->getPositionX() - blood->getContentSize().width - 10), getContentSize().height - 10));
	power->setPosition(Vec2(characterInfo->isMainCharacter ? (avatar->getPositionX() + avatar->getContentSize().width + 10) : (avatar->getPositionX() - power->getContentSize().width - 10), blood->getPositionY() + blood->getContentSize().height + 2));

}




