
#include "Headers.h"



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
		Log::WriteOut(out, in);
		LR::Tables tables = LR::FillingTables(in);
		Log::WriteLexTable(tables, log);
		Log::WriteIdTable(tables, log);
		PN::PolishNotation(18, tables.lexTable, tables.idTable);
		/*PN::PolishNotation(26, tables.lexTable, tables.idTable);
		PN::PolishNotation(36, tables.lexTable, tables.idTable);
		PN::PolishNotation(40, tables.lexTable, tables.idTable);
		PN::PolishNotation(46, tables.lexTable, tables.idTable);
		PN::PolishNotation(69, tables.lexTable, tables.idTable);*/
		Log::Close(log);
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

