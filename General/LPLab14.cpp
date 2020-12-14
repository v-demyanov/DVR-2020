
#include "Headers.h"

#define LOG_TEST 1

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");

#if LOG_TEST
	Log::LOG log = Log::INITLOG;
	Log::LOG out = Log::INITLOG;
	Log::LOG asmbl = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		out = Log::getlog(parm.out);
		asmbl = Log::getlog(parm.asmbl);
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

			// перебор в цикле
		/*for (int n : tables.lexTable.refsToAssigns)
			std::cout << n << "\t";*/
		
		tables.lexTable = PN::PolishNotation(tables.lexTable, tables.idTable);
		GEN::Generation(tables, asmbl);
		Log::WriteLexTable(tables, log);

		/*MFST_TRACE_START
		MFST::Mfst mfst(tables, GRB::getGreibach());
		mfst.start();

		mfst.saveDeducation();
		mfst.printRules();*/

		Log::Close(log);
		Log::Close(out);
		Log::Close(asmbl);
		//Log::CloseOut(out);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};
#endif //LOG_TEST

	system("pause");
	return 0;
};

