
#include <iostream>

#include "HeapSort.h"
#include "../../Common/Common/Generator.h"

int main()
{
	// test for asc heap
	std::cout << "** Heapsort test in ascending order started **" << std::endl;
	for (std::size_t i = 0; i < 1000; ++i)
	{
		HeapSort<int> heapsort(MakeRandomNum(10, 100), 1, 100, MakeRandomNum);
		heapsort.Build();
		heapsort.Sort();
		if (false == heapsort.Validate())
		{
			std::cout << "** Heapsort validation failed **" << std::endl;
			heapsort.Show(3);
			return -1;
		}
		else
		{
			std::cout << "** test number [" << std::setw(3) << i << "] : success!" << std::endl;
		}
	}
	std::cout << "** Heapsort test in ascending order finished **" << std::endl << std::endl;

	// test for desc heap
	std::cout << "** Heapsort test in descending order started **" << std::endl;
	for (std::size_t i = 0; i < 1000; ++i)
	{
		HeapSort<int, true> heapsort(MakeRandomNum(10, 100), 1, 100, MakeRandomNum);
		heapsort.Build();
		heapsort.Sort();
		if (false == heapsort.Validate())
		{
			std::cout << "** Heapsort validation failed **" << std::endl;
			heapsort.Show(3);
			return -1;
		}
		else
		{
			std::cout << "** test number [" << std::setw(3) << i << "] : success!" << std::endl;
		}
	}

	std::cout << "** Heapsort test in descending order finished **" << std::endl << std::endl;

	std::cout << "** All test for Heapsort succeeded **" << std::endl;

	return 0;
}