
#include "In.h"
#include "Headers.h"

using namespace In;
namespace LR
{
	//Tables FillingTables(LT::LexTable& lexTable, IT::IdTable& idTable, In::IN in)
	void FillingTables(In::IN in)
	{
		//Tables tables;
		FST::FST_RESULT lexema;
		
		int ASCII_Table[] = IN_CODE_TABLE;
		char* lexStr = new char[LEN];
		char* result = new char[LEN];
		int t = 0;
		result[t] = '\0';
		for (int i = 0, k = 0; i < in.lmyStr; i++)
		{
			lexStr[k] = '\0';
			for (int j = 0; in.myStr[i][j] != '\0'; j++)
			{
				if (ASCII_Table[(int)in.myStr[i][j]] == in.S && k == 0)
				{				
					lexStr[k] = in.myStr[i][j];
					lexStr[++k] = '\0';
					lexema = FST::CheckLexem(lexStr);
					/*switch (lexema.lex_type)
					{

					}*/
					result[t++] = lexema.lex;
					k = 0;
				}
				else if ((in.myStr[i][j] == SPACE) || (ASCII_Table[(int)(in.myStr[i][j])] == in.S) || (in.myStr[i][j] == END_OF_STR))
				{
					lexStr[k] = '\0';
					lexema = FST::CheckLexem(lexStr);
					/*switch (lexema.lex_type)
					{

					}*/
					result[t++] = lexema.lex;
					if (ASCII_Table[(int)in.myStr[i][j]] == in.S)
					{
						j--;
					}
					k = 0;
				}
				else
				{
					lexStr[k++] = in.myStr[i][j];
				}
			}
		}
		
		/*tables.idTable = idTable;
		tables.lexTable = lexTable;
		return tables;*/
	}
}

//while (in.myStr[i][j] != END_OF_STR)
//{
//	if (in.myStr[i][j] == in.S && k == 0)
//	{
//		lexStr[k] = in.myStr[i][j];
//		lexema = FST::CheckLexem(lexStr);
//
//	}
//	else if (in.myStr[i][j] == SPACE || in.myStr[i][j] == in.S)
//	{
//		lexema = FST::CheckLexem(lexStr);
//		if (in.myStr[i][j] == in.S)
//		{
//			j--;
//		}
//		k = 0;
//	}
//	else
//	{
//		lexStr[++k] = in.myStr[i][j];
//	}
//}