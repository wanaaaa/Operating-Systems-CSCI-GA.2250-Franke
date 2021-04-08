int simuTime = 0;
int evtSerial = 0;
int evtVecI = 0;
int allDone = 0;
int goForward = 1;
int firstOrSecIssueVec = 0;

struct RawData {
	std::vector<int> tSteps;
	std::vector<int> tracks;	
} rawDataStc;
//////////////////
class Event {
public:
	int serial, tStep, ID;
	string operation;
	int currentTrack, preTrack, addTrack, diff;
	
	Event() {
		serial = evtSerial; evtSerial ++; tStep = -1; ID = -1; operation = "xxx"; 
		currentTrack = -1; preTrack = -1; addTrack = -1; diff = -1;
	}
};

std::vector<Event> evtVec, issueVec, secIssueVec ,expiredIssueVec, expiredEvtVec;
/////////////////////////
void showEvtInformation(Event evt, string str) {
	cout<<str<<evt.tStep<<":"<<" ID="<<evt.ID<<" "<<evt.operation<<" "<<evt.currentTrack
	<<" preTra="<<evt.preTrack<<" SN="<<evt.serial<<" addT="<<evt.addTrack <<endl;
}
void showAllelementInVec(vector<Event> &iVec, string str) {
	Event evt;
	for(int i = 0; i < iVec.size(); i++) {
		evt = iVec[i];
		cout<<str<<" sT="<<simuTime<<" tStep="<<evt.tStep<<" "<<evt.ID<<" "<<evt.operation<<" "
		<<evt.currentTrack<<" pTra="<<evt.preTrack<<" SN="<<evt.serial<<" diff="<<evt.diff<<endl;
	}
}

////////////
void mFirstIssue() {
	Event iEvt; 
	iEvt.operation = "issue";
	iEvt.currentTrack = 0;

	expiredIssueVec.push_back(iEvt);
}

Event makeNewIssue(Event rEvt) {
	Event iEvt;

	iEvt.ID = rEvt.ID;
	iEvt.operation = "issue";
	iEvt.currentTrack = rEvt.currentTrack;
	return iEvt;
}

int haveFutureFinish() {

	for(int i = 0; i < evtVec.size(); i++) {
		if(evtVec[i].operation == "finish" && evtVec[i].tStep >= simuTime) 
			return 1;
	}

	return 0;
}


void insertEvtToVecByTime(vector<Event> &iVec, Event iEvt ) {
	int wI = 0;
	while(wI < iVec.size()) {
	
		if(iVec[wI].tStep > iEvt.tStep) {
			iVec.insert(iVec.begin()+wI, iEvt);
			break;
		}
		wI++;
	}

	if(wI == iVec.size()) iVec.push_back(iEvt);

	//showAllelementInVec(evtVec, ">>>>>");
}

int gPreAddTrack(int iID) {
	for(int i = 0; i < expiredEvtVec.size(); i++) {
		if(expiredEvtVec[i].operation == "add" && expiredEvtVec[i].ID == iID)
			return expiredEvtVec[i].tStep;
	}

	return -99;
}

void doIssueToFinish(Event iEvt) {
	Event fEvt;
	fEvt.tStep = abs(iEvt.currentTrack - iEvt.preTrack) + simuTime;
	fEvt.ID = iEvt.ID;
	fEvt.operation = "finish";
	fEvt.currentTrack = fEvt.tStep - gPreAddTrack(fEvt.ID) ;
	fEvt.addTrack = iEvt.currentTrack;
	//cout<<"doIssueToFinish fEvt"; showEvtInformation(fEvt);

	insertEvtToVecByTime(evtVec, fEvt);
	evtVecI ++ ;
}




















