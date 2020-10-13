#pragma once
#include "IT.h"
#include "LT.h"
#include "In.h"

namespace LR
{
	struct Tables
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;
		
	};

	Tables FillingTables(In::IN in);
	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, char str);
	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, int vint);
	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, char str[TI_STR_MAXSIZE - 1]);
}