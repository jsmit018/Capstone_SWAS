#include "SimExec.h"
#include "InputInterface.h"


int main() {

	//Where database connection code goes when DES is ready

	InputReader inputReader;
	inputReader.ReadInputData();
//	inputReader.PrintEverything();
	cout << "reading is finished" << endl;

	return 0;
}