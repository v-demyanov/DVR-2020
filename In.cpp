
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
		char symbol, nextSymbol = 'a';
		int index, prevSymbolId = NULL, nextSymbolId = NULL;
		int letter = 0;
		int word = 0;
		char** str = new char* [LEN];
		for (int i = 0; i < LEN; i++)
			str[i] = new char[LEN];

		for (int col = 1; file.get(symbol); col++)
		{
			structIN.size++;
			file.get(nextSymbol);
			file.unget();

			if ((int)symbol < 0)
			{
				index = (int)symbol + 256;
				nextSymbolId = (int)nextSymbol + 256;
			}	
			else
			{
				index = (int)symbol;
				nextSymbolId = (int)nextSymbol;
			}
				
			if (ASCII_Table[index] == structIN.F)
			{
				throw ERROR_THROW_IN(111, structIN.lines, col);
			}
			else if (ASCII_Table[index] == structIN.I || (symbol == SPACE && (ASCII_Table[prevSymbolId] == structIN.S || ASCII_Table[nextSymbolId] == structIN.S)))
			{
				structIN.ignor++;
			}
			else if (ASCII_Table[index] == structIN.T || ASCII_Table[index] == structIN.S)
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
				prevSymbolId = (int)symbol;
				letter++;
			}
			else if (ASCII_Table[index] != symbol)
			{
				symbol = (char)ASCII_Table[index];
			}
		}

		//*
		str[word][letter] = END_OF_STR;
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
