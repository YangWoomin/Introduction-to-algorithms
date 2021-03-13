#pragma once

#include <string>

class Work
{
private:
	const std::string _name;
	const std::size_t _max = 0;
	const std::size_t _unit = 1;
	std::size_t _currentTotal = 0;
	std::size_t _currentUnit = 0;

public:
	Work(std::string name, std::size_t max, std::size_t unit);
	bool Proceed(std::size_t value);
	double GetPercent();
};
