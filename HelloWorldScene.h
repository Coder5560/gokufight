#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "artemis/Artemis.h"
#include "TestECS.h"
#include "cocos2d.h"
#include "GameHud.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void update(float delta);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	
protected:
	spine::SkeletonAnimation* skeletonNode;
	GameHud* gameHud;
	artemis::World* world;
	TestECS* testEcs;
	Sprite* sprite;
};

#endif // __HELLOWORLD_SCENE_H__
