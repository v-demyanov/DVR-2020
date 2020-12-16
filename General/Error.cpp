#include "Error.h"
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		//	ñèñòåìíûå îøèáêè
		ERROR_ENTRY(0, "Íåäîïóñòèìûé êîä îøèáêè"),
		ERROR_ENTRY(1, "Ñèñòåìíûé ñáîé"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		//	îøèáêè ïàğàìåòğîâ
		ERROR_ENTRY(100, "Ïàğàìåòğ -in äîëæåí áûòü çàäàí"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Ïğåâûøåíà äëèíà âõîäíîãî ïàğàìåòğà"),
		ERROR_ENTRY(105, "Ïàğàìåòğ -asm äîëæåí áûòü çàäàí"),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		//	îøèáêè ñ ôàéëàìè
		ERROR_ENTRY(110, "Îøèáêà ïğè îòêğûòèè ôàéëà ñ èñõîäíûì êîäîì (-in)"),
		ERROR_ENTRY(111, "Íåäîïóñòèìûé ñèìâîë â èñõîäíîì ôàéëå (-in)"),
		ERROR_ENTRY(112, "Îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		//	îøèáêè ëåêñè÷åñêîãî àíàëèçà
		ERROR_ENTRY(120, "Öåïî÷êà ñèìâîëîâ íå ğàñïîçíàíà"),
		ERROR_ENTRY(121, "Äëèíà èäåíòèôèêàòîğà ïğåâûøàåò äîïóñòèìóş äëèíó"),
		ERROR_ENTRY(122, "Ïğåâûøåíî ìàêñ êîë-âî ñòğîê â òàáëèöå èäåíòèôèêàòîğîâ"),
		ERROR_ENTRY(123, "Ïğåâûøåíî ìàêñ êîë-âî ñòğîê â òàáëèöå ëåêñåì"),
		ERROR_ENTRY_NODEF(124), ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		
		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		//	îøèáêè ñåìàíòè÷åñêîãî àíàëèçà
		ERROR_ENTRY(200, "ïåğåîïğåäåëåíèå èäåíòèôèêàòîğà"),
		ERROR_ENTRY(201, "èäåíòèôèêàòîğ íå îïğåäåë¸í, íî èñïîëüçóåòñÿ"),
		ERROR_ENTRY(202, "âñå îïåğàíäû â âûğàæåíèÿõ è îïåğàöèÿõ äîëæíû èìåòü òèïû, äîïóñòèìûå äëÿ äàííîãî âûğàæåíèÿ èëè îïåğàöèè"),
		ERROR_ENTRY(203, "êîëè÷åñòâî èëè òèïû ôàêòè÷åñêèõ ïàğàìåòğîâ íå ñîãëàñîâàíû ñ êîëè÷åñòâîì èëè òèïàìè ôîğìàëüíûõ ïàğàìåòğîâ"),
		ERROR_ENTRY(204, "òèï âîçâğàùàåìîãî çíà÷åíèÿ ôóíêöèè"),
		ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230),
		ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		//	îøèáêè ñèíòàêñè÷åñêîãî àíàëèçà
		ERROR_ENTRY(600, "Íåâåğíàÿ ñòğóêòóğà ïğîãğàììû"),
		ERROR_ENTRY(601, "Îøèáî÷íûé îïåğàòîğ"),
		ERROR_ENTRY(602, "Îøèáêà â âûğàæåíèè"),
		ERROR_ENTRY(603, "Îøèáêà â àğèôìåòè÷åñêèõ îïåğàòîğàõ"),
		ERROR_ENTRY(604, "Îøèáêà â ïàğàìåòğàõ ôóíêöèè"),
		ERROR_ENTRY(605, "Îøèáêà â ïàğàìåòğàõ âûçûâàåìîé ôóíêöèè"),
		ERROR_ENTRY(606, "Îøèáêà â ïàğàìåòğàõ öèêëà èëè îïåğàòîğà âåòâëåíèÿ"),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609), ERROR_ENTRY_NODEF10(610),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			for (int i = 0; i < ERROR_MAX_ENTRY; i++)
			{
				if (id == errors[i].id)
					return errors[i];
			}
			return errors[0];
		}
		else
		{
			return errors[0];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			for (int i = 0; i < ERROR_MAX_ENTRY; i++)
			{
				if (id == errors[i].id)
				{
					errors[i].inext.line = line;
					errors[i].inext.col = col;
					return errors[i];
				}
			}
			return errors[0];
		}
		else
		{
			return errors[0];
		}
	}
};