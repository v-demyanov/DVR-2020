#include "MFST.h"

int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];

#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
#define MFST_TRACE1(syn)	*(syn.stream)<<std::setw(4)<<std::left<<++FST_TRACE_n << ": "\
							<<std::setw(20)<<std::left<<rule.getCRule(rbuf, nrulechain)\
							<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)\
							<<std::setw(20)<<std::left<<getCStack(sbuf)<<std::endl;

#define MFST_TRACE2(syn)	*(syn.stream)<<std::setw(4)<<std::left<< FST_TRACE_n << ": "\
							<<std::setw(20)<<std::left<<" "\
							<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)\
							<<std::setw(20)<<std::left<<getCStack(sbuf)<<std::endl;

#define MFST_TRACE3(syn)	*(syn.stream)<<std::setw(4)<<std::left<< ++FST_TRACE_n << ": "\
							<<std::setw(20)<<std::left<<" "\
							<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)\
							<<std::setw(20)<<std::left<<getCStack(sbuf)<<std::endl;

#define MFST_TRACE4(c, syn)   *(syn.stream)<<std::setw(4)<<std::left<< ++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(c, syn)   *(syn.stream)<<std::setw(4)<<std::left<< FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE6(c,k, syn) *(syn.stream)<<std::setw(4)<<std::left<< FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<std::endl;
#define MFST_TRACE7(syn)		 *(syn.stream)<<std::setw(4)<<std::left<< state.lenta_position<<": " \
							  <<std::setw(20)<<std::left<<rule.getCRule(rbuf,state.nrulechain)<<std::endl;

namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) //(позиция на ленте, стек автомата, номер текущей цепочки и текущего правила)
	{
		lenta_position = pposition;
		stack = pst;
		nrulechain = pnrulechain;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)  //(позиция на ленте, стек автомата,номер текущего правила, номер текущей цепочки)
	{
		lenta_position = pposition;		// позиция на ленте
		stack = pst;					// стек автомата
		nrule = pnrule;
		nrulechain = pnrulechain;		// номер текущего правила и цепочки
	};

	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)		// Диагностика
	{								//(позиция на ленте, код завершения шага, номер правила, номер цепочки правила)
		lenta_position = plenta_position;		// позиция на ленте
		rc_step = prc_step;					    // код завершения шага
		nrule = pnrule;						// номер правила
		nrule_chain = pnrule_chain;			// номер цепочки правила
	};

	Mfst::Mfst()
	{
		lenta = 0;
		lenta_size = lenta_position = 0;
	};

	Mfst::Mfst(LR::Tables plex, GRB::Greibach pgreibach)
	{
		greibach = pgreibach;			// результат работы лексического анализатора, грамматика Грейбах
		lex = plex;
		lenta = new short[lenta_size = lex.lexTable.size];

		for (int k = 0; k < lenta_size; k++)				// заносит в ленту терминалы
		{
			lenta[k] = TS(lex.lexTable.table[k].lexema);
		}

		lenta_position = 0;
		stack.push(greibach.stbottomT);		// добавляет дно стека
		stack.push(greibach.startN);		// добавляет стартовый символ
		nrulechain = -1;					// изначально правило равно -1
	};

	Mfst::RC_STEP Mfst::step(Log::LOG syn) //выполнить шаг автомата
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(stack.top()))		// если в вершине стека содерж нетерм
			{
				GRB::Rule rule;
				if ((nrule = greibach.getRule(stack.top(), rule)) >= 0)	 // ищем правило по нетерминалу
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) //получаем следующую цепочку и выводим её номер, илбо возвращаем -1
					{
						MFST_TRACE1(syn)				// вывод
							saveState(syn);			// сохранить состояние автомата
						stack.pop();		    // извлекаем верхушку стека.
						push_chain(chain);		// поместить цепочку правила в стек
						rc = NS_OK;				// найдено правило и цепочка... цепочка записана в стек
						MFST_TRACE2(syn)				// вывод
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", syn)			// вывод
							saveDiagnosis(NS_NORULECHAIN);						// код завершения
						rc = restState(syn) ? NS_NORULECHAIN : NS_NORULE;		// восстановить состояние автомата
					};
				}
				else rc = NS_ERROR;		// если не подобрали никакое правило для текущ нетерминала
			}
			else if ((stack.top() == lenta[lenta_position]))		// если текущий символ ленты равен вершине стека
			{
				lenta_position++;		// продвигаем ленту
				stack.pop();			// вершина стека
				nrulechain = -1;		// номер текущего правила равен -1
				rc = TS_OK;
				MFST_TRACE3(syn)
			}
			else		// пытаемся восстановить предыдущее сост и подобрать для него другую цепочку
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN", syn)
					rc = restState(syn) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4("LENTA_END", syn)
		}
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain)		//поместить цепочку правила в стек в обратном порядке!!! (цепочка правила)
	{

		for (int k = chain.size - 1; k >= 0; k--)//к = длинне цепочке-1. заносим цепочку в стек
		{
			stack.push(chain.nt[k]);
		}
		return true;
	};

	bool Mfst::saveState(Log::LOG syn)		//Сохранить состояние автомата
	{
		storestate.push(MfstState(lenta_position, stack, nrule, nrulechain));		// стек для сохранения состояния. заносим сохраняемое состояние
		MFST_TRACE6("SAVESTATE: ", storestate.size(), syn);
		return true;
	};

	bool Mfst::restState(Log::LOG syn)		//Восстановить состояние автомата
	{
		bool rc = false;
		MfstState state;		//стек состояний
		if (rc = (storestate.size() > 0))		// если размер стека сост больше 0
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			stack = state.stack;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESTATE", syn)
				MFST_TRACE2(syn)
		};
		return rc;
	};

	bool Mfst::saveDiagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
			{
				diagnosis[j].lenta_position = -1;
			}
		};
		return rc;
	};

	bool Mfst::start(Log::LOG syn)		//Начало работы автомата
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(syn);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = step(syn);
		}

		switch (rc_step)
		{
		case LENTA_END:
			MFST_TRACE4("---->LENTA_END", syn)
				*(syn.stream) << "-------------------------------------------------------------------\n";
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			*(syn.stream) << std::setw(4) << std::left << 0 << ": всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок\n";
			rc = true;
			break;
		case NS_NORULE:
			MFST_TRACE4("---->NS_NORULE", syn)
				*(syn.stream) << "-------------------------------------------------------------------\n";
			*(syn.stream) << getDiagnosis(0, buf) << std::endl;
			*(syn.stream) << getDiagnosis(1, buf) << std::endl;
			*(syn.stream) << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN:
			MFST_TRACE4("---------->NS_NORULECHAIN", syn)
				break;
		case NS_ERROR:
			MFST_TRACE4("---------->NS_ERROR", syn)
				break;
		case SURPRISE:
			MFST_TRACE4("---------->SURPRISE", syn)
				break;
		};
		return rc;
	};

	char* Mfst::getCStack(char* buf)		//Получаем содержимое стека
	{
		for (int k = (signed)stack.size() - 1; k >= 0; --k)
		{
			short p = getStackElement(stack, k);
			//short p = stack._Get_container()[k];
			buf[stack.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);		// в символьный массив записываем содержимое стека
		};
		buf[stack.size()] = 0x00;
		return buf;
	};

	char* Mfst::getCLenta(char* buf, short pos, short n)			//Получаем содержимое входной ленты
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);		// в символьный массив записываем содержим ленты
		}
		buf[i - pos] = 0x00;
		return buf;
	};
	//***
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char* rc = (char*)"";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = greibach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lex.lexTable.table[lpos].lineSource, err.message); // *
			rc = buf;
		};
		return rc;
	};

	void Mfst::printRules(Log::LOG syn)		//Выводим дерево разбора
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = getStackElement(storestate, k);
			//state = storestate._Get_container()[k];
			rule = greibach.getRule(state.nrule);
			MFST_TRACE7(syn)
		};
	};

	bool Mfst::saveDeducation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = getStackElement(storestate, k);
			//state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
}