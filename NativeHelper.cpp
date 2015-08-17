#include "NativeHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#ifndef  __NATIVE_HELPER_WP_H_
#define  __NATIVE_HELPER_WP_H_


void WP8NativeEventHelper::CallShowInterstitial(){

	if (m_CSharpShowInterstitialDelegate)

	{

		m_CSharpShowInterstitialDelegate->Invoke();

	}

}

void WP8NativeEventHelper::CallShowBanner(bool b){
	if (m_CSharpShowBannerDelegate){
		m_CSharpShowBannerDelegate->Invoke(b);
	}
}

bool NativeHelper::instanceFlag = false;
NativeHelper* NativeHelper::instance = NULL;

NativeHelper* NativeHelper::getInstance()
{
	if (!instanceFlag){
		instance = new NativeHelper();
		instanceFlag = true;
		instance->wp8helper = ref new WP8NativeEventHelper();
	}

	return instance;
}

void NativeHelper::showInterstitial(){
	NativeHelper::getInstance()->wp8helper->CallShowInterstitial();
}

void NativeHelper::showBanner(bool b){
	NativeHelper::getInstance()->wp8helper->CallShowBanner(b);
}

#endif
#endif
