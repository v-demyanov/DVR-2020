
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
		variables.clear();
		visibility_area = "";

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
							*(asmbl.stream) << tables.idTable.table[tables.lexTable.table[i].indexIdTable].id << " : ";
							if (tables.idTable.table[tables.lexTable.table[i].indexIdTable].iddatatype == IT::INT)
							{
								*(asmbl.stream) << "DWORD";
							}
							else
							{
								*(asmbl.stream) << "BYTE";
							}
						}
						else if (tables.lexTable.table[i].lexema == LEX_COMMA)
							*(asmbl.stream) << ", ";
					}
					function = true;
					break;
				}
				case LEX_MAIN:
				{
					*(asmbl.stream) << "main PROC\n";
					*(asmbl.stream) << "\tinvoke GetStdHandle, -11\n";
					*(asmbl.stream) << "\tmov stdout, EAX\n";
					main = true;
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
						*(asmbl.stream) << "\tINVOKE ExitProcess, -1\nmain ENDP\nend main";
						main = false;
					}
					break;
				}
			}
		}
	}
};