#pragma once
#include "cocos2d.h"
#include "ui/UIText.h"
#include "gokuartemis/RenderLayer.h"
#include "R.h"
#include "ui/UIImageView.h"
#include "ui/UILayout.h"
#include "spine/spine-cocos2dx.h"

USING_NS_CC;
class MatchInfo;
class HitEffect;
class KameKameHa;

class Effects
{
public:
	Effects();
	~Effects();
	Node* node;
};

class HitEffect : public Effects{
public :
	HitEffect(Node* node);
	void start();
	void dismiss();
	void setHitStyle(R::CharacterType characterType);

	R::CharacterType hitType;
	bool safeToRemove;

};
class NotEnoughManaEffect : public Effects {
public :
	NotEnoughManaEffect(Node* node);
	void start();
	void dismiss();
};

class Message : public Effects {
public:
	Message(Node* node);
	void start(std::string message);
	void dismiss();
};

class KameKameHa : public Effects{
public :
	KameKameHa(Node* node);
	void start(bool isLeftDirection);
	void dismiss();
};

class MatchInfo : public Effects{
public :
	MatchInfo(Node* node);
};

class PlayerInfoLeft : public Effects{
public :
	PlayerInfoLeft(Node* node,std::string name,std::string avatar);
	void update(float blood, float mana);
	std::string _name;
	std::string _avatar;
	ui::Layout* mana;
	ui::Layout* blood;
	Size size;
};

class PlayerInfoRight : public Effects{
public:
	PlayerInfoRight(Node* node, std::string name, std::string avatar);
	void update(float blood, float mana);
	std::string _name;
	std::string _avatar;
	ui::Layout* mana;
	ui::Layout* blood;
	Size size;
};