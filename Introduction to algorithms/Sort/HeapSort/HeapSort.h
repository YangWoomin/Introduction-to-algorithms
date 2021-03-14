#pragma once

#include <iostream>
#include <iomanip>
#include <array>

#include "../../Common/Heap/Heap.h"

/*
	Heapsort sorts elements
*/
template <typename Type, bool Desc = false>
class Heapsort : public Heap<Type, !Desc>
{
private:
	typedef Heap<Type, !Desc> Super;

public:
	typedef Type (*FnRandomGenerator)(Type, Type);

public:
	// make a heap with random n-elements
	Heapsort(std::size_t n, Type min, Type max, FnRandomGenerator generator)
	{
		for (std::size_t i = 0; i < n; ++i)
		{
			Super::_elements.push_back(generator(min, max));
		}
	}

	// make a heap by an array, which is filled with elements of Type in arbitrary order
	Heapsort(Type* array, std::size_t n)
	{
		Super::_elements.assign(array, array + n);
	}

	template <std::size_t Size>
	Heapsort(std::array<Type, Size>& array)
	{
		Super::_elements.assign(array.begin(), array.end());
	}

	Heapsort() = default;

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