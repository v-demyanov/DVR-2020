
#include "LT.h"
#include "Error.h"

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
		if (lextable.size + 1 > LT_MAXSIZE)
			throw ERROR_THROW_IN(123, -1, -1);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		delete[] &lextable;
	}
}
