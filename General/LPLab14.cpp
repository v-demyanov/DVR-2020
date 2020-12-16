
#include "Headers.h"
#include <ctime>

#define LOG_TEST 1

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	unsigned int start_time = clock();
#if LOG_TEST
	Log::LOG log = Log::INITLOG;
	Log::LOG out = Log::INITLOG;
	Log::LOG syn = Log::INITLOG;
	Log::LOG lex = Log::INITLOG;
	Log::LOG pn = Log::INITLOG;
	Log::LOG asmbl = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		//	Создание протоколов
		log = Log::getlog(parm.log);
		out = Log::getlog(parm.out);
		syn = Log::getlog(parm.syn);
		lex = Log::getlog(parm.lex);
		pn = Log::getlog(parm.pn);
		asmbl = Log::getlog(parm.asmbl);
		//	Заполнение журнала log
		Log::WriteLine(log, "Тест:", "без ошибок ", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		std::wcout << "-in:" << parm.in << ", -out:" << parm.out << ", -log:" << parm.log << std::endl << std::endl;
		//	Обработка исходного кода, удаление излишних символов
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::WriteOut(out, in);
		//	Лексический анализ
		LR::Tables tables = LR::FillingTables(in);
		Log::WriteLexTable(tables, lex);
		Log::WriteIdTable(tables, lex);
		//	Синтаксический анализ
		MFST_TRACE_START(syn)
			MFST::Mfst mfst(tables, GRB::getGreibach());
		mfst.start(syn);
		mfst.saveDeducation();
		mfst.printRules(syn);
		//	Семантический анализ
		SA::SemanticAnalyzer(tables);
		//	Перевод выражений в обратную польскую запись
		tables.lexTable = PN::PolishNotation(tables.lexTable, tables.idTable, pn);
		Log::WriteLexTable(tables, pn);
		//	Генерация в целевой код
		GEN::Generation(tables, asmbl);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		std::cout << "Время трансляции : " << search_time << std::endl;
		*(log.stream) << "\nВремя трансляции: " << search_time << std::endl;
		//	Закрытие потоков
		Log::Close(log);
		Log::Close(out);
		Log::Close(syn);
		Log::Close(lex);
		Log::Close(pn);
		Log::Close(asmbl);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};
#endif //LOG_TEST
	system("pause");
	return 0;
};

