#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <queue>

#include <Windows.h>

#include "Work.h"

class ProgressManager
{
private:
	CRITICAL_SECTION _cs;
	std::atomic<bool> _running = false;
	std::thread* _thread = nullptr;
	typedef std::unordered_map<std::string, Work*> Works;
	Works _works;
	typedef std::pair<std::string, std::size_t> Task;
	std::queue<Task> _tasks;
	std::queue<std::string> _messages;

	ProgressManager();
	~ProgressManager();
	void _Run();
	void _Do(Work* work, Task& task);
	void _Print(std::string message);
	std::string GetNowLog();

public:
	static ProgressManager& Get();

	void Start();
	void Stop();
	void Clear();
	
	void Create(std::string name, std::size_t max, std::size_t unit);
	void Proceed(std::string name, std::size_t value);
	void Message(std::string message);
};
