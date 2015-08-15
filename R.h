#pragma once
#include <SimpleAudioEngine.h>
namespace R{

	class Constants{
	public :
		static const float SCALE;
		static const int WIDTH_SCREEN;
		static const int HEIGHT_SCREEN;
		static const int MAX_SCREEN_WIDTH;
		static float musicVolumn;
		static float soundVolumn;
		static bool musicEnable;
		static bool soundEnable;
		static int unlocked;
	};
	enum class Match_Type{
		GOKU_BEAR_INTRODUCE, NONE, GOKU_GIRAN, GOKU_BEAR, GOKU_JACKIECHUN, GOKU_TEGIAC, GOKU_CAMAP, GOKU_RUA, GOKU_KARILLIN, GOKU_PICOLO

	};

	enum class GameState{
		NONE,
		// đây là bước chuẩn bị vào game, mọi thao tác trong game chưa hoạt động, các sự kiện diễn ra trên nền UI.
		PREPARE,
		// Chuẩn bị vào trận đấu
		ANIMATING_TO_FIGHT,
		// Hoạt cảnh thắng
		WIN,
		// Hoạt cảnh thua
		LOSE,
		// Hoạt cảnh chọn người chơi
		CHOSE_PLAYER,
		// Hoạt cảnh lúc chiến đấu.
		FIGHTING,



	};
	enum class Direction{
		AUTO, LEFT, RIGHT, TOP, BOTTOM, CENTER, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT

	};
	enum class CharacterState {
		WIN,START,STAND,STAND_UP, BACK, WALK_LEFT, WALK_RIGHT, JUMP, ATTACK, DEFENSE, DIE, LEFT, RIGHT, TRUNG_DON
	};
	enum class Attack{
		NONE,
		// FOR GOKU
		GOKU_KICK1, GOKU_KICK2, GOKU_KICK3, GOKU_BEAT1, GOKU_BEAT2, GOKU_BEAT3, GOKU_PUNCH1, GOKU_PUNCH2, GOKU_PUNCH3,

		// FOR KARILLIN
		KARILLIN_KICK1, KARILLIN_KICK2, KARILLIN_KICK3, KARILLIN_BEAT1, KARILLIN_BEAT2, KARILLIN_BEAT3, KARILLIN_PUNCH1, KARILLIN_PUNCH2, KARILLIN_PUNCH3,

		//for giran
		GIRAN_PUNCH1,GIRAN_PUNCH2,GIRAN_PUNCH3,


		//for bear
		BEAR_ATTACK1, BEAR_ATTACK2,

		//for jackiechun 
		JACK_PUNCH_AIR,JACK_PUNCH_1,JACK_PUNCH_2,JACK_KICK2,JACK_BEAT3,JACK_KICK3,

		//for tegiac
		TEGIAC_PUNCH_AIR,TEGIAC_PUNCH1,TEGIAC_KICK2,TEGIAC_PUNCH2,TEGIAC_BEAT3,TEGIAC_KICK3,TEGIAC_PUNCH3,

		//for Rua
		RUA_ATTACK1,RUA_ATTACK2,RUA_ATTACK3,RUA_ATTACK4,RUA_ATTACK5,

		//cho con Cá mập
		CAMAP_PUNCH_AIR, CAMAP_SKILL,CAMAP_KICK2,CAMAP_PUNCH2,CAMAP_PUNCH3,CAMAP_PUNCH1,

		PICOLO_ATTACK1,PICOLO_ATTACK2,PICOLO_ATTACK3,PICOLO_KICK,PICOLO_POWER1,PICOLO_POWER2
	};

	enum class Defense{
		NONE,TRUNG_DON,TRUNG_DON_NGA,
	};


	enum class RenderType{

		NONE, BLOOD, POWER,STATIC, DYNAMIC

	};

	enum class CharacterType {
		NONAME, GOKU, YAMCHA, KRILLIN, GIRAN,BEAR,JACKIECHUN,TEGIAC,CAT,BOMB,CAMAP,RUA,KARILLIN,PICOLO	

	};

	enum class CatFollowState {
		FOLLOW_ENEMY, FOLLOW_GOKU,ATTACK_GOKU,MOVE_OUT,MOVE_IN,NONE

	};

	enum class CameraState {
		NONE, FOLLOW_MAIN_CHARACTER,ZOOM_OUT_TO_SKILL
	};

	enum class CharacterUIState{
		SHOW, HIDE, ANIMATING
	};


	enum class IntroduceStep{
		NONE, KICK_ATTACK,PUNCH_ATTACK,BEAT_ATTACK, MOVE, MOVE_AND_JUMP,JUMP
	};
};


