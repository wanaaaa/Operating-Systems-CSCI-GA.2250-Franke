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
#include "0Scheds.cpp"

#include "0simulator.cpp"

void FinalStatistics();

int main(int argc, char *argv[]) {
	
	string algorithm = argv[1];
	algorithm = algorithm.erase(0, 2);
	//string options = argv[2];
	string fileName = argv[2];
	
	parser(argv[2]);	
	mFirstIssue();
	simulator(algorithm, "options");

	FinalStatistics();

	return 0;
}

void FinalStatistics() {
	int totoalTime, TotMove, maxWait;
	double avgTurnAround, avgWait;
	int totalInsturction = rawDataStc.tracks.size();
	std::vector<int> startTimeVec, issueTimeVec;
	startTimeVec.assign(totalInsturction, 0);
	issueTimeVec.assign(totalInsturction, 0);

	TotMove = 0; avgTurnAround = 0; avgWait = 0;
	Event evt;
	// cout<<endl;
	for(int i = 0; i < expiredEvtVec.size(); i++) {
		evt = expiredEvtVec[i];

		// cout<<i <<" t="<<evt.tStep<<" "<<evt.ID<<" "<<evt.operation<<" "
		// <<evt.currentTrack<<" pTra="<<evt.preTrack<<" SN="<<evt.serial<<" addTrac;"<<evt.addTrack <<endl;
		
		if(evt.operation == "add") {
			avgTurnAround += - evt.tStep;
			avgWait += - evt.tStep;
			startTimeVec[evt.ID] = evt.tStep;
		}
		if(evt.operation == "issue") {
			TotMove = TotMove+abs(evt.currentTrack - evt.preTrack);
			avgWait += evt.tStep;
			issueTimeVec[evt.ID] = evt.tStep;
		}
		if(evt.operation == "finish") {
			avgTurnAround += evt.tStep;
		}
	}

	maxWait = -1;
	int diff;
	for(int i = 0; i < startTimeVec.size(); i++) {
		diff = issueTimeVec[i] - startTimeVec[i];
		if(maxWait < diff || maxWait == -1) {
			maxWait = diff;
		}
	}

	totoalTime = expiredEvtVec[expiredEvtVec.size()-1].tStep;
	avgTurnAround = avgTurnAround /totalInsturction;
	avgWait= avgWait /totalInsturction;

	printf("SUM: %d %d %.2lf %.2lf %d\n", totoalTime, TotMove, avgTurnAround, avgWait, maxWait);

}



















