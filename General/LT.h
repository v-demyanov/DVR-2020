#pragma once
#include <list>
#include <stack>
#include <vector>
#define LEXEMA_FIXSIZE				1				// фикс размер лексемы
#define LT_MAXSIZE					4096			// макс кол-во строк в таблице лексем
#define LT_TI_NULLIDX				0xffffffff		// нет элемента таблицы идентификаторов

#define LEX_MAIN	 				'g'				// go
#define LEX_FUNCTION				'f'				// func
#define LEX_RETURN					'r'				// ret
#define LEX_PRINT					'p'				// puts
#define LEX_IF						'q'				// if
#define LEX_UNTIL					'u'				// until
#define LEX_LOOP					'l'				// loop
#define LEX_ELSE					'e'				// else
#define LEX_END						'd'				// end

#define LEX_TYPE					't'				// тип
#define LEX_ID						'i'				// идентификатор
#define LEX_SYMBOL_LITERAL			'c'				// symbol
#define LEX_STRING_LITERAL			's'				// строковый литерал
#define LEX_NUMERICAL_LITERAL		'n'				// числовой литерал
#define LEX_FUNC_CALL				'@'				// вызов функции

#define LEX_DECLARE					'$'				// $
#define LEX_SEMICOLON				';'				// ;
#define LEX_COMMA					','				// ,
#define LEX_LEFTBRACE				'{'				// {
#define LEX_BRACELET				'}'				// }
#define LEX_LEFTHESIS				'('				// (
#define LEX_RIGHTHESIS				')'				// )
#define LEX_ACTION					'v'
#define LEX_PLUS					'+'				// +
#define LEX_MINUS					'-'				// -
#define LEX_STAR					'*'				// *
#define LEX_DIRSLASH				'/'				// /
#define LEX_EQUAL					'='				// =
#define LEX_MORE					'w'				// >
#define LEX_LESS					'w'				// <
#define LEX_COMPARE_EQUAL			'w'				// ==
#define LEX_COMPARE_LESS			'w'				// <=
#define LEX_COMPARE_MORE			'w'				// >=

namespace LT									// таблица лексем
{
	struct Entry								// строка таблицы лексем
	{
		char lexema;							// лексема
		int lineSource;							// номер строки в исходном коде
		int indexIdTable;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		char* sign;
		int numberOfParams;
		Entry();
		Entry(char l, int n);
		Entry(char l, int ls, int i, char* s);
	};

	struct LexTable								// экзэмпл€р таблицы лексем
	{
		int maxsize;							// Ємкость таблицы лексем < LT_MAXSIZE
		int size;								// текущий размер таблицы лексем < maxsize
		std::vector<int> refsToAssigns;			// индексы таблицы лексем, которые содержат =
		Entry* table;							// массив строк таблицы лексем
	};

	LexTable Create(int size);					// создать таблицу лексем, Ємкость таблицы лексем < LT_MAXSIZE
	void Add(LexTable& lextable, Entry entry);	// добавить строку в таблицу лексем 
	Entry GetEntry(LexTable& lextable, int n);	// получить строку таблицы лексем, n - номер получаемой строки
	void Delete(LexTable& lextable);			// удалить таблицу лексем (освободить пам€ть)
	void AddList(LexTable& lextable, std::list <LT::Entry> list_of_LT_Entries);
}