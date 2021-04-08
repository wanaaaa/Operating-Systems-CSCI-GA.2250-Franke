#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm> 

#include <fstream> 
#include <vector>
#include <cstdlib>
#include <list>
#include <queue>
#include <map>

using namespace std;

#include "0header.h"
#include "0agingVMM.cpp"
#include "0RandomVMM.cpp"
// #include "0secondChanceVMM.cpp"
#include "0clockVMM_V.cpp"
#include "0secSecChance.cpp"
#include "0clockVMM.cpp"
#include "0fifoVMM.cpp"
#include "0agingVMM_V.cpp"
#include "0nruVMM.cpp"
#include "0parser.cpp"
#include "0simulator.cpp"


int main(int argc, char *argv[]) {
	cout<<"I am in main====="<<endl;
	string algorithm = argv[1];
	string options = argv[2];
	string frameSt = argv[3];
	string fileName = argv[4];
	
	numFrame = atoi(frameSt.c_str()) ;
	parser(argv[4], argv[5]);
	simulator(algorithm, options, frameSt, fileName );


	return 0;
}