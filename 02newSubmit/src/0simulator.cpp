void simulator(string algo, string opts) {

	bool call_sched = false;
	Event rEvt;
	// while(wI < 100) {
	// while(CURRENT_Time <= 1805) {
	while(evtVec.size() > 0) {
		rEvt = evtVec[0];
		evtVec.erase(evtVec.begin());
		CURRENT_Time = rEvt.evtTStamp;
		rEvt.serial = wI;
// cout<<endl;		
// cout<<rEvt.evtTStamp<<" ID="<<rEvt.ID<<" "<<rEvt.curState<<" -> "<<rEvt.tran<<" cb="
// <<rEvt.cb<<" ib="<<rEvt.ib<<" rem="<<rEvt.rem<< " prio="<<rEvt.prio<< " SI="<<rEvt.serial <<endl;
// cout<<"#evtVec: "<<evtVec.size()<<" #rrVec: "<<rrVec.size() <<endl;
//////////////////////////////
// Event et1;
// for(int i = 0; i < rrVec.size(); i ++) {
// 	et1 = rrVec[i];
// 	cout<<"rrVec<<<<<< "" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<" prio="<<et1.prio <<endl;	
// }///////////////////////////////////////////////////////
// cout<<"-------------------------------"<<endl;
// for(int i = 0; i < evtVec.size(); i ++) {
// 	et1 = evtVec[i];
// 	cout<<"evtVec>>>>> "<<et1.evtTStamp<<" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<endl;	
// }///////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////

		if(rEvt.tran == "ready") {
			Event toRunEvt = tranReadyToRunFun(rEvt);		

			if (algo.find("F") != string::npos) fcfsSched(toRunEvt);
			else if (algo.find("L") != string::npos) lcfsSched(toRunEvt);
			else if (algo.find("S") != string::npos) sjfSched(toRunEvt);
			else if(algo.find("R") != string::npos) fcfsSched(toRunEvt);
			else if(algo.find("P") != string::npos) prioSched(toRunEvt);
			//rrVec.insert(rrVec.begin(), toRunEvt);
			call_sched = true;
		}
		else if(rEvt.tran == "run") {
			if(algo.find("R") != string::npos || algo.find("P") != string::npos) {
				if(rEvt.cb <= quantum) {
					Event toBlockEvt = rEvt;
					
					if(rEvt.cb == rEvt.rem) {
						toBlockEvt.curState = "done";
						toBlockEvt.tran = "";
					}
					else {						
						toBlockEvt.curState = "run";
						toBlockEvt.tran = "block";
						toBlockEvt.rem = rEvt.rem - rEvt.cb;
						toBlockEvt.ib = getRan(toBlockEvt.ibLimit);	
					}
					
					toBlockEvt.evtTStamp = rEvt.evtTStamp + rEvt.cb;

					insertEvtByTime(toBlockEvt);				
				
				} else {

					Event toReadyEvt = rEvt;
					toReadyEvt.evtTStamp = rEvt.evtTStamp + quantum;
					toReadyEvt.curState = "run";
					toReadyEvt.tran = "ready";

					//toReadyEvt.cb = rEvt.cb - quantum;
					toReadyEvt.rem = rEvt.rem - quantum;

					insertEvtByTime(toReadyEvt);
				}

			} 
			else {
				Event toBlockEvt = toBlockEvtFun(rEvt);

				insertEvtByTime(toBlockEvt);
				
			}
		}
		else if(rEvt.tran == "block"){
			Event toReadyEvt;
			toReadyEvt = rEvt;
			toReadyEvt.evtTStamp = toReadyEvt.evtTStamp + toReadyEvt.ib;
			toReadyEvt.curState = "block";
			toReadyEvt.tran = "ready";

			insertEvtByTime(toReadyEvt);

			call_sched = true;
		}
		else if(rEvt.curState == "done") call_sched = true;
		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////

		if(call_sched) {
			if(isNextEventFun() == 0 && exRunForRR(algo) == 0 ) {			
				//
				if(algo.find("P") != string::npos && rrVec.size() == 0) {
					rrVec.swap(expiredRRvec);
					std::sort(rrVec.begin(), rrVec.end(), sortVecByPrioTStamp);		
				}

				if(rrVec.size() > 0) {
					Event readyEvt;
					//////////////////////////////					
					readyEvt = rrVec[0];
					rrVec.erase(rrVec.begin());

					readyEvt.evtTStamp = CURRENT_Time;

					if(algo.find("R") != string::npos || algo.find("P") != string::npos) {

						if(readyEvt.cb <= quantum) {
							readyEvt.cb = getRan(readyEvt.cbLimit);	
							if(readyEvt.cb >= readyEvt.rem) {
								readyEvt.cb = readyEvt.rem;
							}						
						} else {
							readyEvt.cb = readyEvt.cb - quantum;
						}
					} else {
						readyEvt.cb = getRan(readyEvt.cbLimit);

						if(readyEvt.cb >= readyEvt.rem) {
							readyEvt.cb = readyEvt.rem;
						}						
					}

					evtVec.insert(evtVec.begin(), readyEvt);
				}// if(rrVec.size() > 0) {	
				
			}//end of isNextEvenFun
			//fifoSched();
			call_sched = false;
		}//endof call_sched
		/////////////////////////////////////
		pastEvtVec.push_back(rEvt);
		wI ++;	
	}//end of while

	//cout<<"last save Evt serial "<<pastEvtVec[pastEvtVec.size()-1].serial<<endl;
}//End of simulator





