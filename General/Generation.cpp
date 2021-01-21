
#include "Generation.h"
#include "IT.h"
#include <iostream>
#include <map>
#include <time.h>


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
		std::stack<std::string> stack;
		std::string mark = "";
		int markCount = 0;
		int lexLen = 0;

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
		*(asmbl.stream) << "\tCRLF WORD ?\n";
		variables.clear();
		visibility_area = "";
		variable = "";
		result_variable = "";
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
				case LEX_UNTIL:
				{
					mark = "CYCLE" + markCount;
					*(asmbl.stream) << "\t" + mark + ":" << "\n";
					i = i + 2;
					//	Записываем в EAX итератор
					variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
					result_variable = visibility_area + variable;
					*(asmbl.stream) << "\tmov EAX, " << result_variable << "\n";
					//	Сравниваем итератор с другим значением
					variable = tables.idTable.table[tables.lexTable.table[i + 2].indexIdTable].id;
					result_variable = visibility_area + variable;
					*(asmbl.stream) << "\tcmp EAX, " << result_variable << "\n";

					std::string sign = tables.lexTable.table[i + 1].sign;
					std::map <std::string, int> mapping;

					mapping["<="] = 1;
					mapping[">="] = 2;
					mapping["<"] = 3;
					mapping[">"] = 4;
					mapping["=="] = 5;

					switch (mapping[sign]) 
					{
						case 1:
						{
							*(asmbl.stream) << "\tjbe cycle_body\n";
							break;
						}
						case 2:
						{
							*(asmbl.stream) << "\tjae cycle_body\n";
							break;
						}
						case 3:
						{
							*(asmbl.stream) << "\tjb cycle_body\n";
							break;
						}
						case 4:
						{
							*(asmbl.stream) << "\tja cycle_body\n";
							break;
						}
						case 5:
						{
							*(asmbl.stream) << "\tje cycle_body\n";
							break;
						}
					}
					*(asmbl.stream) << "\tcycle_end:\n\t\tjmp " << mark + "end" << "\n";
					*(asmbl.stream) << "\tcycle_body:\n";
					i = i + 3;
					break;
				}
				case LEX_LOOP:
				{
					*(asmbl.stream) << "\tjmp " + mark << "\n";
					*(asmbl.stream) << "\t" + mark + "end:" << "\n";
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
						*(asmbl.stream) << "\tmov ESI, offset buf" << "\n";
						*(asmbl.stream) << "\tmov EDI, -1\n";
						*(asmbl.stream) << "\tcount:\n";
						*(asmbl.stream) << "\tinc EDI\n";
						*(asmbl.stream) << "\tcmp BYTE ptr" << " [ESI+EDI], 0\n";
						*(asmbl.stream) << "\tjne count\n";

						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR buf, EDI, ADDR cWritten, 0" << "\n";
						*(asmbl.stream) << "\tmov CRLF, 0d0ah\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR CRLF, 2, ADDR cWritten, 0\n";
						break;
					}
					else if (tables.lexTable.table[i + 1].lexema == LEX_ID)
					{
						variable = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id;
						result_variable = visibility_area + variable;
						//lexLen = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].value.vstr->len;
						*(asmbl.stream) << "\tmov ESI, " << result_variable << "\n";
						*(asmbl.stream) << "\tmov EDI, -1\n";
						*(asmbl.stream) << "\tcount:\n";
						*(asmbl.stream) << "\tinc EDI\n";
						*(asmbl.stream) << "\tcmp BYTE ptr" << " [ESI+EDI], 0\n";
						*(asmbl.stream) << "\tjne count\n";

						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, EDI " << ", ADDR cWritten, 0" << "\n";
						*(asmbl.stream) << "\tmov CRLF, 0d0ah\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR CRLF, 2, ADDR cWritten, 0\n";
						//*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, " << lexLen << ", ADDR cWritten, 0" << "\n";
						break;
					}

					if (tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].iddatatype == IT::INT)
					{
						*(asmbl.stream) << "\tinvoke wsprintf, ADDR buf, ADDR ifmt, " << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0"  << "\n";
						*(asmbl.stream) << "\tmov CRLF, 0d0ah\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR CRLF, 2, ADDR cWritten, 0\n";
						break;
					}
					else
					{
						//lexLen = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].value.vstr->len;
						*(asmbl.stream) << "\tmov ESI, offset " << tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id << "\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, 7, ADDR cWritten, 0" << "\n";
						*(asmbl.stream) << "\tmov CRLF, 0d0ah\n";
						*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ADDR CRLF, 2, ADDR cWritten, 0\n";
						//*(asmbl.stream) << "\tinvoke WriteConsoleA, stdout, ESI, " << lexLen << ", ADDR cWritten, 0" << "\n";
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
								stack.push(tables.idTable.table[tables.lexTable.table[i].indexIdTable].id);
								*(asmbl.stream) << "\tpush " << tables.idTable.table[tables.lexTable.table[i].indexIdTable].id << "\n";
								break;
							}
							case LEX_ID:
							{
								variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
								result_variable = visibility_area + variable;
								stack.push(result_variable);
								*(asmbl.stream) << "\tpush " << result_variable << "\n";
								break;
							}
							case LEX_SYMBOL_LITERAL:
							case LEX_STRING_LITERAL:
							{
								variable = tables.idTable.table[tables.lexTable.table[i].indexIdTable].id;
								stack.push("offset " + variable);
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
							case LEX_FUNC_CALL:
							{
								for (int k = 0; k < tables.lexTable.table[i].numberOfParams; k++)
									*(asmbl.stream) << "\tpop edx\n";
								for (int k = 0; k < tables.lexTable.table[i].numberOfParams; k++)
								{
									*(asmbl.stream) << "\tpush " << stack.top() << "\n";
									stack.pop();
								}
								*(asmbl.stream) << "\tcall " << tables.idTable.table[tables.lexTable.table[i].indexIdTable].id << "\n\tpush EAX\n";
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
							variable = tables.idTable.table[tables.lexTable.table[i + 1].indexIdTable].id;
							result_variable = visibility_area + variable;
							*(asmbl.stream) << result_variable << "\n";
						}
						*(asmbl.stream) << "\tpop EAX\n";
						*(asmbl.stream) << "\tret";
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
						*(asmbl.stream) << "\tINVOKE ReadConsole, stdin, ADDR buf, 1, ADDR cRead, 0\n";
						*(asmbl.stream) << "\tINVOKE ExitProcess, -1\nmain ENDP\nend main";
						main = false;
					}
					break;
				}
			}
		}
	}
	
};