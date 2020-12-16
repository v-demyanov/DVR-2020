#pragma once
#include <TCHAR.H>
#define PARM_IN L"-in:"
#define PARM_OUT L"-out:"
#define PARM_LOG L"-log:"
#define PARM_ASM L"-asm:"
#define PARM_SYN L"-syn:"
#define PARM_LEX L"-lex:"
#define PARM_PN L"-pn:"
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out"
#define PARM_LOG_DEFAULT_EXT L".log"
#define PARM_ASM_DEFAULT_EXT L".asm"
#define PARM_SYN_DEFAULT_EXT L".syn"
#define PARM_LEX_DEFAULT_EXT L".lex"
#define PARM_PN_DEFAULT_EXT L".pn"

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t lex[PARM_MAX_SIZE];
		wchar_t pn[PARM_MAX_SIZE];
		wchar_t syn[PARM_MAX_SIZE];
		wchar_t asmbl[PARM_MAX_SIZE];
	};

	PARM getparm(int argc, _TCHAR* argv[]);
};