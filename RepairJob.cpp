#include "RepairJob.h"

RepairJob::RepairJob(string name, int listSize) {
    _name = name;
    _activeStep = 0;
    _stepList = new RepairJob*[listSize];
}

string RepairJob::GetName() {
    return _name;
}

void RepairJob::SetName(string name) {
    _name = name;
}

RepairJob* RepairJob::GetNextStep() {
    _activeStep++;
}

void RepairJob::AddStep(RepairJob* step, int listSpot) {
    _stepList[listSpot] = step;
}
