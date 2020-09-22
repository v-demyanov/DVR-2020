#pragma once
#include "Headers.h"
#define LEXEMA_FIXSIZE				1				// фикс размер лексемы
#define LT_MAXSIZE					4096			// макс кол-во строк в таблице лексем
#define LT_TI_NULLIDX				0xffffffff		// нет элемента таблицы идентификаторов
#define LEX_MAIN					'm'				// main
#define LEX_INTEGER					't'				// integer
#define LEX_STRING					't'				// string
#define LEX_ID						'i'				// идентификатор
#define LEX_LITERAL					'l'				// литерал
#define LEX_STRING_LITERAL			'sl'			// строковый литерал
#define LEX_NUMERICAL_LITERAL		'nl'			// числовой литерал
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

namespace LT									// таблица лексем
{
	struct Entry								// строка таблицы лексем
	{
		char lexema;							// лексема
		int sn;									// номер строки в исходном коде
		int idxTI;								// индекс в таблице идентификаторов или LT_TI_NULLIDX
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
	Entry CheckLexem(const char* lex);
}
