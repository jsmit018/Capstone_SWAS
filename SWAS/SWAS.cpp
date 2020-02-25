#include "SimExec.h"
#include "InputInterface.h"


int main() {

	//Where database connection code goes when DES is ready

	InputReader inputReader;
	inputReader.ReadInputData();
	cout << "read finished" << endl;

	return 0;
}