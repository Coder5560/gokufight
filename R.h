#pragma once


namespace R{

	class Constants{
	public :
		static const float SCALE;
		static const int WIDTH_SCREEN;
		static const int HEIGHT_SCREEN;
		static const int MAX_SCREEN_WIDTH;
	};
	enum class Match_Type{
		NONE, GOKU_GIRAN, GOKU_BEAR,GOKU_JACKIECHUN

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
		WIN,START,STAND,STAND_UP, WALK_LEFT, WALK_RIGHT, JUMP, KICK, PUNCH, BEAT, ATTACK, DEFENSE, DIE, LEFT, RIGHT, TRUNG_DON
	};
	enum class Attack{
		NONE,
		// FOR GOKU
		GOKU_KICK1, GOKU_KICK2, GOKU_KICK3, GOKU_BEAT1, GOKU_BEAT2, GOKU_BEAT3, GOKU_PUNCH1, GOKU_PUNCH2, GOKU_PUNCH3,

		//for giran
		GIRAN_PUNCH1,GIRAN_PUNCH2,GIRAN_PUNCH3,


		//for bear
		BEAR_ATTACK1, BEAR_ATTACK2,

		//for jackiechun 

		JACK_PUNCH_AIR,JACK_PUNCH_1,JACK_PUNCH_2,JACK_KICK2,JACK_BEAT3,JACK_KICK3

	};

	enum class Defense{
		NONE,TRUNG_DON,

		GOKU_TRUNG_DON, GOKU_JUMP, GOKU_STAND_UP
	};


	enum class RenderType{

		NONE, BLOOD, POWER,STATIC, DYNAMIC

	};

	enum class CharacterType {

		NONAME, GOKU, YAMCHA, KRILLIN, GIRAN,BEAR,JACKIECHUN

	};
};


