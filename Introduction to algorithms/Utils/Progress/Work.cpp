#include "Work.h"

Work::Work(std::string name, std::size_t max, std::size_t unit)
	: _name(name), _max(max), _unit(unit)
{

}

bool Work::Proceed(std::size_t value)
{
	_currentTotal += value;
	_currentUnit += value;

	if (_unit <= _currentUnit)
	{
		_currentUnit = _currentUnit % _unit;
		return true;
	}
	else
	{
		return false;
	}
}

double Work::GetPercent()
{
	return (double)_currentTotal / _max;
}
