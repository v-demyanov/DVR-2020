#include <iostream>
#include <stack>
#include <list>
#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"

namespace PN
{
	/*bool PolishNotation(int lextable_pos, LT::LexTable&	lextable, IT::IdTable& idtable)
	{
		std::list <LT::Entry> list_of_LT_Entries;
		std::stack <LT::Entry> stack_of_arithmeticals;
		int leftHesisCount = 0;
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
				leftHesisCount++;
				continue;
			}
			else if (lextable.table[i].lexema == LEX_RIGHTHESIS)
			{
				if (!stack_of_arithmeticals.empty())
				{
					list_of_LT_Entries.push_back(stack_of_arithmeticals.top());
					stack_of_arithmeticals.pop();
					leftHesisCount--;
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
			if ((*iter).sign == TI_NULLIDX)
			{
				std::cout << (*iter).lexema << "\t";
			}
			else
			{
				std::cout << (*iter).sign << "\t";
			}
			
		}
		std::cout << std::endl;
		return true;
	}*/

	int getPriority(char l)
	{
		switch (l)
		{
			case '*': case '/': return 3;
			case '-': case '+': return 2;
			case '(': return 1;
		}
		return 0;
	}

	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		std::list <LT::Entry> list_of_LT_Entries;
		std::stack <LT::Entry> stack;
		int leftHesisCount = 0;
		for (int i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_NUMERICAL_LITERAL ||
				lextable.table[i].lexema == LEX_STRING_LITERAL)
			{
				list_of_LT_Entries.push_back(lextable.table[i]);
			}
			else if (lextable.table[i].lexema == LEX_LEFTHESIS)
			{
				stack.push(lextable.table[i]);
			}
			else if (lextable.table[i].lexema == LEX_RIGHTHESIS)
			{
				while (!stack.empty() && stack.top().lexema != LEX_LEFTHESIS)
				{
					list_of_LT_Entries.push_back(stack.top());
					stack.pop();
				}
				if (stack.top().lexema == LEX_LEFTHESIS)
				{
					stack.pop();
				}
			}
			else
			{
				while (!stack.empty() && getPriority(lextable.table[i].sign) <= getPriority(stack.top().sign))
				{
					list_of_LT_Entries.push_back(stack.top());
					stack.pop();
				}
				stack.push(lextable.table[i]);
			}
		}

		while (stack.size() != 0)
		{
			list_of_LT_Entries.push_back(stack.top());
			stack.pop();
		}
		std::cout << "\n\n\tВыражение в польской записи:\t";
		for (auto iter = list_of_LT_Entries.begin(); iter != list_of_LT_Entries.end(); iter++)
		{
			if ((*iter).sign == TI_NULLIDX)
			{
				std::cout << (*iter).lexema << "\t";
			}
			else
			{
				std::cout << (*iter).sign << "\t";
			}

		}
		std::cout << std::endl;
		return true;
	}

}