//
//  AdsManager.cpp
//  JellySpeed
//
//  Created by Manh Tran on 5/13/15.
//
//

#include "AdsManager.h"
#include "NativeHelper.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"

void AdsManager::showFullAds()
{
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/bgate/social/AdmobBridge", "showFullAdsJNI", "()V")) {
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void AdsManager::showAds(bool visible)
{
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/bgate/social/AdmobBridge", "showBannerJNI", "(Z)V")) {
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, visible);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

#elif CC_TARGET_PLATFORM != CC_PLATFORM_IOS

void AdsManager::showFullAds()
{
    
}

void AdsManager::showAds(bool visible)
{
    
}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
void AdsManager::showFullAds()
{
	NativeHelper::showInterstitial();
}

void AdsManager::showAds(bool visible)
{
		NativeHelper::showBanner(visible);
}
#else 
#endif


