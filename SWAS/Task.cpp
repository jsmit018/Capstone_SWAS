#include "task.h"
#include "SimExec.h"
#include "Aircraft.h"

void Task::SetNextTask(Task* nextTask)
{
	_nextTask = nextTask;  
}

void Task::Arrive(Aircraft* aircraft)
{
	//if (SimExec::GetSimulationTime()._timeOfDay >= 10.00) {
	//	cout << "Arrival at " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month) << " " << SimExec::GetSimulationTime()._day
	//		<< " at " << SimExec::GetSimulationTime()._timeOfDay << "00 in " << SimExec::GetSimulationTime()._year << " at " << _name <<
	//		" ID: " << aircraft->GetAircraftID() << endl;  
	//}
	//else {
	//	cout << "Arrival at " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month) << " " << SimExec::GetSimulationTime()._day
	//		<< " at 0" << SimExec::GetSimulationTime()._timeOfDay << "00 in " << SimExec::GetSimulationTime()._year << " at " << _name <<
	//		" ID: " << aircraft->GetAircraftID() << endl;  
	//}
	Execute(aircraft);  
}

void Task::SetName(string name)
{
	_name = name;  
}

string Task::GetName()
{
	return string();  
}

int Task::GetID()
{
	return 0;  
}

Task::Task(string name)
{
	_name = name;  
}

void Task::Depart(Aircraft* aircraft)
{
	/*if (SimExec::GetSimulationTime()._timeOfDay >= 10.00) {
		cout << "Aircraft Departing at " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month) << " " << SimExec::GetSimulationTime()._day
			<< " at " << SimExec::GetSimulationTime()._timeOfDay << "00 in " << SimExec::GetSimulationTime()._year << " from " << _name <<
			" ID: " << aircraft->GetAircraftID() << endl;  
	}
	else {
		cout << "Aircraft Departing at " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month) << " " << SimExec::GetSimulationTime()._day
			<< " at 0" << SimExec::GetSimulationTime()._timeOfDay << "00 in " << SimExec::GetSimulationTime()._year << " from " << _name <<
			" ID: " << aircraft->GetAircraftID() << endl;  
	}*/
	aircraft->SetSource(this->GetID());  
	_nextTask->Arrive(aircraft);  
}