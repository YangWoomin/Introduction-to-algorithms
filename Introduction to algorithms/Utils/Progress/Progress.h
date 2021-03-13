#pragma once

#include <string>

class Progress
{
public:
	static void Initiate();
	static void Finalize();
	static void Create(std::string name, std::size_t max, std::size_t unit);
	static void Proceed(std::string name, std::size_t value);
	static void Message(std::string message);
};
