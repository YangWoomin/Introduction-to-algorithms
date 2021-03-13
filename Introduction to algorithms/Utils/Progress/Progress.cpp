#include "Progress.h"
#include "ProgressManager.h"

void Progress::Initiate()
{
	ProgressManager& manager = ProgressManager::Get();
	manager.Start();
}

void Progress::Finalize()
{
	ProgressManager& manager = ProgressManager::Get();
	manager.Clear();
}

void Progress::Create(std::string name, std::size_t max, std::size_t unit)
{
	ProgressManager& manager = ProgressManager::Get();
	manager.Create(name, max, unit);
}

void Progress::Proceed(std::string name, std::size_t value)
{
	ProgressManager& manager = ProgressManager::Get();
	manager.Proceed(name, value);
}

void Progress::Message(std::string message)
{
	ProgressManager& manager = ProgressManager::Get();
	manager.Message(message);
}
