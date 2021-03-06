#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "artemis/Artemis.h"
#include "cocos2d.h"
#include "GameHud.h"
#include "R.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(R::Match_Type type);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void gotoMatch(R::Match_Type type);
	void update(float delta);
	
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	
protected:
	spine::SkeletonAnimation* skeletonNode;
	artemis::World* world;
	Sprite* sprite;
};

#endif // __HELLOWORLD_SCENE_H__
