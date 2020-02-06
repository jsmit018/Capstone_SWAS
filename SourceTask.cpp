#include "SourceTask.h"
#include "SimExec.h"

SourceBlock::SourceBlock(Distribution* interarrivalTime, string aircraftType, int numberOfAircraftToGenerate
Time timeForFirstAircraft, Aircraft* aircraft) : Task(name) {
    _name = name;
    _interarrivalTime = interarrivalTime;
    _aircraftType = aircraftType;
    _aircraft = aircraft;
    _numberOfAircraftToGenerate = numberOfAircraftToGenerate;
   // _timeForFirstAircraft = timeForFirstAircraft;
    _numberGenerated = 0;
    //I believe we should change the name of this variable
    SimulationExecutive::ScheduleEventAt(timeForFirstAircraft, new ScheduleNextEntityEA(this));
}

class SourceBlock::ScheduleNextEntityEA : public EventAction {
public:
    ScheduleNextEntityEA(SourceBlock* source){
        _source = source;
    }
    void Execute(){
        _source->ScehduleNextEntityEM();
    }
private:
    SourceBlock* _source;
};

class SourceBlock::ScheduleNextRandomEntityEa : public EventAction {
public:
	ScheduleNextRandomEntityEa(SourceBlock* source) {
		_source = source;
	}

	void Execute() {
		_source->ScheduleNextRandomEntityEM();
	}

private:
	SourceBlock* _source;
};

//Changed to a distribution we no longer need
/*Time SourceBlock::GetInterarrivalTime() {

}*/

//This is handled in the constructor
/*void SourceBlock::SetInterarrivalTime(Time interarrivalTime) {

}*/

string SourceBlock::GetAircraftType() {
    return _aircraftType;
}

void  SourceBlock::SetAircraftType(string aircraftType) {
    _aircraftType = aircraftType;
}

int SourceBlock::GetNumberOfAircraftToGenerate() {
    _return _numberOfAircraftToGenerate();
}

//Handled in the Constructor
/*void SourceBlock::SetNumberOfAircraftToGenerate(int numberOfAircraftToGenerate) {

}*/

string SourceBlock::GetName() {
    return _name;
}

void SourceBlock::SetName(string name) {
    _name = name;
}

Time SourceBlock::GetTimeForFirstAircraft() {
    return _timeForFirstAircraft;
}

void SourceBlock::SetTimeforFirstAircraft(Time setTime) {
    _timeForFirstAircraft = setTime;
}

int SourceBlock::GetNumberGenerated() {
    return _numberGenerated;
}

//Don't need this the simulation will handle
/*void SourceBlock::SetNumberGenerated(int numberToGenerate) {

}*/

void SourceBlock::ScheduleNextEntityEM() {
   // while(_numberOFAircraftToGenerate != _numberGenerated){
        SimExec::ScheduleEventIn(_interarrivalTime->GetRV(), new ScheduleNextEntityEA(this));
		if (rand() >= .51) {
			SimExec::ScheduleEventIn(_interarrivalTimeRND->GetRV(), new ScheduleNextRandomEntityEa(this));
		}
        Depart(_aircraft->New());
        _numberGenerated++;
    //}
}

void SourceBlock::ScheduleNextRandomEntityEM() {
	Depart(_aircraft->New());
}


void SourceBlock::Execute(Aircraft* aircraft){
}
