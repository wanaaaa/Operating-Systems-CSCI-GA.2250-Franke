int gBackwardSameTimeIssueSCAN(int outPosi);
//---------------------------------------
bool sortRuleDiffabs(Event aEvt, Event bEvt) {

	if(aEvt.diff < bEvt.diff) return true;
	if(aEvt.diff > bEvt.diff) return false;

	if(aEvt.tStep < bEvt.tStep) return true;
	if(aEvt.tStep > bEvt.tStep) return false;

	if(aEvt.serial < bEvt.serial) return true;
	if(aEvt.serial > bEvt.serial) return false;

	return false;
}
///////////////////////
bool sortRuleCurTrack(Event aEvt, Event bEvt) {

	if(aEvt.currentTrack < bEvt.currentTrack) return true;
	if(aEvt.currentTrack > bEvt.currentTrack) return false;

	if(aEvt.serial < bEvt.serial) return true;
	if(aEvt.serial > bEvt.serial) return false;

	return false;
}
//////////////////////////////////////////////
//////////////////////////////////////////////
void sortIssueSSTF(Event inEvt) {

	for(int i = 0; i < issueVec.size(); i++) {
		issueVec[i].diff = abs(inEvt.addTrack - issueVec[i].currentTrack);
	}

	std::sort(issueVec.begin(), issueVec.end(),  sortRuleDiffabs);
}
/////////////////////
void sortIssueFIFO(Event inEvt) {};
/////////////////////
void sortIssueCSCAN(Event inEvt) {

	std::sort(issueVec.begin(), issueVec.end(),  sortRuleCurTrack);

	int finishTrack = inEvt.addTrack;
	int outPosi = 0;
	Event outEvt = issueVec[0]; 
	int wI = 0;
	Event rEvt;
	while(wI < issueVec.size()) {
		rEvt = issueVec[wI];
		if(rEvt.currentTrack >= finishTrack) {
		// if(rEvt.currentTrack > finishTrack) {
			outPosi = wI;
			outEvt = issueVec[outPosi];
			break;
		}
		wI++;
	}

	if(wI == issueVec.size() 
		&& issueVec[issueVec.size()-1].currentTrack == inEvt.currentTrack ) {
		outPosi = issueVec.size()-1; 
		outEvt = issueVec[outPosi];
	}

	// showAllelementInVec(issueVec, "issueVec <<<<<<< ");

	issueVec.erase(issueVec.begin() + outPosi);
	issueVec.insert(issueVec.begin(), outEvt);

}
////////////////////////////////////////////
void sortIssueSCAN(Event inEvt) {
	std::sort(issueVec.begin(), issueVec.end(),  sortRuleCurTrack);

	//cout<<"sortIssueSCAN ======= goForward"<<goForward <<endl;
	
	int finishTrack = inEvt.addTrack;
	int outPosi = 0;
	Event outEvt = issueVec[0]; 
	int wI = 0;
	Event rEvt;
	while(wI < issueVec.size()) {
		rEvt = issueVec[wI];
		if(rEvt.currentTrack >= finishTrack) {
		// if(rEvt.currentTrack > finishTrack) {
			outPosi = wI;
			//outEvt = issueVec[outPosi];
			break;
		}
		wI++;
	}

	if(goForward == 1) {
		if(wI == issueVec.size()) {
			outPosi = issueVec.size() -1;
			goForward = 0;
		} 

		// else if (issueVec[issueVec.size() -1].currentTrack < finishTrack) {
		// 	goForward = 0;
		// }
	} else if(goForward == 0) {
		if(wI == issueVec.size()) {
			outPosi = issueVec.size() -1;
		} else if(wI == 0) {
			outPosi = 0;
			goForward = 1;
		} else {
			outPosi --;
		}

		//check if has same one....
		outPosi = gBackwardSameTimeIssueSCAN(outPosi);
		//chedk if finishTrack == outging Issue
		if(outPosi < issueVec.size()-1 && issueVec[outPosi+1].currentTrack == finishTrack ) {
			outPosi = outPosi + 1;
		}
		// cout<<"wI ++++++++++++++" <<wI<<" vec size "<<issueVec.size()<<" outPosi="<<outPosi<<" goForward="<<goForward <<endl;
	}

	outEvt = issueVec[outPosi];

	// showAllelementInVec(issueVec, "issueVec <<<<<<< ");

	issueVec.erase(issueVec.begin() + outPosi);
	issueVec.insert(issueVec.begin(), outEvt);	
	
}
//-----------------
int gBackwardSameTimeIssueSCAN(int outPosi) {
	//cout<<"gBackwardSameTimeIssueSCAN  @@@@@@@@@@@@@@@@@@@@@@"<<endl;
	int returnInt = outPosi;
	for(int i = 0; i < outPosi; i++) {
		if(issueVec[i].currentTrack == issueVec[outPosi].currentTrack) {
			return i;
		}
	}
	return returnInt;
}

//int gBWardSameFinishTrack_Outging()
///////////////////////////////////////////////////
///////////////////////////////////////////////////
void sortIssueFSCAN(Event inEvt) {
	std::sort(issueVec.begin(), issueVec.end(),  sortRuleCurTrack);

	int finishTrack = inEvt.addTrack;
	int outPosi = 0;
	Event outEvt = issueVec[0]; 
	int wI = 0;
	Event rEvt;
	while(wI < issueVec.size()) {
		rEvt = issueVec[wI];
		if(rEvt.currentTrack >= finishTrack) {
		// if(rEvt.currentTrack > finishTrack) {
			outPosi = wI;
			//outEvt = issueVec[outPosi];
			break;
		}
		wI++;
	}

	if(goForward == 1) {
		if(wI == issueVec.size()) {
			outPosi = issueVec.size() -1;
			goForward = 0;
		}
	} else if(goForward == 0) {
		//cout<<"wI ++++++++++++++" <<wI<<" vec size "<<issueVec.size()<<" outPosi="<<outPosi <<endl;
		if(wI == issueVec.size()) {
			outPosi = issueVec.size() -1;
		} else if(wI == 0) {
			outPosi = 0;
			goForward = 1;
		} else {
			outPosi --;
		}
	}

	outEvt = issueVec[outPosi];

	// showAllelementInVec(issueVec, "issueVec <<<<<<< ");

	issueVec.erase(issueVec.begin() + outPosi);
	issueVec.insert(issueVec.begin(), outEvt);	

}

























