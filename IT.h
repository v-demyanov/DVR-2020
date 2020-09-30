#pragma once
#define ID_MAXSIZE		5			// макс кол-во символов в идентификаторе
#define TI_MAXSIZE		4096		// макс кол-во строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000	// значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT	0x00		// значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff	// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE  255

namespace IT										// таблица идентификатов
{
	enum IDDATATYPE {INT=1, STR=2};					// типы данных идентификаторов: integer, string
	enum IDTYPE		{V = 1, F = 2, P = 3, L = 3};	// типы идентификаторов: переменна€, функци€, параметр, литерал 

	struct Entry									// строка таблицы идентификаторов
	{
		int			idxfirstLE;						// индекс первой строки в таблицы лексем
		char*		id;								// идентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		IDDATATYPE	iddatatype;						// тип данных
		IDTYPE		idtype;							// тип идентикатора
		union
		{
			int vint;								// значение integer
			struct
			{
				char len;							// количесвто символов в string
				char* str;							// символы string
			} vstr[TI_STR_MAXSIZE];					// значение string
		} value;									// значение идентификатора
	};

	struct IdTable									// экземпл€р таблицы идентификаторов
	{
		int maxsize;								// Ємкость таблицы идентификаторов < TI_MAXSIZE
		int size;									// текущий размер таблицы идентификаторов < maxsize
		Entry* table;								// массив строк таблицы идентификаторов
	};

	IdTable Create(int size);						// создать таблицу идентификаторов, size - Ємкость таблицы идентификаторов < TI_MAXSIZE
	void Add(IdTable& idtable, Entry entry);		// добавить строку в таблицу идентификаторов
	Entry GetEntry(IdTable& idtable, int n);		// получить строку таблицы идентификаторов, n - номер получаемой строки
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);// возврат: номер строки (если есть), TI_NULLIDX(если нет)
	void Delete(IdTable& idtable);					// удалить таблицу лексем (освободить пам€ть)
}