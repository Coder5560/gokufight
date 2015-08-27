//
//  CppJavaMethodManager.cpp
//  GameStore
//
//  Created by Manh Tran on 3/3/15.
//
//

#include "CppJavaMethodManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

std::map<long long, std::function<void(int)>> _funcs;


extern "C" {

JNIEXPORT void JNICALL Java_com_bgate_integration_JavaCpp_intCallback(
		JNIEnv* env, jobject thiz, jlong pointer, jint val) {
	cocos2d::log("MONKEYFIGHT invite callback 1 , address = %lld", pointer);
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(
		[env, pointer, val]() {
		cocos2d::log("MONKEYFIGHT invite callback 2");
					_funcs[pointer](val);
		cocos2d::log("MONKEYFIGHT invite callback 3");
	});
}

}

long long JavaCppBridge::put(const std::function<void(int)>& callback) {
	long long* address = (long long*) &callback;
	cocos2d::log("MONKEYFIGHT put address = %lld", *address);
	_funcs[*address] = callback;
	return *address;
}


#else

long long JavaCppBridge::put(const std::function<void(int)>& f) {
	return 0;
}

#endif
