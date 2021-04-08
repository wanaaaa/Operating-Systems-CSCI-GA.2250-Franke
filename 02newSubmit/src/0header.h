int CURRENT_Time;
int ranVecCount = 1;
int quantum;
int wI = 0;

std::vector<int> rawDataVec;
std::vector<int> randomVec;

class Event {
public:
	//int data;
	int evtTStamp, ID, cb, ib, rem, prio, cbLimit, ibLimit, staticPrio, serial
		, finalT, sumIB, CW;
	string curState;
	string tran;
	Event() {
		evtTStamp=-1; ID=-1; cb=-1; ib=-1; rem=-1; prio=-10; staticPrio=-10; serial = -1;
		finalT = -1; sumIB = 0; CW = 0;
		curState = "xxx"; tran = "xxx";
	}
};

std::vector<Event> evtVec;
std::vector<Event> rrVec;
std::vector<Event> pastEvtVec;
std::vector<Event> expiredRRvec;
std::vector<Event> initEvtVec;

std::vector<int> sta;
//////////////////////////////
int getRan(int num) {
	if(ranVecCount > randomVec[0]) ranVecCount = 1;
	int returnRan;
	returnRan = 1 + randomVec[ranVecCount] % num;

	ranVecCount ++;

	return returnRan;
}

Event tranReadyToRunFun(Event iEvt) {
	Event nEvt;
	nEvt = iEvt;

	nEvt.curState = "ready";
	nEvt.tran = "run";
	return nEvt;
}

int isNextEventFun() {
	int returnInt = 0;
	Event rEvt;
	for(int i = 0; i < evtVec.size(); i++) {
		rEvt = evtVec[i];
		if(rEvt.evtTStamp ==  CURRENT_Time || ((rEvt.tran == "block" || rEvt.curState=="done") && rEvt.evtTStamp > CURRENT_Time)) {
			returnInt = 1;
			break;
		} 
	}
	return returnInt;
}

void insertEvtByTime(Event iEvt) {
	Event rEvt;
	int i = 0;
	int foundInt = 0;
	while(i < evtVec.size()){

		rEvt = evtVec[i];
		if(rEvt.evtTStamp > iEvt.evtTStamp) {
			evtVec.insert(evtVec.begin()+i, iEvt );
			foundInt = 1;
			break;
		}

		i++;
	}

	if(foundInt == 0) evtVec.insert(evtVec.end(), iEvt);
}

Event toBlockEvtFun(Event rEvt){
	Event toBlockEvt;

	toBlockEvt = rEvt;
	toBlockEvt.evtTStamp = toBlockEvt.evtTStamp + toBlockEvt.cb;
	toBlockEvt.curState = "run";
	toBlockEvt.tran = "block";
	toBlockEvt.rem = toBlockEvt.rem - toBlockEvt.cb;
	
	if(toBlockEvt.rem == 0) {
		//cout<<"~~~~~~~~~~~~Done~~~~~~"<<endl;
		toBlockEvt.curState = "done";
		toBlockEvt.tran = " ";
	} else {				
		toBlockEvt.ib = getRan(toBlockEvt.ibLimit);
	}

	return toBlockEvt;
}















