
#include "In.h"
#include "Headers.h"

using namespace In;
namespace LR
{
	LT::Entry CreateLex(char lex, int lineSource, int indexIdTable)
	{
		LT::Entry result;
		result.lexema = lex;
		result.lineSource = lineSource;
		result.indexIdTable = indexIdTable;
		return result;
	}

	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, int vint)
	{
		IT::Entry result;
		result.id = new char[ID_MAXSIZE];
		result.idxfirstLE = idxfirstLE;
		result.id = id;
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		result.value.vint = vint;
		return result;
	}

	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, char str[TI_STR_MAXSIZE - 1])
	{
		IT::Entry result;
		result.id = new char[ID_MAXSIZE];
		result.idxfirstLE = idxfirstLE;
		result.id = id;
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		result.value.vstr->str = str;
		return result;
	}
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

		std::cout << "......Test......" << std::endl;
		for (int i = 0; i < t; i++)
		{
			std::cout << "\t" << result[i] << std::endl;
		}
		
		/*tables.idTable = idTable;
		tables.lexTable = lexTable;
		return tables;*/
	}
}
