
#include "Headers.h"


namespace LR
{
	/*Tables FillingTables(LT::LexTable& lexTable, IT::IdTable& idTable, In::IN in)
	{
		LT::Entry lexStr;
		char* lex;
		int k = 0;
		for (int i = 0, int j = 0; i < in.lmyStr; i++)
		{
			while (in.myStr[i][j] != END_OF_STR)
			{
				if (in.myStr[i][j] == in.S && k == 0)
				{
					lexStr = LT::CheckLexem(lex, i);
					lex[k] = in.myStr[i][j];
					LT::Add(lexTable, lexStr);
					j++;
				}
				else if (in.myStr[i][j] == SPACE || in.S)
				{
					lexStr = LT::CheckLexem(lex, i);
					LT::Add(lexTable, lexStr);
					k = 0;
					j++;
				}
				else
				{
					lex[k] = in.myStr[i][j];
					j++;
				}	
			}
		}

		return { lexTable, idTable };
	}*/
}