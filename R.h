#pragma once


namespace R{
	enum class GameState{
		// đây là bước chuẩn bị vào game, mọi thao tác trong game chưa hoạt động, các sự kiện diễn ra trên nền UI.
		PREPARE,
		// Chuẩn bị vào trận đấu
		ANIMATING_TO_FIGHT,
		// Hoạt cảnh thắng
		WIN_EFFECT,
		// Hoạt cảnh thua
		LOSE_EFFECT,
		// Hoạt cảnh chọn người chơi
		CHOSE_PLAYER,
		// Hoạt cảnh lúc chiến đấu.
		FIGHTING,
	};
	enum class Direction{
		AUTO, LEFT, RIGHT, TOP, BOTTOM, CENTER, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT

	};
	enum class CharacterState {
		START,STAND, WALK_LEFT, WALK_RIGHT, JUMP, KICK, PUNCH, BEAT, ATTACK, DEFENSE, DIE, LEFT, RIGHT, TRUNG_DON
	};
	enum class Attack{
		NONE,
		// FOR GOKU
		GOKU_KICK1, GOKU_KICK2, GOKU_KICK3, GOKU_BEAT1, GOKU_BEAT2, GOKU_BEAT3, GOKU_PUNCH1, GOKU_PUNCH2, GOKU_PUNCH3,

		//for giran
		GIRAN_PUNCH1,GIRAN_PUNCH2,GIRAN_PUNCH3,

	};

	enum class Defense{
		NONE,TRUNG_DON,

		GOKU_TRUNG_DON, GOKU_JUMP, GOKU_STAND_UP
	};


	enum class RenderType{

		NONE, BLOOD, POWER,STATIC, DYNAMIC

	};

	enum class CharacterType {

		NONAME, GOKU, YAMCHA, KRILLIN, GIRAN

	};
};


