#pragma once
#define LEXEMA_FIXSIZE				1				// фикс размер лексемы
#define LT_MAXSIZE					4096			// макс кол-во строк в таблице лексем
#define LT_TI_NULLIDX				0xffffffff		// нет элемента таблицы идентификаторов
#define LEX_MAIN					'm'				// main
#define LEX_TYPE					't'
#define LEX_INTEGER					'x'				// integer
#define LEX_STRING					'y'				// string
#define LEX_ID						'i'				// идентификатор
#define LEX_LITERAL					'l'				// литерал
#define LEX_STRING_LITERAL			's'				// строковый литерал
#define LEX_NUMERICAL_LITERAL		'n'				// числовой литерал
#define LEX_FUNCTION				'f'				// function
#define LEX_DECLARE					'd'				// declare
#define LEX_RETURN					'r'				// return
#define LEX_PRINT					'p'				// print
#define LEX_SEMICOLON				';'				// ;
#define LEX_COMMA					','				// ,
#define LEX_LEFTBRACE				'{'				// {
#define LEX_BRACELET				'}'				// }
#define LEX_LEFTHESIS				'('				// (
#define LEX_RIGHTHESIS				')'				// )
#define LEX_PLUS					'v'				// +
#define LEX_MINUS					'v'				// -
#define LEX_STAR					'v'				// *
#define LEX_DIRSLASH				'v'				// /
#define LEX_EQUAL					'='				// =

namespace LT									// таблица лексем
{
	struct Entry								// строка таблицы лексем
	{
		char lexema;							// лексема
		int lineSource;							// номер строки в исходном коде
		int indexIdTable;						// индекс в таблице идентификаторов или LT_TI_NULLIDX	
	};

	struct LexTable								// экзэмпл€р таблицы лексем
	{
		int maxsize;							// Ємкость таблицы лексем < LT_MAXSIZE
		int size;								// текущий размер таблицы лексем < maxsize
		Entry* table;							// массив строк таблицы лексем
	};

	LexTable Create(int size);					// создать таблицу лексем, Ємкость таблицы лексем < LT_MAXSIZE
	void Add(LexTable& lextable, Entry entry);	// добавить строку в таблицу лексем 
	Entry GetEntry(LexTable& lextable, int n);	// получить строку таблицы лексем, n - номер получаемой строки
	void Delete(LexTable& lextable);			// удалить таблицу лексем (освободить пам€ть)
	
}