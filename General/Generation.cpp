
#include "Generation.h"
#include "IT.h"
#include <iostream>


namespace GEN
{
	void Generation(LR::Tables tables, Log::LOG asmbl)
	{
		*(asmbl.stream) << ".586\n.MODEL FLAT, STDCALL\n\toption casemap:none\n\tinclude C:\\Program Files\\masm32\\INCLUDE\\kernel32.inc\n\t";
		*(asmbl.stream) << "include C:\\Program Files\\masm32\\INCLUDE\\user32.inc\n\t";
		*(asmbl.stream) << "includelib libucrt.lib\n\tincludelib kernel32.lib\n\tincludelib user32.lib\n\t";
		*(asmbl.stream) << "ExitProcess PROTO : DWORD\n";
		*(asmbl.stream) << ".STACK 4096\n";

		std::string visibility_area;
		std::string variable;
		std::string result_variable;
		std::vector<std::string> variables;

		//	Вывод сегмента констант
		*(asmbl.stream) << ".CONST\n";
		for (int i = 0; i < tables.idTable.size; i++)
		{
			if (tables.idTable.table[i].idtype == IT::L)
			{
				*(asmbl.stream) << "\t" << tables.idTable.table[i].id;
				if (tables.idTable.table[i].iddatatype == IT::STR || tables.idTable.table[i].iddatatype == IT::SYBM)
				{
					*(asmbl.stream) << " BYTE " << tables.idTable.table[i].value.vstr->str << ", 0\n";
				}
				if (tables.idTable.table[i].iddatatype == IT::INT)
				{
					*(asmbl.stream) << " DWORD " << tables.idTable.table[i].value.vint << "\n";
				}
			}
		}
		*(asmbl.stream) << "\tBSIZE equ 15\n";
		
		//	Вывод сегмента данных
		*(asmbl.stream) << ".DATA\n";
		for (int i = 0; i < tables.lexTable.size; i++)
		{
			if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].idtype == IT::F)
			{
				visibility_area = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
			}

			if (tables.lexTable.table[i].lexema == LEX_MAIN)
			{
				visibility_area = "main";
			}

