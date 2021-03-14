
#include <iostream>

#include "HeapSort.h"
#include "../../Common/Common/Generator.h"

int main()
{
	// test for asc heap
	std::cout << "** Heapsort test in ascending order started **" << std::endl;
	for (std::size_t i = 0; i < 1000; ++i)
	{
		Heapsort<int> heapsort(MakeRandomNum(10, 100), 1, 100, MakeRandomNum);
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
		Heapsort<int, true> heapsort(MakeRandomNum(10, 100), 1, 100, MakeRandomNum);
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

	const std::size_t SIZE = 20;
	int array[SIZE] = { 0, };
	for (std::size_t i = 0; i < SIZE; ++i)
	{
		array[i] = MakeRandomNum(1, 100);
	}
	Heapsort<int> HeapFromArray(array, SIZE);
	HeapFromArray.Build();
	HeapFromArray.Sort();
	if (false == HeapFromArray.Validate())
	{
		std::cout << "** Heapsort(HeapFromArray) validation failed **" << std::endl;
		HeapFromArray.Show(3);
		return -1;
	}
	else
	{
		HeapFromArray.Show(3);
	}

	std::array<int, SIZE> stdArray;
	for (std::size_t i = 0; i < SIZE; ++i)
	{
		stdArray[i] = MakeRandomNum(1, 100);
	}
	Heapsort<int> HeapFromStdArray(stdArray);
	HeapFromStdArray.Build();
	HeapFromStdArray.Sort();
	if (false == HeapFromStdArray.Validate())
	{
		std::cout << "** Heapsort(HeapFromStdArray) validation failed **" << std::endl;
		HeapFromStdArray.Show(3);
		return -1;
	}
	else
	{
		HeapFromStdArray.Show(3);
	}

	std::cout << "** All test for Heapsort succeeded **" << std::endl;

	return 0;
}