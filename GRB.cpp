#include "GRB.h"
#include <string.h>
#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(
		NS('S'), TS('$'),			// стартовый символ, дно стека NS-нетерминал(большие буквы),TS-терминал
		6, 							// количество правил
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверна€ структура программы  
			3,                                // S->tfi(F){N}S | m{N};
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('i'), TS('f'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,   // ошибочный оператор
			8,                               // N->vti;N | rE; | i=E;N | o(i);N | o(y);N |o(x);N |o(y); |o(i);|o(x); 
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,		// ошибка в выражении
			8,									// E->i | y | x | (E) | xM | i(W) | iM | vM | (E)M | i(W)M | p(i,x) | p(x,i)| p(x,x) |p(i,i) |n(y) |n(i) |
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),// три следующих узла неправильные, временное
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверна€ структура программы  
			3,                                // S->tfi(F){N}S | m{N};
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('i'), TS('f'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S'))
		),
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверна€ структура программы  
			3,                                // S->tfi(F){N}S | m{N};
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('i'), TS('f'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S'))
		),
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверна€ структура программы  
			3,                                // S->tfi(F){N}S | m{N};
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('i'), TS('f'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  // онструктор цепочки - правой части правила
	{				//(кол-во символов в цепочке, терминал или нетерминал)
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	//  онструктор правила
	{		//(нетерминал, идентификатор диагностического сообщени€, количество цепочек(правых частей правила), ...
																				// ... множество цепочек (правых частей правила)
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)	//  онструктор грамматики √рейбаха
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

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)	// ѕолучить правило по нетерминалу
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)		//пока   меньше кол-ва правил и пока левый символ правила не равен парметру ф-ции?????????
			k++;
		if (k < size)
			prule = rules[rc = k];		//возвращаемое правило граматики равно правилу с индексом  
		return rc;			//возвращаетс€ номер правила или -1
	};

	Rule Greibach::getRule(short n)		// ѕолучить правило по номеру
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)		//ѕолучить правило в виде N->цепочка 
	{												//(буфер, номер цепочки(правой части) в правиле)
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);	// добавл€ем строку (куда, с какого элемента, строку)
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)	// ѕолучить следующую за j подход€щую цепочку, вернуть еЄ номер или -1 
	{					  //(первый символ цепочки, возвращаема€ цепочка, номер цепочки)
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