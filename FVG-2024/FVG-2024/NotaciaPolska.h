#pragma once
#include "LEX.h"

namespace NotaciaPolska {
	bool NotaciaPolska(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
	//short UzyskacPierwszenstwo(char t);
	void Wykonac(LEX::LEX t);
}