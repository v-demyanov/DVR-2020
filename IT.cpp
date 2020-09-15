#include "IT.h"


namespace IT
{
	IdTable Create(int size)
	{
		IdTable* tableEx = new IdTable;
		tableEx->maxsize = size;
		tableEx->size = 0;
		tableEx->table = new Entry[size];
		return *tableEx;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == id)
				return i;
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		delete[] & idtable;
	}
}
