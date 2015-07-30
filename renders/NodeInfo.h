#pragma once
#include "cocos2d.h"
#include "gokuartemis/Components.h"
#include "ui/UIImageView.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "RenderLayer.h"

USING_NS_CC;
class NodeInfo : public ui::Layout
{
public:
	NodeInfo();
	~NodeInfo();
	void process(CharacterInfoComponent* characterInfo);
	void createNode(CharacterInfoComponent* characterInfo);
protected:

	std::string characterTag;
	bool isCreated;
	ui::ImageView* avatar;
	ui::Layout* blood;
	ui::Layout* power;
	ui::Text* name;
	int timeNotify;
};

