#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <stdarg.h>
#include "LexicalRecognizer.h"

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};

	static const LOG INITLOG = { L"", NULL };
	LOG getlog(wchar_t logfile[]);
	void WriteLine(LOG log, const char* c, ...);
	void WriteLine(LOG log, const wchar_t* c, ...);
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteOut(LOG out, In::IN in);
	void WriteError(LOG log, Error::ERROR error);
	void Close(LOG log);
	void WriteLexTable(LR::Tables tables, LOG log);
	void WriteIdTable(LR::Tables tables, LOG log);
};