
#include "SemanticAnalyzer.h"
#include "Error.h"
#include <iostream>

namespace SA
{
	void SemanticAnalyzer(LR::Tables tables)
	{
		for (int i = 0; i < tables.lexTable.size; i++)
		{
			switch (tables.lexTable.table[i].lexema)
			{
				case LEX_FUNCTION:
				{
					int funcIndex = ++i;
					for (; tables.lexTable.table[i].lexema != LEX_RIGHTHESIS; i++)
					{
						if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].idtype == IT::P)
						{
							tables.idTable.table[tables.lexTable.table[funcIndex].indexIdTable].parameters.push_back((tables.idTable.table[tables.lexTable.table[i].indexIdTable].iddatatype));
						}
					}
					break;
				}
				case LEX_EQUAL:
				{
					int resultType = tables.idTable.table[tables.lexTable.table[i - 1].indexIdTable].iddatatype;
					std::vector<int> parameters;
					for (; tables.lexTable.table[i].lexema != LEX_SEMICOLON; i++)
					{
						if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].iddatatype != resultType)
						{
							throw ERROR_THROW_IN(202, tables.lexTable.table[i].lineSource, 0);
						}
						else if (tables.lexTable.table[i].lexema == LEX_SYMBOL_LITERAL && resultType != IT::SYBM)
						{
							throw ERROR_THROW_IN(202, tables.lexTable.table[i].lineSource, 0);
						}
						else if (tables.lexTable.table[i].lexema == LEX_STRING_LITERAL && resultType != IT::STR)
						{
							throw ERROR_THROW_IN(202, tables.lexTable.table[i].lineSource, 0);
						}
						else if (tables.lexTable.table[i].lexema == LEX_NUMERICAL_LITERAL && resultType != IT::INT)
						{
							throw ERROR_THROW_IN(202, tables.lexTable.table[i].lineSource, 0);
						}

						if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].idtype == IT::F)
						{
							int funcIndex = i;
							i++;
							for (; tables.lexTable.table[i].lexema != LEX_RIGHTHESIS; i++)
							{
								if (tables.lexTable.table[i].lexema == LEX_ID)
								{
									parameters.push_back(tables.idTable.table[tables.lexTable.table[i].indexIdTable].iddatatype);
								}
								else if (tables.lexTable.table[i].lexema == LEX_SYMBOL_LITERAL)
								{
									parameters.push_back(IT::SYBM);
								}
								else if (tables.lexTable.table[i].lexema == LEX_STRING_LITERAL)
								{
									parameters.push_back(IT::STR);
								}
								else if (tables.lexTable.table[i].lexema == LEX_NUMERICAL_LITERAL)
								{
									parameters.push_back(IT::INT);
								}
							}
							if (parameters != tables.idTable.table[tables.lexTable.table[funcIndex].indexIdTable].parameters)
								throw ERROR_THROW_IN(203, tables.lexTable.table[i].lineSource, 0);
							parameters.clear();
						}
					}
				}
			}
		}
	}
}