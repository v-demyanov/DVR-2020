#include "GRB.h"
#include <string.h>
#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(
		NS('S'), TS('$'),			// стартовый символ, дно стека NS-нетерминал(большие буквы),TS-терминал
		7, 							// количество правил
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверная структура программы  
			4,		// S->g{NrE;}; | tfi(F){NrE;};S | g{NrE;};S | tfi(F){NrE;};
			Rule::Chain(8, TS('g'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(9, TS('g'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,   // ошибочный оператор
			24,		// N->$ti; | rE; | i=E; | $tfi(F); | $ti;N | rE;N | i=E;N | $tfi(F);N | pi; | pn; | ps; | pc; |  pi;N | pn;N | ps;N | pc;N |pi(F); | pi(F);N | u(I)Nl; | u(I)Nl;N | q(I)NeNd; | q(I)NeNd;N | q(I)Nd; | q(I)NeNd;N
			Rule::Chain(4, TS('$'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('$'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('$'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('$'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), TS('i'), TS(';')),
			Rule::Chain(3, TS('p'), TS('n'), TS(';')),
			Rule::Chain(3, TS('p'), TS('s'), TS(';')),
			Rule::Chain(3, TS('p'), TS('c'), TS(';')),
			Rule::Chain(4, TS('p'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('p'), TS('n'), TS(';'), NS('N')),
			Rule::Chain(4, TS('p'), TS('s'), TS(';'), NS('N')),
			Rule::Chain(4, TS('p'), TS('c'), TS(';'), NS('N')),
			Rule::Chain(6, TS('p'), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(7, TS('p'), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(7, TS('u'), TS('('), NS('I'), TS(')'), NS('N'), TS('l'), TS(';')),
			Rule::Chain(8, TS('u'), TS('('), NS('I'), TS(')'), NS('N'), TS('l'), TS(';'), NS('N')),
			Rule::Chain(9, TS('q'), TS('('), NS('I'), TS(')'), NS('N'), TS('e'), NS('N'), TS('d'), TS(';')),
			Rule::Chain(10, TS('q'), TS('('), NS('I'), TS(')'), NS('N'), TS('e'), NS('N'), TS('d'), TS(';'), NS('N')),
			Rule::Chain(7, TS('q'), TS('('), NS('I'), TS(')'), NS('N'), TS('d'), TS(';')),
			Rule::Chain(8, TS('q'), TS('('), NS('I'), TS(')'), NS('N'), TS('d'), TS(';'), NS('N'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,		// ошибка в выражении
			12,		// E->i | c | (E) | i(W) | iM | cM | (E)M | i(W)M | n | s | nM | sM
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('n')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('c'), NS('M')),
			Rule::Chain(2, TS('n'), NS('M')),
			Rule::Chain(2, TS('s'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 3,		// оператор
			2,		// M->vE | vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 4,		// ошибка в параметрах функции
			2,		// F->ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 5,		// ошибка в параметрах вызываемой функции 
			8,		// W->i | c | i,W | c,W | n | s | n,W | s,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('n')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('c'), TS(','), NS('W')),
			Rule::Chain(3, TS('n'), TS(','), NS('W')),
			Rule::Chain(3, TS('s'), TS(','), NS('W'))
		),
		Rule(
			NS('I'), GRB_ERROR_SERIES + 6,		// ошибка в параметрах цикла или оператора ветвления ?????
			4, // iwc | iws | iwn | iwi
			Rule::Chain(3, TS('i'), TS('w'), TS('c')),
			Rule::Chain(3, TS('i'), TS('w'), TS('s')),
			Rule::Chain(3, TS('i'), TS('w'), TS('n')),
			Rule::Chain(3, TS('i'), TS('w'), TS('i'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //Конструктор цепочки - правой части правила
	{				//(кол-во символов в цепочке, терминал или нетерминал)
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	// Конструктор правила
	{		//(нетерминал, идентификатор диагностического сообщения, количество цепочек(правых частей правила), ...
																				// ... множество цепочек (правых частей правила)
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)	// Конструктор грамматики Грейбаха
	{											//(стартовый символ, дно стека, количество правил, правила)
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++)
			rules[i] = p[i];
	};

	Greibach getGreibach()
	{
		return greibach;
	};

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)	// Получить правило по нетерминалу
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)		//пока К меньше кол-ва правил и пока левый символ правила не равен парметру ф-ции?????????
			k++;
		if (k < size)
			prule = rules[rc = k];		//возвращаемое правило граматики равно правилу с индексом К
		return rc;			//возвращается номер правила или -1
	};

	Rule Greibach::getRule(short n)		// Получить правило по номеру
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)		//Получить правило в виде N->цепочка 
	{												//(буфер, номер цепочки(правой части) в правиле)
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);	// добавляем строку (куда, с какого элемента, строку)
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)	// Получить следующую за j подходящую цепочку, вернуть её номер или -1 
	{					  //(первый символ цепочки, возвращаемая цепочка, номер цепочки)
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
			++j;
		rc = (j < size ? j : -1);

		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);	// символьный массив из символов нашей цепочки
		b[size] = 0x00;
		return b;
	};
}