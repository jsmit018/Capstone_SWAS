#include "step.h"

Step::Step()
{
}

Time Step::GetServiceTime()
{
	return Time();
}

void Step::SetServiceTime(Time serviceTime)
{
}

string Step::GetName()
{
	return string();
}

void Step::SetName(string name)
{
}

int Step::GetNumberInQueue()
{
	return 0;
}

void Step::SetNumberInQueue(int NumberInQueue)
{
}

void Step::AcquireServerEM()
{
}

void Step::AddQueueEM()
{
}

void Step::ScheduleDoneServiceEM()
{
}

void Step::ReleaseServer()
{
}

void Step::DoneService()
{
}
