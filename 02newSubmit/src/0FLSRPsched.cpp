bool sortVecByRemFun(Event aEvt, Event bEvt);
bool sorVecByPrioFun(Event aEvt, Event bEvt);
bool sortVecByTStamp(Event aEvt, Event bEvt);
bool sortVecByPrioTStamp(Event aEvt, Event bEvt);

int qWasCurBlockPrio(Event iEvt);
void moveExpireToRRvec();
////////////////////////////////////////
void fcfsSched(Event toRunEvt) {
	rrVec.push_back(toRunEvt);
}
void lcfsSched(Event toRunEvt) {
	rrVec.insert(rrVec.begin(), toRunEvt);
}
void sjfSched(Event toRunEvt) {
	rrVec.push_back(toRunEvt);
	std::sort (rrVec.begin(), rrVec.end(), sortVecByRemFun);
}

void prioSched(Event toRunEvt) {
	int qPrio0 = 0;
	int qSwaped = 0;
	int rrVecZero = 0;
	/////////////////////////////////////////
	///////////////////////////////////////////////
	//Event et1;
	// for(int i = 0; i < rrVec.size(); i ++) {
	// 	et1 = rrVec[i];
	// 	cout<<"Head in Loop rrVec >>>>>> "<<" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
	// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<" prio="<<et1.prio <<endl;	
	// }//
	// for(int i = 0; i < expiredRRvec.size(); i ++) {
	// 	et1 = expiredRRvec[i];
	// 	cout<<"Head in Loop expiredRRvec <<<<<< "<<" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
	// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<" prio="<<et1.prio <<endl;	
	// }//		
	////////////////////////////////////////
	Event *fEvt, pEvt ;
	if(pastEvtVec.size() > 0) pEvt = pastEvtVec[pastEvtVec.size()-1];

	if(toRunEvt.cb != -1) {
		//cout<<"prioSched pEvt: id="<<pEvt.ID<<" "<<pEvt.curState<<" -> "<<pEvt.tran<<endl;
		if( toRunEvt.prio == 0 || qWasCurBlockPrio(toRunEvt) == 1) {
			toRunEvt.prio = toRunEvt.staticPrio;
			qPrio0 = 1;
		} else {			
			toRunEvt.prio += -1;
		}		
	}
	////////////////////////////////////////
	if(qPrio0 == 1 && qWasCurBlockPrio(toRunEvt) == 0) {
			expiredRRvec.push_back(toRunEvt);
	} else {
		rrVec.push_back(toRunEvt);			
	}

	std::sort(rrVec.begin(), rrVec.end(), sortVecByPrioTStamp);	
	fEvt = &rrVec[0];

	Event tempEvt;
	if(rrVec.size() != 0 && toRunEvt.ID == fEvt->ID) {
		if(fEvt->prio <= rrVec[1].prio) {
			tempEvt = rrVec[0];

			rrVec.erase(rrVec.begin());
			rrVec.insert(rrVec.end(), tempEvt);
		}
	}
	/////////////////////////////////////////////////
	if(rrVec.size() == 0 && wI > 0) {
		rrVec.swap(expiredRRvec);;
		std::sort(rrVec.begin(), rrVec.end(), sortVecByPrioTStamp);	
	}

	// Event et1;
	// for(int i = 0; i < rrVec.size(); i ++) {
	// 	et1 = rrVec[i];
	// 	cout<<"Loop End rrVec >>>>>> SI="<<et1.serial<<" TStamp="<<et1.evtTStamp <<" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
	// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<" prio="<<et1.prio <<endl;	
	// }//
	// for(int i = 0; i < expiredRRvec.size(); i ++) {
	// 	et1 = expiredRRvec[i];
	// 	cout<<"in Loop End expiredRRvec <<<<<< "<<" ID="<<et1.ID<<" "<<et1.curState<<" -> "<<et1.tran<<" cb="
	// 	<<et1.cb<<" ib="<<et1.ib<<" rem="<<et1.rem<<" prio="<<et1.prio <<endl;	
	// }//	
	//////////////////////////////////////////////////////

}//end of prioSched
//////////////////////////////////////////
void moveExpireToRRvec() {
	for(int i = 0; i < expiredRRvec.size();i++) {
		rrVec.push_back(expiredRRvec[i]);
	}

	expiredRRvec.clear();
}

int qWasCurBlockPrio(Event iEvt) {
	int returnInt = 0;
	Event rEvt;
	for(int i = pastEvtVec.size()-1; i >= 0; i --) {
		rEvt = pastEvtVec[i];
		if(rEvt.ID == iEvt.ID) {
			if(rEvt.tran == "block") returnInt = 1;
			break;
		}
	}
	return returnInt;
}

int exRunForRR(string algo) {
	int returnInt = 0;
	if( algo.find("R") != string::npos || algo.find("P") != string::npos) {
		Event rEvt;
		for(int i = 0; i < evtVec.size(); i++) {
			rEvt = evtVec[i];
			if(rEvt.curState == "run" && rEvt.evtTStamp >= CURRENT_Time) 
				returnInt = 1; 
		}
	}
		
	return returnInt;
}
bool sortVecByRemFun(Event aEvt, Event bEvt) {
	return aEvt.rem < bEvt.rem;
}

bool sorVecByPrioFun(Event aEvt, Event bEvt) {
	return aEvt.prio > bEvt.prio;
}

bool sortVecByTStamp(Event aEvt, Event bEvt) {
	return aEvt.evtTStamp < bEvt.evtTStamp;
}

bool sortVecByPrioTStamp(Event aEvt, Event bEvt) {
	if(aEvt.prio > bEvt.prio) return true;
	if(aEvt.prio < bEvt.prio) return false;

	if(aEvt.evtTStamp < bEvt.evtTStamp) return true;
	if(aEvt.evtTStamp > bEvt.evtTStamp) return false;

	if(aEvt.serial < bEvt.serial) return true;
	if(aEvt.serial > bEvt.serial) return false;

	return false;

}





















