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
#include <cmath>

using namespace std;

#include "0header.h"
#include "0parser.cpp"
#include "0FLSRPsched.cpp"
#include "0getStatistics.cpp"
// #include "0scanSched.cpp"
// #include "0cscanSched.cpp"
// #include "0fscanSched.cpp"

#include "0simulator.cpp"


int main(int argc, char *argv[]) {
	string verboseSt, algorithmSt, dFileName, rFileName, arOne, arTwo;
	if(argc == 5) {
		arOne = argv[1];
		arTwo = argv[2];

		if(arOne.find("v") != string::npos) {
		// cout<<"1111111111111 "<<argc <<"arTwo "<< arTwo <<endl;
			verboseSt = arOne;
			algorithmSt = arTwo;
		} else {
			verboseSt = arTwo;
			algorithmSt = arOne;
		}	
	} else if( argc ==4 ) {
		verboseSt = "no";
		algorithmSt = argv[1];
	}

	// cout<<"argv1="<<arOne<<" arTwo="<<argv[2]<<endl;
	// string verboseSt = argv[1];
	// string algorithmSt = argv[2];
	// string dFileName = argv[3];
	// string rFileName = argv[4];

	//string verboseSt = argv[1];
	//algorithmSt = argv[1];
	dFileName = argv[argc-2];
	rFileName = argv[argc-1];

	string quanSt = "";
	if(algorithmSt.find("R") != string::npos || algorithmSt.find("P") != string::npos) {
		for(int i = 0; i < algorithmSt.length(); i++) {
			if(isdigit( algorithmSt[i])) quanSt += algorithmSt[i];
			
		}
		quantum = atoi(quanSt.c_str());
	}

	// parser(argv[3], argv[4]);
	// parser(argv[2], argv[3]);
	parser(argv[argc -2], argv[argc -1]);
	simulator(algorithmSt, "options");
	getStatistics(algorithmSt, quanSt, dFileName);

	// for(int i = 0; i < rawDataVec.size(); i++) {
	// 	cout<<"main "<<rawDataVec[i]<<endl; 
	// 	cout<<"random: "<<randomVec[i]<<endl;
	// }

	// Event rEvt;
	// for(int i = 0; i < evtVec.size();i++) {
	// 	rEvt = evtVec[i];
	// 	cout<<" main time: "<<rEvt.evtTStamp<<" rEvt.rem: "<<rEvt.rem<<" cbL: "<<rEvt.cbLimit
	// 	<<" ibLimit: "<<rEvt.ibLimit<<" prio: "<<rEvt.prio <<endl;
	// }


	return 0;
}