#pragma once
#include "Headers.h"

#define FST_ARR_SIZE 20

namespace FST
{
	struct RELATION
	{
		char symbol;
		short nnode;
		RELATION(char c = 0x00, short ns = NULL);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST
	{
		const char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		char lex;
		FST();
		FST(const char* s, short ns, char l, NODE n, ...);
	};

	bool execute(FST& fst);
	bool step(FST& fst, short*& rstates);
	FST* arrFST();
};