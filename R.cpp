#pragma once

#include "R.h"


const float R::Constants::SCALE = 1;
const int R::Constants::WIDTH_SCREEN = 480;
const int R::Constants::HEIGHT_SCREEN = 800;
const int R::Constants::MAX_SCREEN_WIDTH =1280;
const int R::Constants::MAX_LIFE = 5;
const int R::Constants::MAX_LEVEL = 8;

float R::Constants::musicVolumn = .6f;
float R::Constants::soundVolumn = 1;
bool R::Constants::musicEnable = true;
bool R::Constants::soundEnable = true;
int R::Constants::unlocked = 0;
int R::Constants::remaininglife = 5;
bool R::Constants::howtoplay = false;

// bien de kiem tra so lan thua lien tiep, 3 lan thua thi hien quang cao 1 lan.
int R::Constants::countLose = 0;

void R::Constants::resetVariable(){
	auto userDefault = UserDefault::sharedUserDefault();
	unlocked = 0;
	remaininglife = 5;
	updateVariable();
}

void R::Constants::loadVariable(){
	auto userDefault = UserDefault::sharedUserDefault();
	musicEnable = userDefault->getBoolForKey("musicEnable");
	soundEnable = userDefault->getBoolForKey("soundEnable");
	howtoplay = userDefault->getBoolForKey("howtoplay");

	unlocked = userDefault->getIntegerForKey("unlocked");
	remaininglife = userDefault->getIntegerForKey("remaininglife");

	if (!howtoplay){
		musicEnable = true;
		soundEnable = true;
		unlocked = 0;
		remaininglife = 5;
	}
	CCLOG("unlocked : %d ", unlocked);
}

void R::Constants::updateVariable(){
	auto userDefault = UserDefault::sharedUserDefault();
	userDefault->setBoolForKey("musicEnable",musicEnable);
	userDefault->setBoolForKey("soundEnable", soundEnable);
	userDefault->setBoolForKey("howtoplay", howtoplay);

	userDefault->setIntegerForKey("remaininglife", remaininglife);
	userDefault->setIntegerForKey("unlocked", unlocked);
	userDefault->flush();
}