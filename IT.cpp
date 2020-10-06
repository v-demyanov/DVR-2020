#include "IT.h"
#include <iostream>


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

	int IsId(IdTable& idtable, char id[ID_MAXSIZE], int beginPosition)
	{
		for (int i = idtable.size - beginPosition; i < idtable.size; i++)
		{
			if (i < 0) i = 0;
			int t1 = strcmp(idtable.table[i].id, id);
			if (t1 == 0)
			{
				return i;
			}	
		}
		return TI_NULLIDX;
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == id)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		delete[] & idtable;
	}
}
