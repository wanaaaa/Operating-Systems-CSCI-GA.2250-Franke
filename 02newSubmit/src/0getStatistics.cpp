void getStatistics(string algo, string quanSt, string inputFileName) {
	//cout<<"in getStatistics init vec size="<<initEvtVec.size()<<endl;
	int lastEvtTime;
	int cpuOn = 0; 
	int ioOn = 0;
	double cpuUtil = 0.0; 
	double ioUtil = 0.0;

	int wI = 0;
	Event rEvt;
	while(wI < pastEvtVec.size()) {
		rEvt = pastEvtVec[wI];

		/////////////////////////////////////////////////////////////////
// cout<<endl;		
// cout<<rEvt.evtTStamp<<" ID="<<rEvt.ID<<" "<<rEvt.curState<<" -> "<<rEvt.tran<<" cb="
// <<rEvt.cb<<" ib="<<rEvt.ib<<" rem="<<rEvt.rem<< " prio="<<rEvt.prio<< " SI="<<rEvt.serial <<endl;	
		/////////////////////////////////////////////////////////////////

		if(rEvt.curState == "done") {
			initEvtVec[rEvt.ID].finalT = rEvt.evtTStamp;
		}
		if(rEvt.tran == "block") {
			initEvtVec[rEvt.ID].sumIB += rEvt.ib;
		}

		if(rEvt.tran == "ready") {
			initEvtVec[rEvt.ID].CW -= rEvt.evtTStamp;
		}
		if(rEvt.curState == "ready") {
			initEvtVec[rEvt.ID].CW += rEvt.evtTStamp;
		}

		if(wI == pastEvtVec.size()-1) {
			lastEvtTime = rEvt.evtTStamp;
		}

		//CPU Util Calculate
		if(rEvt.curState == "ready" && rEvt.tran == "run"){

			cpuOn++;
			// cout<<"+++++++++++++ cpu Turn On "<<cpuOn<<endl;
			if (cpuOn == 1) cpuUtil -= (double) rEvt.evtTStamp;
		}
		if((rEvt.curState=="run" && rEvt.tran=="block")
			|| (rEvt.curState=="run"&&rEvt.tran=="ready") || rEvt.curState=="done") {

			cpuOn--;
			// cout<<"------------- cpu Turn Off "<<cpuOn<<endl;
			if(cpuOn == 0) cpuUtil += (double) rEvt.evtTStamp;
		}

		//IO Util Cal
		if(rEvt.curState == "run" && rEvt.tran == "block") {
			ioOn++;
			if(ioOn == 1) ioUtil -= (double) rEvt.evtTStamp;
		}
		if(rEvt.curState == "block" && rEvt.tran == "ready" ) {
			ioOn --;
			if(ioOn == 0) ioUtil += (double) rEvt.evtTStamp;
		}


		wI ++;	
	}//end of While

	//Print

	string algoSt;

	if(algo.find("F") != string::npos ) {
		algoSt = "FCFS";
		// algoPrint = "F";
	}
	if(algo.find("L") != string::npos ) {
		algoSt = "LCFS";
		// algoPrint = "L";
	}	
	if(algo.find("S") != string::npos ) {
		algoSt = "SJF";
		// algoPrint = "S";
	}	
	if(algo.find("R") != string::npos ) { 
		algoSt = "RR";
		// algoPrint = "R";
	}	
	if(algo.find("P") != string::npos ) {
		algoSt = "PRIO";
		// algoPrint = "P";
	}
	string outFileName;
	//cout<<"inputFileName==== "<<inputFileName<<" "<<inputFileName.find("/")<<endl;


	// if(inputFileName.find("/") != string::npos) 
	// 	inputFileName = inputFileName.replace(0, inputFileName.find("/")+1, "");
	

	// outFileName = inputFileName;
	// outFileName.replace(0, 5, "output");
	// outFileName = "../outputdir/"+outFileName+"_"+algoPrint+quanSt;

	
	//std::ofstream outfile (outFileName.c_str());
	//Sum start
	
	if(algo.find("R") != string::npos || algo.find("P") != string::npos) {
		cout<<algoSt<<" "<<quanSt<<endl;
		// outfile<<algoSt<<" "<<quanSt<<endl;
	} else {
		cout<<algoSt<<endl;
		// outfile<<algoSt<<endl;
	}


	double averTurnT, averCW, throughput100;
	averTurnT = 0; averCW = 0; throughput100 = 0;

	for(int i = 0; i < initEvtVec.size(); i++) {
		rEvt = initEvtVec[i];
		averTurnT += rEvt.finalT - rEvt.evtTStamp;
		averCW += rEvt.CW;

		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n", i, rEvt.evtTStamp, rEvt.rem
		, rEvt.cbLimit, rEvt.ibLimit, rEvt.prio+1, rEvt.finalT, rEvt.finalT- rEvt.evtTStamp
		, rEvt.sumIB, rEvt.CW);

		// outfile<<setfill('0')<<setw(4)<<i<<":  "<<setfill(' ')<<setw(3)<<rEvt.evtTStamp<<"  "<<setfill(' ')<<setw(3)
		// <<rEvt.rem<<"  "<<setfill(' ')<<setw(3)<<rEvt.cbLimit<<"  "<<setfill(' ')<<setw(3)<<rEvt.ibLimit <<" "
		// <<rEvt.prio+1<<" |  "<<setfill(' ')<<setw(4)<<rEvt.finalT<<"  "<<setfill(' ')<<setw(4)<<rEvt.finalT- rEvt.evtTStamp 
		// <<"  "<<setfill(' ')<<setw(4)<<rEvt.sumIB<<"  "<<setfill(' ')<<setw(4)<<rEvt.CW <<endl;
		// outfile.precision(4);
	}

	averTurnT = averTurnT/initEvtVec.size();
	averCW = averCW/initEvtVec.size();
	throughput100 = (double) initEvtVec.size()/ (double) lastEvtTime*100;
	cpuUtil = cpuUtil/(double)lastEvtTime*100;
	ioUtil = ioUtil/(double) lastEvtTime*100;
	
	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", lastEvtTime, cpuUtil, ioUtil, averTurnT, averCW, throughput100);

	// outfile<<"SUM: "<<lastEvtTime<<" "<<fixed<<setprecision(2)<<cpuUtil<<" "<<fixed
	// <<setprecision(2)<<ioUtil<<" "<<fixed<<setprecision(2)<<averTurnT<<" " <<fixed
	// <<setprecision(2)<<averCW<<" "<<fixed<<setprecision(3)<<throughput100 <<endl;

	// outfile.close();
}









