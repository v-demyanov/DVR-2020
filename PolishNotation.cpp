#include <iostream>
#include <stack>
#include <list>
#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"

namespace PN
{
	bool PolishNotation(int lextable_pos, LT::LexTable&	lextable, IT::IdTable& idtable)
	{
		std::list <LT::Entry> list_of_LT_Entries;
		std::stack <LT::Entry> stack_of_arithmeticals;
		for (int i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_NUMERICAL_LITERAL ||
				lextable.table[i].lexema == LEX_STRING_LITERAL)
			{
				list_of_LT_Entries.push_back(lextable.table[i]);
			}
			else if (lextable.table[i].lexema == LEX_PLUS)
			{
				stack_of_arithmeticals.push(lextable.table[i]);
			}
			else if (lextable.table[i].lexema == LEX_LEFTHESIS || lextable.table[i].lexema == LEX_COMMA)
			{
				continue;
			}
			else if (lextable.table[i].lexema == LEX_RIGHTHESIS)
			{
				if (!stack_of_arithmeticals.empty())
				{
					list_of_LT_Entries.push_back(stack_of_arithmeticals.top());
					stack_of_arithmeticals.pop();
				}
			}
		}

		while (stack_of_arithmeticals.size() != 0)
		{
			list_of_LT_Entries.push_back(stack_of_arithmeticals.top());
			stack_of_arithmeticals.pop();
		}
		std::cout << "\n\n\tВыражение в польской записи:\t";
		for (auto iter = list_of_LT_Entries.begin(); iter != list_of_LT_Entries.end(); iter++)
		{
			std::cout << (*iter).lexema << "\t";
		}
		std::cout << std::endl;
		return true;
	}
}