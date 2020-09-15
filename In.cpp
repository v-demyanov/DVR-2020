
#include "Error.h"
#include "In.h"
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <string>

namespace In
{
	IN getin(wchar_t infile[])
	{
		std::fstream file;
		file.open(infile, std::fstream::in | std::fstream::out);
		if (!file.is_open())
			throw ERROR_THROW(110);
		In::IN structIN;
		structIN.size = 0;
		structIN.ignor = 0;
		structIN.lines = 1;
		int ASCII_Table[] = IN_CODE_TABLE;
		char symbol, prevSymbol;
		int index;
		int letter = 0;
		int word = 0;
		char** str = new char* [LEN];
		for (int i = 0; i < LEN; i++)
			str[i] = new char[LEN];

		for (int col = 1; file.get(symbol); col++)
		{
			structIN.size++;

			if ((int)symbol < 0)
				index = (int)symbol + 256;
			else
				index = (int)symbol;

			if (ASCII_Table[index] == structIN.F)
			{
				throw ERROR_THROW_IN(111, structIN.lines, col);
			}
			else if (ASCII_Table[index] == structIN.T)
			{
				if (symbol == '\n')
				{
					structIN.lines++;
					col = 1;

					/*if (prevSymbol != '|')
					{
						str[word][letter] = '\0';
						word++;
					}*/
					
					str[word][letter] = '?';
					letter++;
					str[word][letter] = '\0';
					word++;
					letter = 0;
					continue;
				}

				/*if (symbol == '|')
				{
					str[word][letter] = '\0';
					word++;
					letter = 0;
					prevSymbol = symbol;
					continue;
				}*/

				str[word][letter] = symbol;
				prevSymbol = symbol;
				letter++;
			}
			else if (ASCII_Table[index] == structIN.I)
			{
				structIN.ignor++;
			}
			else if (ASCII_Table[index] != symbol)
			{
				symbol = (char)ASCII_Table[index];
			}
		}

		//*
		str[word][letter] = '?';
		letter++;

		if (symbol != '|')
		{
			str[word][letter] = '\0';
			word++;
		}

		structIN.myStr = str;
		structIN.lmyStr = word;
		file.close();
		return structIN;
	}
};
