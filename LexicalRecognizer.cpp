
#include "In.h"
#include "Headers.h"
#include <list>
#include <iomanip>
#include <string.h>

using namespace In;
namespace LR
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);

	LT::Entry CreateLex(char lex, int lineSource, int indexIdTable, char sign)
	{
		LT::Entry result;
		result.lexema = lex;
		result.lineSource = lineSource;
		result.indexIdTable = indexIdTable;
		result.sign = sign;
		return result;
	}

	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, int vint)
	{
		IT::Entry result;
		result.id = new char[ID_MAXSIZE];
		result.idxfirstLE = idxfirstLE;
		strcpy(result.id, id);
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		result.value.vint = vint;
		return result;
	}

	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, char str)
	{
		IT::Entry result;
		result.id = new char[ID_MAXSIZE];
		result.value.vstr->str = new char[TI_STR_MAXSIZE];
		result.idxfirstLE = idxfirstLE;
		strcpy(result.id, id);
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		result.value.vstr->str[0] = str;
		result.value.vstr->str[1] = '\0';
		return result;
	}

	IT::Entry CreateId(int idxfirstLE, char id[ID_MAXSIZE], IT::IDDATATYPE iddatatype, IT::IDTYPE idtype, char str[TI_STR_MAXSIZE - 1])
	{
		IT::Entry result;
		result.id = new char[ID_MAXSIZE];
		result.value.vstr->str = new char[TI_STR_MAXSIZE];
		result.idxfirstLE = idxfirstLE;
		strcpy(result.id, id);
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		strcpy(result.value.vstr->str, str);
		result.value.vstr->len = strlen(result.value.vstr->str);
		return result;
	}

	struct Id
	{
		char* id;
		IT::IDTYPE idtype;
	};

	Id CreateCurrentId(char* id, IT::IDTYPE idtype)
	{
		Id result;
		result.id = new char[ID_MAXSIZE];
		strcpy(result.id, id);
		result.idtype = idtype;
		return result;
	}

	void removeItemFromSight(std::list<Id>& arr)
	{
		Id currentId;
		for (auto item = arr.begin(); item != arr.end(); item++)
		{
			currentId = *item;
			if (currentId.idtype != IT::F)
			{
				item = arr.erase(item);
				item--;
			}
		}
	}

	bool findId(std::list<Id> arr, char* id)
	{
		bool result = false;
		Id currentId;
		for (auto item = arr.begin(); item != arr.end(); item++)
		{
			currentId = *item;
			if (strcmp(currentId.id, id) == 0)
			{
				result = true;
			}
		}
		return result;
	}

	Tables FillingTables(In::IN in)
	{
		bool pointerInDeclareFunc = false,
			pointerInProperties = false,
			pointerInFunc = false,
			pointerInMain = false,
			isInteger = false,
			isString = false;

		int beginPosition = 0;

		std::list<Id> list_Of_Current_Ids;
		Tables tables;
		FST::FST_RESULT lexema;
		FST::LEXTYPE lextype;
		LT::Entry itemLT;
		IT::Entry itemIT;
		IT::IDDATATYPE iddatatype;
		IT::IDTYPE idtype;
		int ASCII_Table[] = IN_CODE_TABLE;
		char* lexStr = new char[LEN];

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
					lextype = lexema.lex_type;
					switch (lextype)
					{
					case FST::LEX_NOT_FOUND:
					{
						if (lexStr[0] != '\0')
							throw ERROR_THROW_IN(120, i, j);
						break;
					}
					case FST::COMMON_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						switch (lexema.lex)
						{
						case LEX_TYPE:
						{
							if (strcmp(lexStr, "integer") == 0)
								isInteger = true;
							if (strcmp(lexStr, "string") == 0)
								isString = true;
							break;
						}
						case LEX_FUNCTION:
						{
							pointerInDeclareFunc = true;
							break;
						}
						case LEX_MAIN:
						{
							pointerInMain = true;
							break;
						}
						case LEX_LEFTBRACE:
						{
							pointerInFunc = true;
							break;
						}
						case LEX_BRACELET:
						{
							removeItemFromSight(list_Of_Current_Ids);
							pointerInFunc = false;
							pointerInMain = false;
							beginPosition = 0;
							break;
						}
						}
						break;
					}
					case FST::LEX_ID_TABLE:
					{
						IT::IDDATATYPE currentIDDT = IT::NDT;
						IT::IDTYPE currentIDT = IT::NT;
						if (!findId(list_Of_Current_Ids, lexStr))
						{
							if (pointerInDeclareFunc && isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::F, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::F;
							}
							else if (pointerInDeclareFunc && isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::F, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::F;
							}
							else if (pointerInProperties && isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::P, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::P;
							}
							else if (pointerInProperties && isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::P, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::P;
							}
							else if (isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::V, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::V;
							}
							else if (isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::V, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::V;
							}
							beginPosition++;
						}

						int indexIdTable = IsId(idTable, lexStr, beginPosition);
						if (indexIdTable == -1 && findId(list_Of_Current_Ids, lexStr))
						{
							indexIdTable = IsId(idTable, lexStr, idTable.size);
						}
						itemLT = CreateLex(lexema.lex, i, indexIdTable, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::ACTION_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, lexStr[0]);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::LITERAL_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::PRIORITY_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						if (lexema.lex == LEX_LEFTHESIS)
							pointerInProperties = true;
						else if (lexema.lex == LEX_RIGHTHESIS)
							pointerInProperties = false;
						break;
					}
					}
					k = 0;
				}
				else if ((in.myStr[i][j] == SPACE) || (ASCII_Table[(int)(in.myStr[i][j])] == in.S) || (in.myStr[i][j] == END_OF_STR && in.myStr[i][j + 1] == '\0'))
				{
					lexStr[k] = '\0';
					lexema = FST::CheckLexem(lexStr);
					lextype = lexema.lex_type;
					switch (lextype)
					{
					case FST::LEX_NOT_FOUND:
					{
						if (lexStr[0] != '\0')
							throw ERROR_THROW_IN(120, i, j);
						break;
					}
					case FST::COMMON_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						switch (lexema.lex)
						{
						case LEX_TYPE:
						{
							if (strcmp(lexStr, "integer") == 0)
								isInteger = true;
							if (strcmp(lexStr, "string") == 0)
								isString = true;
							break;
						}
						case LEX_FUNCTION:
						{
							pointerInDeclareFunc = true;
							break;
						}
						case LEX_MAIN:
						{
							pointerInMain = true;
							break;
						}
						case LEX_LEFTBRACE:
						{
							pointerInFunc = true;
							break;
						}
						case LEX_BRACELET:
						{
							removeItemFromSight(list_Of_Current_Ids);
							pointerInFunc = false;
							pointerInMain = false;
							beginPosition = 0;
							break;
						}
						}
						break;
					}
					case FST::LEX_ID_TABLE:
					{
						IT::IDDATATYPE currentIDDT = IT::NDT;
						IT::IDTYPE currentIDT = IT::NT;

						if (!findId(list_Of_Current_Ids, lexStr))
						{
							if (pointerInDeclareFunc && isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::F, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::F;
							}
							else if (pointerInDeclareFunc && isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::F, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::F;
							}
							else if (pointerInProperties && isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::P, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::P;
							}
							else if (pointerInProperties && isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::P, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::P;
							}
							else if (isInteger)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::V, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
								currentIDDT = IT::INT;
								currentIDT = IT::V;
							}
							else if (isString)
							{
								if (strlen(lexStr) > ID_MAXSIZE)
									throw ERROR_THROW_IN(121, i, j);
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::V, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
								currentIDDT = IT::STR;
								currentIDT = IT::V;
							}
							beginPosition++;
						}

						int indexIdTable = IsId(idTable, lexStr, beginPosition);
						if (indexIdTable == -1 && findId(list_Of_Current_Ids, lexStr))
						{
							indexIdTable = IsId(idTable, lexStr, idTable.size);
						}

						itemLT = CreateLex(lexema.lex, i, indexIdTable, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::ACTION_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, lexStr[0]);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::LITERAL_LEX:
					{
						if (lexema.lex == LEX_NUMERICAL_LITERAL)
						{
							int lexNum = atoi(lexStr);
							char* lexNumName = new char[ID_MAXSIZE];
							strcpy(lexNumName, "num");
							itemIT = CreateId(lexTable.size, lexNumName, IT::INT, IT::L, lexNum);
							IT::Add(idTable, itemIT);
							beginPosition++;
						}
						else if (lexema.lex == LEX_STRING_LITERAL)
						{
							char* lexStrName = new char[ID_MAXSIZE];
							strcpy(lexStrName, "str");
							itemIT = CreateId(lexTable.size, lexStrName, IT::STR, IT::L, lexStr);
							IT::Add(idTable, itemIT);
							beginPosition++;
						}

						itemLT = CreateLex(lexema.lex, i, idTable.size - 1, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::PRIORITY_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						if (lexema.lex == LEX_LEFTHESIS)
							pointerInProperties = true;
						else if (lexema.lex == LEX_RIGHTHESIS)
							pointerInProperties = false;
						break;
					}
					}
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

		//���������������������������������������������
		std::cout << "\t......Test......" << std::endl;
		for (int i = 0; i < lexTable.size; i++)
		{
			std::cout << "\t\t" << lexTable.table[i].lexema << "\tlineSource " << lexTable.table[i].lineSource << "\tindexIdTable " << lexTable.table[i].indexIdTable << std::endl;
		}

		std::cout << "\n\n\t\t////////////////////////" << std::endl;
		for (int i = 0; i < idTable.size; i++)
		{
			std::cout << "\t\t" << idTable.table[i].id << "\t" << idTable.table[i].idtype << "\tidxfirstLE: " << idTable.table[i].idxfirstLE << std::endl;
		}
		//���������������������������������������������
		tables.lexTable = lexTable;
		tables.idTable = idTable;
		return tables;
	}
}
