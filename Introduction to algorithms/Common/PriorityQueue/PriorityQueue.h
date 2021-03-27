#pragma once

#include "../Heap/Heap.h"

template <typename Element, bool Desc = true>
class PriorityQueue : public Heap<Element, Desc>
{
private:
	typedef Heap<Element, Desc> Super;

protected:
	void heapifyUpward(std::size_t idx)
	{
		std::size_t pidx = Super::parent(idx);
		while (1 <= idx && false == Super::validate<Desc>(pidx, idx))
		{
			Super::exchange(idx, pidx);
			idx = pidx;
			pidx = Super::parent(idx);
		}
	}

public:
	// how do I let a user know what is the element the user wants to find?
	bool Update(const Element& element)
	{
		return false;
	}

	void Insert(Element& element)
	{
		Super::_elements.push_back(element);
		heapifyUpward(Super::_elements.size() - 1);
	}

	bool Extract(Element& element)
	{
		const std::size_t size = Super::_elements.size();
		if (1 > size)
		{
			return false;
		}

		element = Super::_elements[0];
		Super::exchange(0, size - 1);
		Super::_elements.pop_back();
		Super::Heapify(0, size - 1);

		return true;
	}

	bool Pick(Element& element)
	{
		if (1 > Super::_elements.size())
		{
			return false;
		}

		element = Super::_elements[0];
		return true;
	}

	bool Validate()
	{
		const std::size_t size = Super::_elements.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			const std::size_t pidx = Super::parent(i);
			if (1 <= i && false == Super::validate<Desc>(pidx, i))
			{
				return false;
			}
		}

		return true;
	}
};
