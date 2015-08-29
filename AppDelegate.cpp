#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "renders/StartScreen.h"
#include "R.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
        glview = GLViewImpl::create("Goku Fight");
        director->setOpenGLView(glview);
		//glview->setFrameSize(320, 480);
		//glview->setFrameSize(640,960);
		//glview->setFrameSize(640, 1136);
		//glview->setFrameSize(750, 1334);
		glview->setFrameSize(1242, 2208);
		//glview->setFrameSize(480, 800);

		glview->setFrameZoomFactor(.2f);
    }

	CCLOG("FrameSize : %f  %f", Director::getInstance()->getOpenGLView()->getFrameSize().width, Director::getInstance()->getOpenGLView()->getFrameSize().height);

    // turn on display FPS
    director->setDisplayStats(false);
	director->getOpenGLView()->setDesignResolutionSize(R::Constants::WIDTH_SCREEN, R::Constants::HEIGHT_SCREEN, ResolutionPolicy::FIXED_HEIGHT);
	
	R::Constants::WIDTH_SCREEN = Director::getInstance()->getWinSize().width;

	/*Size winSize = Size(R::Constants::WIDTH_SCREEN, 800);
	Size frameSize = glview->getFrameSize();
	float widthRate = frameSize.width / winSize.width;
	float heightRate = frameSize.height / winSize.height;
	if (widthRate > heightRate)
	{
		glview->setDesignResolutionSize(winSize.width,
			winSize.height*heightRate / widthRate, ResolutionPolicy::FIXED_HEIGHT);
	}
	else
	{
		glview->setDesignResolutionSize(winSize.width*widthRate / heightRate, winSize.height,
			ResolutionPolicy::FIXED_HEIGHT);
	}*/



    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    //auto scene = HelloWorld::createScene();
	auto scene = StartScreen::createScene();
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
