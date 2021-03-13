#include <iomanip>
#include <ctime>

#include "Generator.h"

int MakeRandomNum(int min, int max)
{
	static bool init = false;
	if (false == init)
	{
		srand((unsigned int)time(NULL));
		init = true;
	}

	return (int)rand() % (max - min + 1) + min;
}
