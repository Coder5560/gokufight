#ifndef  __NATIVE_HELPER_H_
#define  __NATIVE_HELPER_H_

#include <string>
#include <functional>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
namespace cocos2d

{

	public delegate void CSharpShowInterstitialDelegate();
	public delegate void CSharpShowBannerDelegate(bool b);

	public ref class WP8NativeEventHelper sealed

	{

	public:

		void WP8NativeEventHelper::SetCSharpShowInterstitialDelegate(CSharpShowInterstitialDelegate^ delegate){

			m_CSharpShowInterstitialDelegate = delegate;

		}

		void WP8NativeEventHelper::SetCSharpShowBanner(CSharpShowBannerDelegate^ delegate){
			m_CSharpShowBannerDelegate = delegate;
		}

		void CallShowInterstitial();
		void CallShowBanner(bool b);

	private:

		property static CSharpShowInterstitialDelegate^ m_CSharpShowInterstitialDelegate;
		property static CSharpShowBannerDelegate^ m_CSharpShowBannerDelegate;

	};



}
#endif

class NativeHelper
{
public:
	static void showInterstitial();
	static void showBanner(bool b);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	WP8NativeEventHelper^ wp8helper;
#endif

	//instance required only for setting callback
	static NativeHelper* getInstance();

	~NativeHelper()
	{
		instanceFlag = false;
	}

private:

	static bool instanceFlag;
	static NativeHelper* instance;

	NativeHelper() {};

};

#endif // __NATIVE_HELPER_H_