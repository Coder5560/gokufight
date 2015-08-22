#pragma once

#include "R.h"

const char* R::Constants::ENEMY_ATTACK = "sounds/enemy_attack.mp3";
const char* R::Constants::CLICK = "sounds/button_click.mp3";
const char* R::Constants::ENEMY_DEATH = "sounds/enemy_death.mp3";
const char* R::Constants::ENEMY_HIT2 = "sounds/enemy_hit_2.mp3";
const char* R::Constants::ENEMY_HIT3 = "sounds/hit_03.mp3";
const char* R::Constants::PUNCH = "sounds/punch.mp3";
const char* R::Constants::GOKU_ATTACK = "sounds/goku_attack.mp3";
const char* R::Constants::GOKU_BEHIT = "sounds/goku_behit.mp3";
const char* R::Constants::GOKU_DEATH = "sounds/enemy_death_3.mp3";
const char* R::Constants::JUMP = "sounds/jump.mp3";
const char* R::Constants::SKILL_1 = "sounds/skill_1.mp3";
const char* R::Constants::SKILL_3 = "sounds/skill_3.mp3";

const char* R::Constants::THEME_1 = "sounds/theme_1.mp3";
const char* R::Constants::THEME_2 = "sounds/theme_2.mp3";
const char* R::Constants::THEME_3 = "sounds/theme_3.mp3";
const char* R::Constants::BOMB = "sounds/bomb.mp3";
const char* R::Constants::MENU = "sounds/menu.mp3";
const char* R::Constants::LANDING = "sounds/landing.mp3";

const float R::Constants::SCALE = 1;
const int R::Constants::WIDTH_SCREEN = 480;
const int R::Constants::HEIGHT_SCREEN = 800;
const int R::Constants::MAX_SCREEN_WIDTH = 1280;
const int R::Constants::MAX_LIFE = 5;
const int R::Constants::MAX_LEVEL = 8;

float R::Constants::musicVolumn = .6f;
float R::Constants::soundVolumn = 1;
bool R::Constants::musicEnable = true;
bool R::Constants::soundEnable = true;
int R::Constants::lastPlay = 0;
int R::Constants::unlocked = 0;
int R::Constants::remaininglife = 5;
bool R::Constants::howtoplay = false;

// bien de kiem tra so lan thua lien tiep, 3 lan thua thi hien quang cao 1 lan.
int R::Constants::countLose = 0;

void R::Constants::resetVariable() {
	auto userDefault = UserDefault::sharedUserDefault();
	unlocked = 0;
	lastPlay = 0;
	remaininglife = 5;
	updateVariable();
}

void R::Constants::loadVariable() {
	auto userDefault = UserDefault::sharedUserDefault();
	musicEnable = userDefault->getBoolForKey("musicEnable");
	soundEnable = userDefault->getBoolForKey("soundEnable");
	howtoplay = userDefault->getBoolForKey("howtoplay");

	unlocked = userDefault->getIntegerForKey("unlocked");
	lastPlay = userDefault->getIntegerForKey("lastPlay");
	remaininglife = userDefault->getIntegerForKey("remaininglife");

	if (!howtoplay) {
		musicEnable = true;
		soundEnable = true;
		unlocked = 0;
		remaininglife = 5;
		howtoplay = true;
		lastPlay = 1;
	}

	/*// begin test
	 musicEnable = false;
	 soundEnable = true;
	 unlocked = 4;
	 remaininglife = 5;
	 howtoplay = true;
	 lastPlay = 2;
	 // end test*/
}

void R::Constants::updateVariable() {
	auto userDefault = UserDefault::sharedUserDefault();
	userDefault->setBoolForKey("musicEnable", musicEnable);
	userDefault->setBoolForKey("soundEnable", soundEnable);
	userDefault->setBoolForKey("howtoplay", howtoplay);

	userDefault->setIntegerForKey("remaininglife", remaininglife);
	userDefault->setIntegerForKey("unlocked", unlocked);
	userDefault->setIntegerForKey("lastPlay", lastPlay);
	userDefault->flush();
}
