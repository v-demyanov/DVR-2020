#pragma once
#include <stack>
#include <iomanip>
#include "GRB.h"
#include "Log.h"
#include <iostream>


#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
#define MFST_TRACE_START(syn) *(syn.stream)<<std::setw( 4)<<std::left<<"Шаг"<<":"\
									<<std::setw(20)<<std::left<<"Правило"\
									<<std::setw(30)<<std::left<<"Входная лента"\
									<<std::setw(20)<<std::left<<"Стек"<<std::endl;

typedef std::stack<short> MFSTSTSTACK;	// стек автомата

namespace MFST				// магазинный автомат
{
	template<class T>T getStackElement(const std::stack<T>& st, size_t element)
	{
		using base = std::stack<T>;
		struct acces : base { using base::c; };
		return (st.* & acces::c)[element];
	}

	struct MfstState		// состояние автомата (для сохранения)
	{
		short lenta_position;		// позиция на ленте
		short nrule;				// номер текущего правила
		short nrulechain;			// номер текущей цепочки
		MFSTSTSTACK stack;			// стек автомата
		MfstState();
		MfstState(
			short pposition,       // позиция на ленте
			MFSTSTSTACK pst,       // стек автомата
			short pnrulechain      // номер текущей цепочки
		);
		MfstState(
			short pposition,       // позиция на ленте
			MFSTSTSTACK pst,       // стек автомата
			short pnrule,		   // номер текущего правила		
			short pnrulechain      // номер текущей цепочки
		);
	};

	struct Mfst			// магазинный автомат
	{
		enum RC_STEP {				// код возврата функции step
			NS_OK,					// найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,				// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,			// не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,				// неизвестный нетерминальный символ грамматики
			TS_OK,					// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,					// тек. символ ленты != вершине стека, восстановлено состояние 
			LENTA_END,				// текущая позиция ленты >= lenta_size
			SURPRISE				// неожиданный код возврата (ошибка в step)
		};

		struct MfstDiagnosis				// диагностика
		{
			short lenta_position;			// позиция на ленте
			RC_STEP rc_step;				// код завершения шага
			short nrule;					// номер правила
			short nrule_chain;				// номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(				// дигностики
				short plenta_position,	// позиция на ленте
				RC_STEP prc_step,		// код завершения шага
				short pnrule,			// номер правила 
				short pnrule_chain		// номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER];		// последние самые глубокие сообщения

		GRBALPHABET* lenta;					// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;				// текущая позиция на ленте
		short nrule;						// номер текущего правила
		short nrulechain;					// номер текущей цепочки текущего правила
		short lenta_size;					// размер ленты
		GRB::Greibach greibach;				// грамматика Грейбах
		LR::Tables lex;						// результат работы лексического анализатора
		MFSTSTSTACK stack;					// стек автомата
		std::stack<MfstState> storestate;	// стек для сохранения состояний
		Mfst();
		Mfst(
			LR::Tables plex,				// результат работы лексического анализатора
			GRB::Greibach pgrebach		// грамматика Грейбах
		);

		char* getCStack(char* buf);								// получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);	// лента: n символов с pos
		char* getDiagnosis(short n, char* buf);					// получить n-ую строку диагностики или 0х00
		bool saveState(Log::LOG syn);										// сохранить состояние автомата
		bool restState(Log::LOG syn);										// восстановить состояние автомата
		bool push_chain(										// поместить цепочку правила в стек
			GRB::Rule::Chain chain								// цепочка правил
		);
		RC_STEP step(Log::LOG syn);							// выполнить шаг автомата
		bool start(Log::LOG syn);							// запустить автомат
		bool saveDiagnosis(
			RC_STEP pprc_step					// код завершения шага
		);
		void printRules(Log::LOG syn);						// вывести последовательность правил

		struct Deducation	// вывод
		{
			short size;					// кличество шагов в выводе
			short* nrules;				// номера правил грамматики
			short* nrulechains;			// номера цепочек правил граммитики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;
		bool saveDeducation();			// сохранить дерево вывода
	};
}