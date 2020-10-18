#pragma once
#include "IT.h"
#include "LT.h"

#define EXP1 28		// позиция первого выражения
#define EXP2 50		// позиция второго выражения
#define EXP3 66		// позиция третьего выражения

namespace PN
{
	bool PolishNotation(
		int				lextable_pos,	// позиция выражения в lextable
		LT::LexTable&	lextable,
		IT::IdTable&	idtable
	);

}