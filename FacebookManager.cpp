#include "FacebookManager.h"
#include "NativeHelper.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
void FacebookManager::shareFacebookPhoto(std::string filePath)
{
	CCLOG("AAAAAAAAAAAAA");
	cocos2d::JniMethodInfo methodInfo;

	if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/bgate/social/FacebookBridge", "sharePhotoJNI", "(Ljava/lang/String;)V")) {
		return;
	}
	jstring stringArg1 = methodInfo.env->NewStringUTF(filePath.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1);
	methodInfo.env->DeleteLocalRef(stringArg1);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}


#elif CC_TARGET_PLATFORM != CC_PLATFORM_IOS

void FacebookManager::shareFacebookPhoto(std::string filePath)
{
}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
void FacebookManager::shareFacebookPhoto(std::string filePath)
{
}

#else
#endif


