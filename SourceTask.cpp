#include "SourceTask.h"

SourceBlock::SourceBlock() : Task() {

}

class SourceBlock::ScheduleNextEntityEA : public EventAction {

};

Time SourceBlock::GetInterarrivalTime() {

}

void SourceBlock::SetInterarrivalTime(Time interarrivalTime) {

}

string SourceBlock::GetAircraftType() {

}

void  SourceBlock::SetAircraftType(string aircraftType) {

}

int SourceBlock::GetNumberOfAircraftToGenerate() {

}

void SourceBlock::SetNumberOfAircraftToGenerate(int numberOfAircraftToGenerate) {

}

string SourceBlock::GetName() {

}

void SourceBlock::SetName(string name) {

}

Time SourceBlock::GetTimeForFirstAircraft() {

}

void SourceBlock::SetTimeforFirstAircraft(Time setTime) {

}

int SourceBlock::GetNumberGenerated() {

}

void SourceBlock::SetNumberGenerated(int numberToGenerate) {

}

void SourceBlock::ScheduleNextEntityEM(Airplane* airplane) {

}
