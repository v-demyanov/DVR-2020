#include "LT.h"

namespace LT
{
	LexTable Create(int size)
	{
		LexTable* tableEx = new LexTable;
		tableEx->maxsize = size;
		tableEx->size = 0;
		tableEx->table = new Entry[size];
		return *tableEx;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	Entry CheckLexem(const char* lex)
	{
		FST::FST* arrFST = new FST::FST;
		LT::Entry result;
		for (int i = 0; i < FST_ARR_SIZE; i++)
		{
			arrFST[i].string = lex;
			if (execute(arrFST[i]))
			{
				result.lexema = arrFST[i].lex;
				return result;
			}
		}
		return {};
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		delete[] &lextable;
	}
}
