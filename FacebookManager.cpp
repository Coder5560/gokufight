#include "FacebookManager.h"
#include "NativeHelper.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
void FacebookManager::shareFacebookPhoto(std::string filePath)
{
	cocos2d::JniMethodInfo methodInfo;

	if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/bgate/social/FacebookBridge", "sharePhotoJNI", "(Ljava/lang/String;)V")) {
		return;
	}
	jstring stringArg1 = methodInfo.env->NewStringUTF(filePath.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1);
	methodInfo.env->DeleteLocalRef(stringArg1);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void FacebookManager::inviteFriends(long long pointer)
{
	cocos2d::JniMethodInfo methodInfo;

	cocos2d::log("MONKEYFIGHT send address = %lld", pointer);

	if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/bgate/social/FacebookBridge", "inviteFriendsFacebook", "(J)V")) {
		return;
	}
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,pointer);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}


#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

void FacebookManager::shareFacebookPhoto(std::string filePath)
{
}

void FacebookManager::inviteFriends(long long pointer)
{
	FBSDKAppInviteContent *content =[[FBSDKAppInviteContent alloc] init];
	content.appLinkURL = [NSURL URLWithString:"https://fb.me/515705371925582"];
	//optionally set previewImageURL
	content.appInvitePreviewImageURL = [NSURL URLWithString:"http://mywork.com.vn/data/images/logo/2cf36a0aee82.png"];

	// present the dialog. Assumes self implements protocol `FBSDKAppInviteDialogDelegate`
	[FBSDKAppInviteDialog showWithContent:content
	                             delegate:self];

}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
void FacebookManager::shareFacebookPhoto(std::string filePath)
{
}
void FacebookManager::inviteFriends(long long pointer)
{
}
#else
#endif


