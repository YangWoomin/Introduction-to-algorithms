#include "ProgressManager.h"

ProgressManager::ProgressManager()
{
	InitializeCriticalSection(&_cs);
}

ProgressManager::~ProgressManager()
{
	Clear();
	DeleteCriticalSection(&_cs);
}

void ProgressManager::_Run()
{
	while (true == _running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		std::queue<Task> tasks;
		std::queue<std::string> messages;
		{
			EnterCriticalSection(&_cs);
			tasks.swap(_tasks);
			messages.swap(_messages);
			LeaveCriticalSection(&_cs);
		}

		while (false == tasks.empty())
		{
			Task task = tasks.front();
			Works::iterator finder = _works.find(task.first);
			if (_works.end() != finder)
			{
				_Do(finder->second, task);
			}
			tasks.pop();
		}

		while (false == messages.empty())
		{
			std::string message = messages.front();
			_Print(message);
			messages.pop();
		}
	}
}

void ProgressManager::_Do(Work* work, Task& task)
{
	if (nullptr != work)
	{
		if (true == work->Proceed(task.second))
		{
			double percent = work->GetPercent();
			std::string message = "Progress of " + task.first + " : ";
			message.append(std::to_string(percent));
			_Print(message);
		}
	}
}

void ProgressManager::_Print(std::string message)
{
	std::cout << "** " << GetNowLog() << " " << message << std::endl;
}

std::string ProgressManager::GetNowLog()
{
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	char buff[64] = { 0, };
	snprintf(buff, sizeof(buff), "[%04d-%02d-%02d %02d:%02d:%02d]",
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);
	return std::string(buff);
}

ProgressManager& ProgressManager::Get()
{
	static ProgressManager instance;
	return instance;
}

void ProgressManager::Start()
{
	if (nullptr == _thread)
	{
		_running = true;
		_thread = new std::thread(&ProgressManager::_Run, this);
	}
}

void ProgressManager::Stop()
{
	if (nullptr != _thread)
	{
		_running = false;
		_thread->join();
		delete _thread;
		_thread = nullptr;
	}
}

void ProgressManager::Clear()
{
	Stop();

	Works::iterator it = _works.begin();
	Works::iterator end = _works.end();
	for (; it != end; ++it)
	{
		if (nullptr != it->second)
		{
			delete it->second;
		}
	}
	_works.clear();
}

void ProgressManager::Create(std::string name, std::size_t max, std::size_t unit)
{
	Works::iterator finder = _works.find(name);
	if (_works.end() == finder)
	{
		Work* work = new Work(name, max, unit);
		_works.emplace(name, work);
	}
}

void ProgressManager::Proceed(std::string name, std::size_t value)
{
	EnterCriticalSection(&_cs);
	_tasks.push(Task(name, value));
	LeaveCriticalSection(&_cs);
}

void ProgressManager::Message(std::string message)
{
	EnterCriticalSection(&_cs);
	_messages.push(message);
	LeaveCriticalSection(&_cs);
}
