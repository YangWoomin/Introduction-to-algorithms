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

	template<bool Desc>
	bool validate(std::size_t idx1, std::size_t idx2)
	{
		if (Desc)
		{
			return _elements[idx1] >= _elements[idx2];
		}
		else
		{
			return _elements[idx1] <= _elements[idx2];
		}
	}

	void exchange(std::size_t idx1, std::size_t idx2)
	{
		Element temp = _elements[idx1];
		_elements[idx1] = _elements[idx2];
		_elements[idx2] = temp;
	}

	std::size_t parent(std::size_t idx)
	{
		if (1 < idx)
		{
			return (idx - 1) / 2;
		}

		return 0;
	}

	std::size_t leftChild(std::size_t idx)
	{
		return idx * 2 + 1;
	}

	std::size_t rightChild(std::size_t idx)
	{
		return (idx + 1) * 2;
	}

public:
	Heap() = default;

	Heap(std::vector<Element>& other)
		: _elements(other)
	{
		
	}

	void Heapify(std::size_t idx, std::size_t size)
	{
		std::size_t target = idx;
		std::size_t left = leftChild(idx);
		std::size_t right = rightChild(idx);
		if (size > left && false == validate<Desc>(idx, left))
		{
			target = left;
		}
		if (size > right && false == validate<Desc>(target, right))
		{
			target = right;
		}
		if (idx != target)
		{
			exchange(idx, target);
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
