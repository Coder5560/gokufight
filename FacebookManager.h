#pragma once
#include "cocos2d.h"
class FacebookManager
{
public:

	static void shareFacebookPhoto(std::string filePath);

	static void inviteFriends(long long pointer);
};

