#pragma once

#include <iostream>
#include <iomanip>

#include "../../Common/Heap/Heap.h"

/*
	HeapSort sorts elements
*/
template <typename Type, bool Desc = false>
class HeapSort : public Heap<Type, !Desc>
{
private:
	typedef Heap<Type, !Desc> Super;

public:
	typedef Type (*FnRandomGenerator)(Type, Type);

public:
	// make a heap with random n-elements
	HeapSort(std::size_t n, Type min, Type max, FnRandomGenerator generator)
	{
		for (std::size_t i = 0; i < n; ++i)
		{
			Super::_elements.push_back(generator(min, max));
		}
	}

	HeapSort() = default;

	void Sort()
	{
		if (1 >= Super::_elements.size())
		{
			return;
		}

		std::size_t n = Super::_elements.size() - 1;
		while (0 < n)
		{
			Super::exchange(Super::_elements[0], Super::_elements[n]);
			n--;
			Super::Heapify(0, n);
		}
	}

	bool Validate()
	{
		const std::size_t n = Super::_elements.size();
		for (std::size_t i = 1; i < n; ++i)
		{
			if (!Super::compare<Desc>(Super::_elements[i - 1], Super::_elements[i]))
			{
				return false;
			}
		}

		return true;
	}

	void Show(int width)
	{
		std::cout << "** Heapsort elements start **" << std::endl;

		const std::size_t n = Super::_elements.size();
		for (std::size_t i = 0; i < n; ++i)
		{
			std::cout << "[" << std::setw(width) << i << "] : " << Super::_elements[i] << std::endl;
		}

		std::cout << "** Heapsort elements end **" << std::endl << std::endl;
	}
};