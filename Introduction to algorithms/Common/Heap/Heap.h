#pragma once

#include <vector>

/*
	If there is no definition of comparison operator(<,>) in Element, 
	you should define comparison operator of that type.
	And you should also define assignment operator(=) because they can need to be exchanged.
*/
template <typename Element, bool Desc = true>
class Heap
{
protected:
	std::vector<Element> _elements;

	template <bool Desc>
	bool compare(Element resident, Element challenger)
	{
		if (Desc)
		{
			return resident >= challenger;
		}
		else
		{
			return resident <= challenger;
		}
	}

public:
	Heap()
	{

	}

	Heap(std::vector<Element>& other)
		: _elements(other)
	{

	}

	void Heapify(std::size_t idx, std::size_t last)
	{
		std::size_t target = idx;
		std::size_t left = idx * 2 + 1;
		std::size_t right = idx * 2 + 2;
		if (last >= left && !compare<Desc>(_elements[idx], _elements[left]))
		{
			target = left;
		}
		if (last >= right && !compare<Desc>(_elements[target], _elements[right]))
		{
			target = right;
		}
		if (idx != target)
		{
			Element temp = _elements[idx];
			_elements[idx] = _elements[target];
			_elements[target] = temp;
			Heapify(target, last);
		}
	}

	void Build()
	{
		const std::size_t n = _elements.size() - 1;
		std::size_t idx = _elements.size() / 2 - 1;
		while (0 <= idx)
		{
			Heapify(idx, n);

			// for preventing underflow of std::size_t
			if (0 == idx)
			{
				break;
			}
			else
			{
				idx--;
			}
		}
	}
};
