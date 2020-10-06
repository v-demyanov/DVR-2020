
#include "In.h"
#include "Headers.h"
#include <list>

using namespace In;
namespace LR
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);

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
		strcpy(result.id, id);
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
		strcpy(result.id, id);
		result.iddatatype = iddatatype;
		result.idtype = idtype;
		result.value.vstr->str = str;
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
			if (currentId.id == id)
				result = true;
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
						break;
					}
					case FST::COMMON_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						switch (lexema.lex)
						{
						case LEX_INTEGER:
						{
							isInteger = true;
							break;
						}
						case LEX_STRING:
						{
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
							break;
						}
						}
						break;
					}
					case FST::LEX_ID_TABLE:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						if (!findId(list_Of_Current_Ids, lexStr))
						{
							if (pointerInDeclareFunc && isInteger)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::F, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isInteger = false;
							}
							else if (pointerInDeclareFunc && isString)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::F, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::F);
								list_Of_Current_Ids.push_back(currentId);
								pointerInDeclareFunc = false;
								isString = false;
							}
							else if (pointerInProperties && isInteger)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::P, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
							}
							else if (pointerInProperties && isString)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::P, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::P);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
							}
							else if (isInteger)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::V, TI_INT_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isInteger = false;
							}
							else if (isString)
							{
								itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::V, TI_STR_DEFAULT);
								IT::Add(idTable, itemIT);
								Id currentId = CreateCurrentId(lexStr, IT::V);
								list_Of_Current_Ids.push_back(currentId);
								isString = false;
							}

						}
						break;
					}
					case FST::ACTION_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::LITERAL_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
						LT::Add(lexTable, itemLT);
						break;
					}
					case FST::PRIORITY_LEX:
					{
						itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
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
				else if ((in.myStr[i][j] == SPACE) || (ASCII_Table[(int)(in.myStr[i][j])] == in.S) || (in.myStr[i][j] == END_OF_STR))
				{
					lexStr[k] = '\0';
					lexema = FST::CheckLexem(lexStr);
					lextype = lexema.lex_type;
					switch (lextype)
					{
						case FST::LEX_NOT_FOUND:
						{
							break;
						}
						case FST::COMMON_LEX:
						{
							itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
							LT::Add(lexTable, itemLT);
							switch (lexema.lex)
							{
								case LEX_INTEGER:
								{
									isInteger = true;
									break;
								}
								case LEX_STRING:
								{
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
									break;
								}
							}
							break;
						}
						case FST::LEX_ID_TABLE:
						{
							itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
							LT::Add(lexTable, itemLT);
							if (!findId(list_Of_Current_Ids, lexStr))
							{
								if (pointerInDeclareFunc && isInteger)
								{																		
									itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::F, TI_INT_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::F);
									list_Of_Current_Ids.push_back(currentId);
									pointerInDeclareFunc = false;
									isInteger = false;
								}
								else if (pointerInDeclareFunc && isString)
								{
									itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::F, TI_STR_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::F);
									list_Of_Current_Ids.push_back(currentId);
									pointerInDeclareFunc = false;
									isString = false;
								}
								else if (pointerInProperties && isInteger)
								{
									itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::P, TI_INT_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::P);
									list_Of_Current_Ids.push_back(currentId);
									isInteger = false;
								}
								else if (pointerInProperties && isString)
								{
									itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::P, TI_STR_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::P);
									list_Of_Current_Ids.push_back(currentId);
									isString = false;
								}
								else if (isInteger)
								{
									itemIT = CreateId(lexTable.size, lexStr, IT::INT, IT::V, TI_INT_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::V);
									list_Of_Current_Ids.push_back(currentId);
									isInteger = false;
								}
								else if (isString)
								{
									itemIT = CreateId(lexTable.size, lexStr, IT::STR, IT::V, TI_STR_DEFAULT);
									IT::Add(idTable, itemIT);
									Id currentId = CreateCurrentId(lexStr, IT::V);
									list_Of_Current_Ids.push_back(currentId);
									isString = false;
								}

							}
							break;
						}
						case FST::ACTION_LEX:
						{
							itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
							LT::Add(lexTable, itemLT);
							break;
						}
						case FST::LITERAL_LEX:
						{
							itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
							LT::Add(lexTable, itemLT);
							break;
						}
						case FST::PRIORITY_LEX:
						{
							itemLT = CreateLex(lexema.lex, i, TI_NULLIDX);
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

		//ׂוסעûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûû
		std::cout << "\t......Test......" << std::endl;
		for (int i = 0; i < lexTable.size; i++)
		{
			std::cout << "\t\t" << lexTable.table[i].lexema << std::endl;
		}

		std::cout << "\n\n\t\t////////////////////////" << std::endl;
		for (int i = 0; i < idTable.size; i++)
		{
			std::cout << "\t\t" << idTable.table[i].id << "\t" << idTable.table[i].idtype << std::endl;
		}
		//ׂוסעûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûûû
		tables.lexTable = lexTable;
		tables.idTable = idTable;
		return tables;
	}
}