			if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].idtype == IT::V)
			{
				variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
				result_variable = visibility_area + variable;
				if (std::find(variables.begin(), variables.end(), result_variable) != variables.end())
				{
					continue;
				}
				else
				{
					variables.push_back(result_variable);
					*(asmbl.stream) << "\t" << result_variable;
					*(asmbl.stream) << " DWORD ?\n";
				}
			}
		}
		*(asmbl.stream) << "\tifmt DB \"%d\", 0\n";
		*(asmbl.stream) << "\tbuf DB BSIZE dup(?)\n";
		*(asmbl.stream) << "\tstdout DD ?\n";
		*(asmbl.stream) << "\tcWritten DD ?\n";
		*(asmbl.stream) << "\tstdin DWORD ?\n";
		*(asmbl.stream) << "\tcRead DD ?\n";
		variables.clear();
		visibility_area = "";
		variable = "";
		result_variable = "";

		//LEX_MAIN LEX_FUNCTION LEX_RETURN LEX_PRINT LEX_IF LEX_UNTIL LEX_LOOP LEX_ELSE LEX_END LEX_EQUAL LEX_BRACELET

		bool main = false, function = false;
		//	Вывод сегмента кода
		*(asmbl.stream) << ".CODE\n\n";
		for (int i = 0; i < tables.lexTable.size; i++)
		{
			switch (tables.lexTable.table[i].lexema)
			{
				case LEX_FUNCTION:
				{
					visibility_area = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id;
					*(asmbl.stream) << visibility_area << " PROC ";
					for (; tables.lexTable.table[i].lexema != LEX_RIGHTHESIS; i++)
					{
						if (tables.lexTable.table[i].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i].indexIdTable].idtype == IT::P)
						{
							variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
							result_variable = visibility_area + variable;
							*(asmbl.stream) << result_variable << " : ";
							if (tables.idTable.table[tables.lexTable.table[i].indexIdTable].iddatatype == IT::INT)
							{
								*(asmbl.stream) << "DWORD";
							}
							else
							{
								*(asmbl.stream) << "DWORD";
							}
						}
						else if (tables.lexTable.table[i].lexema == LEX_COMMA)
							*(asmbl.stream) << ", ";
					}
					*(asmbl.stream) << "\n";
					function = true;
					break;
				}
				case LEX_MAIN:
				{
					*(asmbl.stream) << "main PROC\n";
					*(asmbl.stream) << "\tinvoke GetStdHandle, -11\n";
					*(asmbl.stream) << "\tmov stdout, EAX\n";
					*(asmbl.stream) << "\tinvoke GetStdHandle, -10\n";
					*(asmbl.stream) << "\tmov stdin, EAX\n";
					visibility_area = "main";
					main = true;
					break;
				}
				case LEX_PRINT:
				{
					if (tables.lexTable.table[i + 1].lexema == LEX_ID && tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].iddatatype == IT::INT)
					{
						variable = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id;
						result_variable = visibility_area + variable;
						*(asmbl.stream) << "\tinvoke wsprintf, ADDR buf, ADDR ifmt, " << result_variable << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0" << "\n";
						break;
					}

					else if (tables.lexTable.table[i + 1].lexema == LEX_ID)
					{
						variable = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id;
						result_variable = visibility_area + variable;
						*(asmbl.stream) << "\tmov ESI, offset " << result_variable << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, lengthof " << result_variable << ", ADDR cWritten, 0" << "\n";
						break;
					}

					if (tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].iddatatype == IT::INT)
					{
						*(asmbl.stream) << "\tinvoke wsprintf, ADDR buf, ADDR ifmt, " << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0"  << "\n";
						break;
					}
					else
					{
						*(asmbl.stream) << "\tmov ESI, offset " << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, 16, ADDR cWritten, 0" << "\n";
						break;
					}
					break;
				}
				case LEX_EQUAL:
				{
					int res_pos = i - 1;
					for (;tables.lexTable.table[i].lexema != LEX_SEMICOLON; i++)
					{
						switch (tables.lexTable.table[i].lexema)
						{
							case LEX_NUMERICAL_LITERAL:
							{
								*(asmbl.stream) << "\tpush " << tables.idTable.table[tables.lexTable.table[i].indexIdTable].id << "\n";
								break;
							}
							case LEX_ID:
							{
								variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
								result_variable = visibility_area + variable;
								*(asmbl.stream) << "\tpush " << result_variable << "\n";
								break;
							}
							case LEX_SYMBOL_LITERAL:
							case LEX_STRING_LITERAL:
							{
								*(asmbl.stream) << "\tpush offset " << tables.idTable.table[tables.lexTable.table[i].indexIdTable].id << "\n";
								break;
							}
							case LEX_ACTION:
							{
								switch (tables.lexTable.table[i].sign[0])
								{
									case LEX_PLUS:
									{
										*(asmbl.stream) << "\tpop EAX\n\tpop EBX\n";
										*(asmbl.stream) << "\tadd EAX, EBX\n\tpush EAX\n";
										break;
									}
									case LEX_MINUS:
									{
										*(asmbl.stream) << "\tpop EBX\n\tpop EAX\n";
										*(asmbl.stream) << "\tsub EAX, EBX\n\tpush EAX\n";
										break;
									}
									case LEX_STAR:
									{
										*(asmbl.stream) << "\tpop EAX\n\tpop EBX\n";
										*(asmbl.stream) << "\tmul EBX\n\tpush EAX\n";
										break;
									}
									case LEX_DIRSLASH:
									{
										*(asmbl.stream) << "\tmov EDX, 0\n";
										*(asmbl.stream) << "\tpop EBX\n\tpop EAX\n";
										*(asmbl.stream) << "\tdiv EBX\n\tpush EAX\n";
										break;
									}
								}
								break;
							}
							// switch
						}
						// for
					}
					variable = tables.idTable.table[tables.lexTable.table[res_pos].indexIdTable].id;
					result_variable = visibility_area + variable;
					*(asmbl.stream) << "\tpop " << result_variable << "\n";
					break;
				}
				case LEX_RETURN:
				{
					if (!main)
					{
						*(asmbl.stream) << "\tpush ";
						if (tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].idtype == IT::L && tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].iddatatype == IT::INT)
						{
							*(asmbl.stream) << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].value.vint << "\n";
						}
						else if (tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].idtype == IT::L)
						{
							*(asmbl.stream) << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].value.vstr->str << "\n";
						}
						else
						{
							*(asmbl.stream) << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id << "\n";
						}
						*(asmbl.stream) << "\tret\n";
					}
					break;
				}
				case LEX_BRACELET:
				{
					if (function)
					{
						*(asmbl.stream) << "\n" << visibility_area << " ENDP\n\n";
						function = false;
					}
					else if (main)
					{
						*(asmbl.stream) << "\tINVOKE ReadConsole, stdin, ADDR buf, 20, ADDR cRead, 0\n";
						*(asmbl.stream) << "\tINVOKE ExitProcess, -1\nmain ENDP\nend main";
						main = false;
					}
					break;
				}
			}
		}
	}
};