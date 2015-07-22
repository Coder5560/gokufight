#pragma once


namespace R{
	enum class Direction{
		AUTO, LEFT, RIGHT, TOP, BOTTOM, CENTER, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT

	};
	enum class CharacterState {
		STAND, WALK_LEFT, WALK_RIGHT, JUMP, KICK, PUNCH, BEAT, ATTACK,DEFENSE, DIE, LEFT, RIGHT,TRUNG_DON
	};

	enum class RenderType{

		NONE, BLOOD, POWER,STATIC, DYNAMIC

	};

	enum class CharacterType {

		NONAME, GOKU, YAMCHA, KRILLIN, GIRAN

	};
};


