
#include "Error.h"
#include "Log.h"
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <ctime>


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

		for (int i = 0; i < in.lmyStr; i++)
		{
			*log.stream << "Строка " << in.myStr[i] << "\n";
		}

		/*for (int i = 0; i < in.lmyStr; i++)
		{
			FST::FST myStr(
				in.myStr[i],
				7,
				FST::NODE(1, FST::RELATION('a', 1)),
				FST::NODE(5, FST::RELATION('b', 1), FST::RELATION('g', 5), FST::RELATION('c', 2), FST::RELATION('d', 4), FST::RELATION('e', 5)),
				FST::NODE(2, FST::RELATION('c', 2), FST::RELATION('b', 3)),
				FST::NODE(2, FST::RELATION('f', 6), FST::RELATION('b', 3)),
				FST::NODE(2, FST::RELATION('b', 3), FST::RELATION('d', 4)),
				FST::NODE(2, FST::RELATION('e', 5), FST::RELATION('b', 3)),
				FST::NODE()
			);

			if (FST::execute(myStr))
				*log.stream << "Цепочка " << myStr.string << " распознана\n";
			else
				*log.stream << "Цепочка " << myStr.string << " не распознана\n";
		}*/
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
}
