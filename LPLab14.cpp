
#include <TCHAR.H>
#include <iostream>
#include <locale>
#include <cwchar>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"



#define LOG_TEST 1

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");

#if LOG_TEST
	Log::LOG log = Log::INITLOG;
	Log::OUT out = Log::INITOUT;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		out = Log::getout(parm.out);
		Log::WriteLine(log, "Тест:", "без ошибок ", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		std::wcout << "-in:" << parm.in << ", -out:" << parm.out << ", -log:" << parm.log << std::endl << std::endl;
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::Close(log);
		Log::WriteOut(out, in);
		Log::CloseOut(out);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};
#endif //LOG_TEST

	system("pause");
	return 0;
};

