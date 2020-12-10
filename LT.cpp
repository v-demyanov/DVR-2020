
#include "LT.h"
#include "Error.h"
#include <iomanip>

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

	void AddList(LexTable& lextable, std::list <LT::Entry> list_of_LT_Entries)
	{
		while (!list_of_LT_Entries.empty())
		{
			Add(lextable, list_of_LT_Entries.front());
			list_of_LT_Entries.pop_front();
		}
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

	Entry::Entry() {};
	
	Entry::Entry(char l, int n)
	{
		lexema = l;
		lineSource = n;
	};

	Entry::Entry(char l, int ls, int i, char* s)
	{
		lexema = l;
		lineSource = ls;
		indexIdTable = i;
		sign = s;
	};
	
}
