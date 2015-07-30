#pragma once
#include "cocos2d.h"
#include "ui/UIText.h"
#include "gokuartemis/RenderLayer.h"
#include "R.h"


USING_NS_CC;
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
	void setHitStyle(R::CharacterType characterType);

	R::CharacterType hitType;

};
class NotEnoughManaEffect : public Effects {
public :
	NotEnoughManaEffect(Node* node);
	void start();
	void dismiss();
};
class KameKameHa : public Effects{
public :
	KameKameHa(Node* node);
	void start(bool isLeftDirection);
	void dismiss();
};