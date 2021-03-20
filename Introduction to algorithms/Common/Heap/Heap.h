#pragma once

#include <vector>

/*
	If there is no definition of comparison operator(<=,>=) in Element, 
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

	void exchange(Element& element1, Element& element2)
	{
		Element temp = element1;
		element1 = element2;
		element2 = temp;
	}

public:
	Heap()
	{

	}

	Heap(std::vector<Element>& other)
		: _elements(other)
	{

	}

	void Heapify(std::size_t idx, std::size_t size)
	{
		std::size_t target = idx;
		std::size_t left = idx * 2 + 1;
		std::size_t right = (idx + 1) * 2;
		if (size > left && !compare<Desc>(_elements[idx], _elements[left]))
		{
			target = left;
		}
		if (size > right && !compare<Desc>(_elements[target], _elements[right]))
		{
			target = right;
		}
		if (idx != target)
		{
			exchange(_elements[idx], _elements[target]);
			Heapify(target, size);
		}
	}

	void Build()
	{
		if (1 >= _elements.size())
		{
			return;
		}

		const std::size_t size = _elements.size();
		std::size_t idx = size / 2 - 1;
		while (0 <= idx)
		{
			Heapify(idx, size);

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
