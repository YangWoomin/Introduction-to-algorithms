
#include <iostream>
#include <iomanip>

#include "../../Common/Common/Generator.h"
#include "../../Common/PriorityQueue/PriorityQueue.h"

int main()
{
	PriorityQueue<int> pq;

	std::cout << "** Priority Queue test in descending order started **" << std::endl;

	for (std::size_t i = 0; i < 1000; ++i)
	{
		int temp = MakeRandomNum(1, 100);
		
		if (2 <= (temp % 3))
		{
			// pop
			int temp = 0;
			pq.Extract(temp);
			if (false == pq.Validate())
			{
				std::cout << "** Priority Queue is invalid after Extract **" << std::endl;
			}
			std::cout << "** Extract from Priority Queue, the number : " << std::setw(3) << temp << std::endl;
		}
		else
		{
			// push
			int temp = MakeRandomNum(1, 100);
			pq.Insert(temp);
			if (false == pq.Validate())
			{
				std::cout << "** Priority Queue is invalid after Insert **" << std::endl;
			}
			std::cout << "** Insert into Priority Queue, the number : " << std::setw(3) << temp << std::endl;
		}
	}

	int temp = 0;
	while (true == pq.Extract(temp))
	{
		std::cout << "** Extract from Priority Queue, the number : " << std::setw(3) << temp << std::endl;
		if (false == pq.Validate())
		{
			std::cout << "** Priority Queue is invalid after Extract **" << std::endl;
		}
	}

	std::cout << "** Priority Queue test in descending order finished **" << std::endl;

	return 0;
}