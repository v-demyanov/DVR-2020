
#include "Headers.h"

#define LOG_TEST 1


//z = (x * (x + y) + fi()) + fi(x, x, y, x, y) / x / (x + y);
//z = x + (z - y) * x / (x + y);
//z = x + (x + y) + x + (x + y);
//x* (x* (x + y))
//
//
//z = x * (x + y) + fi(x, y);
//x + (z - y) * x / (x + y);
//x + y / x + y;
//x + y * z;
//(x + y)* z;

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
		for (int i = 0; i < in.lmyStr; i++)
		{
			std::cout << in.myStr[i] << "\n";
		}
		Log::WriteOut(out, in);
		LR::Tables tables = LR::FillingTables(in);
		Log::WriteLexTable(tables, log);
		Log::WriteIdTable(tables, log);
		/*PN::PolishNotation(18, tables.lexTable, tables.idTable);
		MFST_TRACE_START
		MFST::Mfst mfst(tables, GRB::getGreibach());
		mfst.start();

		mfst.saveDeducation();
		mfst.printRules();*/

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

