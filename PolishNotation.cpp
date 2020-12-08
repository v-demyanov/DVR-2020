#include <iostream>
#include <stack>
#include <list>
#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"
#include <map>

namespace PN
{
	int getPriority(std::string l)
	{
		std::map <std::string, int> mapping;

		mapping["*"] = 3;
		mapping["/"] = 3;
		mapping["-"] = 2;
		mapping["+"] = 2;
		mapping["("] = 1;

		switch (mapping[l]) {
		case 3:  return 3;  break;
		case 2:  return 2;  break;
		case 1:  return 1; break;
		}

		/*switch (l)
		{
			case (mult): case div: return 3;
			case minus: case plus: return 2;
			case brace: return 1;
		}*/
		return 0;
	}

	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		std::list <LT::Entry> list_of_LT_Entries;
		std::stack <LT::Entry> stack;
		int leftHesisCount = 0;
		int paramCount = 0;
		bool isFunction = false;
		for (int i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_NUMERICAL_LITERAL ||
				lextable.table[i].lexema == LEX_STRING_LITERAL)
			{
				if (idtable.table[lextable.table[i].indexIdTable].idtype == IT::F)
				{
					isFunction = true;
					paramCount = 0;
				}

				if (isFunction)
					paramCount++;
					
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
				if (isFunction)
				{
					char* test = new char[2];
					strcpy(test, " ");
					list_of_LT_Entries.push_back({ '@', -1, -1, test });
					isFunction = false;
					paramCount = 0;
				}
			}
			else if (lextable.table[i].lexema == LEX_COMMA)
			{
				continue;
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
			if (strcmp((*iter).sign, SIGN_DEFAULT) == 0)
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