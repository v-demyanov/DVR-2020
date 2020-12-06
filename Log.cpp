
#include "Error.h"
#include "Log.h"
#include "LexicalRecognizer.h"
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>


namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new std::ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open())
			ERROR_THROW(112);
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	OUT getout(wchar_t outfile[]) //*
	{
		OUT out;
		out.stream = new std::ofstream;
		out.stream->open(outfile);
		if (!out.stream->is_open())
			ERROR_THROW(112);
		wcscpy_s(out.outfile, outfile);
		return out;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		std::string str;
		va_list pargs;
		va_start(pargs, c);
		for (const char* i = c; i != ""; i = va_arg(pargs, const char*))
		{
			str += i;
		}
		va_end(pargs);
		*log.stream << str << "\n";
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		std::string str;
		char tmp[100];
		va_list pargs;
		va_start(pargs, c);
		for (const wchar_t* i = c; i != L""; i = va_arg(pargs, const wchar_t*))
		{
			wcstombs(tmp, c, sizeof(tmp));
			str += tmp;
		}
		va_end(pargs);
		*log.stream << str << "\n";
	}
	void WriteLog(LOG log)
	{
		*log.stream << " ---- Протокол ---- Дата: ";
		time_t current = time(NULL); // в секундах
		errno_t err;
		tm date;
		err = localtime_s(&date, &current);
		if (err)
		{
			*log.stream << "Ошибка даты\n";
		}
		else
		{
			char str[100];
			strftime(str, sizeof(str), "%c", &date);
			*log.stream << str << " ----\n";
		}
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char str[PARM_MAX_SIZE];
		*log.stream << "---- Параметры ----\n";
		wcstombs(str, parm.log, sizeof(str));
		*log.stream << "-log: " << str << "\n";
		wcstombs(str, parm.out, sizeof(str));
		*log.stream << "-out: " << str << "\n";
		wcstombs(str, parm.in, sizeof(str));
		*log.stream << "-in: " << str << "\n";
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- Исходные данные ----\n";
		*log.stream << "Количество символов: " << in.size << "\n";
		*log.stream << "Проигнорировано: " << in.ignor << "\n";
		*log.stream << "Количество строк: " << in.lines << "\n";
	}

	void WriteLexTable(LR::Tables tables , LOG log)
	{
		LT::LexTable lexTable = tables.lexTable;
		*log.stream << "\n" << "LEX_TABLE: " << "\n";
		int line = -1;
		char* strLine = new char[5];
		for (int i = 0; i < lexTable.size; i++)
		{
			if (line < lexTable.table[i].lineSource)
			{
				line = lexTable.table[i].lineSource;
				_itoa(line + 1, strLine, 10);
				*log.stream << "\n";
				if (line < 10)
					*log.stream << '\t' << std::setw(2) << std::setfill('0') << strLine << ' ';
				else
					*log.stream << '\t' << strLine << ' ';
			}
			*log.stream << lexTable.table[i].lexema;
		}
		*log.stream << '\n';
		delete[] strLine;
	}

	void fillStr(LOG log, std::string id, std::string dataType, std::string idType, int vint, int idxfirstLE)
	{
		int l1 = 45 - id.length();
		int l2 = 35 - dataType.length();
		int l3 = 45 - idType.length();
		int l4 = 45 - idType.length();
		*log.stream << '\t' << std::setw(l1) << std::setfill(' ') << std::left << id
			<< std::setw(l2) << std::setfill(' ') << dataType
			<< std::setw(l3) << std::setfill(' ') << idType
			<< std::setw(l4) << std::setfill(' ') << vint
			<< std::setw(35) << std::setfill(' ') << idxfirstLE
			<< std::endl;
	}

	void fillStr(LOG log, std::string id, std::string dataType, std::string idType, std::string str, int idxfirstLE)
	{
		int l1 = 45 - id.length();
		int l2 = 35 - dataType.length();
		int l3 = 45 - idType.length();
		int l4 = 45 - idType.length();
		*log.stream << '\t' << std::setw(l1) << std::setfill(' ') << std::left << id
			<< std::setw(l2) << std::setfill(' ') << dataType
			<< std::setw(l3) << std::setfill(' ') << idType
			<< std::setw(l4) << std::setfill(' ') << str
			<< std::setw(35) << std::setfill(' ') << idxfirstLE
			<< std::endl;
	}

	void WriteIdTable(LR::Tables tables, LOG log)
	{
		IT::IdTable idTable = tables.idTable;
		IT::IDDATATYPE iddatatype;
		IT::IDTYPE idtype;
		*log.stream << "\n" << "ID_TABLE: " << "\n";
		*log.stream << '\n' << '\t' << std::setw(30) << std::setfill(' ') << std::left << "ID_NAME"
			<< std::setw(30) << std::setfill(' ') << "DATA_TYPE"
			<< std::setw(30) << std::setfill(' ') << "ID_TYPE"
			<< std::setw(30) << std::setfill(' ') << "VALUE"
			<< std::setw(30) << std::setfill(' ') << "ID_FIRST_LT"
			<< std::endl << std::endl;
		for (int i = 0; i < idTable.size; i++)
		{
			iddatatype = idTable.table[i].iddatatype;
			idtype = idTable.table[i].idtype;
			switch (idtype)
			{
				case IT::V:
				{
					if (iddatatype == IT::INT)
					{
						fillStr(log, idTable.table[i].id, "INT", "variable", idTable.table[i].value.vint, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::STR)
					{
						fillStr(log, idTable.table[i].id, "STR", "variable", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::SYBM)
					{
						fillStr(log, idTable.table[i].id, "SYMB", "variable", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					break;
				}
				case IT::F:
				{
					if (iddatatype == IT::INT)
					{
						fillStr(log, idTable.table[i].id, "INT", "Function", idTable.table[i].value.vint, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::STR)
					{
						fillStr(log, idTable.table[i].id, "STR", "Function", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::SYBM)
					{
						fillStr(log, idTable.table[i].id, "SYMB", "Function", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					break;
				}
				case IT::P:
				{
					if (iddatatype == IT::INT)
					{
						fillStr(log, idTable.table[i].id, "INT", "PARAMETER", idTable.table[i].value.vint, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::STR)
					{
						fillStr(log, idTable.table[i].id, "STR", "PARAMETER", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::SYBM)
					{
						fillStr(log, idTable.table[i].id, "SYMB", "PARAMETER", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					break;
				}
				case IT::L:
				{
					if (iddatatype == IT::INT)
					{
						fillStr(log, idTable.table[i].id, "INT", "Literal", idTable.table[i].value.vint, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::STR)
					{
						fillStr(log, idTable.table[i].id, "STR", "Literal", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					else if (iddatatype == IT::SYBM)
					{
						fillStr(log, idTable.table[i].id, "SYMB", "Literal", idTable.table[i].value.vstr->str, idTable.table[i].idxfirstLE);
					}
					break;
				}
			}
			
		}

	}

	void WriteOut(OUT out, In::IN in) //*
	{
		for (int i = 0; i < in.lmyStr; i++)
		{
			*out.stream << in.myStr[i] << "\n";
		}
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream != NULL)
		{
			*log.stream << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", колонка " << error.inext.col;
		}
		else
		{
			std::cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line;
		}
	}

	void Close(LOG log)
	{
		log.stream->close();
	}

	void CloseOut(OUT out) //*
	{
		out.stream->close();
	}
}
