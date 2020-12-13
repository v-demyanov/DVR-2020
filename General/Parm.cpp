#include "Parm.h"
#include "Error.h"
#include <TCHAR.H>
#include <wchar.h>
#include <iostream>

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM options;
		if (argc == 1)
			throw ERROR_THROW(100);
		for (int i = 1; i < argc; i++)
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);
		wchar_t logAuto[PARM_MAX_SIZE];
		wchar_t outAuto[PARM_MAX_SIZE];
		wcscpy_s(logAuto, argv[1] + wcslen(PARM_IN));
		wcscat_s(logAuto, PARM_LOG_DEFAULT_EXT);
		wcscpy_s(outAuto, argv[1] + wcslen(PARM_IN));
		wcscat_s(outAuto, PARM_OUT_DEFAULT_EXT);

		options.in[0] = '\0';
		options.out[0] = '\0';
		options.log[0] = '\0';
		options.asmbl[0] = '\0';

		for (int i = 1; i < argc; i++)
		{
			if (wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(options.in, argv[i] + wcslen(PARM_IN));
				options.in[wcslen(argv[i])] = '\0';
			}

			if (wcsstr(argv[i], PARM_ASM))
			{
				wcscpy_s(options.asmbl, argv[i] + wcslen(PARM_ASM));
				options.asmbl[wcslen(argv[i])] = '\0';
			}

			if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(options.out, argv[i] + wcslen(PARM_OUT));
				options.out[wcslen(argv[i])] = '\0';
			}

			if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(options.log, argv[i] + wcslen(PARM_LOG));
				options.log[wcslen(argv[i])] = '\0';
			}
		}

		if (options.in[0] == '\0')
			throw ERROR_THROW(100);
		if (options.asmbl[0] == '\0')
			throw ERROR_THROW(105);

		if (options.out[0] == '\0' && options.log[0] == '\0')
		{
			wcscpy_s(options.out, outAuto);
			wcscpy_s(options.log, logAuto);
		}
		else if (options.out[0] == '\0')
			wcscpy_s(options.out, outAuto);
		else if (options.log[0] == '\0')
			wcscpy_s(options.log, logAuto);

		return options;
	}
};