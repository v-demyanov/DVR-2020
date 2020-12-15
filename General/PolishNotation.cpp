#include <iostream>
#include <stack>
#include <list>
#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"
#include <map>

namespace PN
{
	int getPriority(std::string l)	//	приоритет операции
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
		return 0;
	}
	//	создание новой таблице лексем с преобразованными выражениями
	LT::LexTable PolishNotation(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		int ref = 0;
		int semicolon;
		std::list <LT::Entry> list_of_LT_Entries;
		LT::LexTable result = LT::Create(LT_MAXSIZE);
		for (int i = 0; i < lextable.size; i++)
		{
			if (ref < lextable.refsToAssigns.size() && i == lextable.refsToAssigns[ref])
			{
				LT::Add(result, lextable.table[i]);
				list_of_LT_Entries = ConvertToPN(i + 1, lextable, idtable, semicolon);
				LT::AddList(result, list_of_LT_Entries);
				i = semicolon;
				ref++;
			}
			LT::Add(result, lextable.table[i]);
		}
		return result;
	}
	//	трансформация выражения в форму обратной польской записи 
	std::list <LT::Entry> ConvertToPN(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable, int& semicolon)
	{
		std::list <LT::Entry> list_of_LT_Entries;
		std::stack <LT::Entry> stack;
		LT::Entry func;
		int leftHesisCount = 0;
		int paramCount = 0;
		bool isFunction = false;
		int i;
		for (i = lextable_pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_NUMERICAL_LITERAL ||
				lextable.table[i].lexema == LEX_STRING_LITERAL)
			{
				if (idtable.table[lextable.table[i].indexIdTable].idtype == IT::F)
				{
					isFunction = true;
					paramCount = 0;
					lextable.table[i].lexema = LEX_FUNC_CALL;
					func = lextable.table[i];
				}
				else
				{
					if (isFunction)
						paramCount++;

					list_of_LT_Entries.push_back(lextable.table[i]);
				}
	
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
					func.numberOfParams = paramCount;
					list_of_LT_Entries.push_back(func);
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
				
				if ((*iter).lexema == LEX_FUNC_CALL)
				{
					std::cout << (*iter).lexema << " ";
					std::cout << (*iter).numberOfParams << "\t";
				}
				else
				{
					std::cout << (*iter).lexema << "\t";
				}
			}
			else
			{
				std::cout << (*iter).sign << "\t";
			}
		}
		std::cout << std::endl;
		semicolon = i;
		return list_of_LT_Entries;
	}

}