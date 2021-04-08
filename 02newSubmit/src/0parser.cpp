void readData(char str[], int dataRan);
void makeInitEvtFun();

void parser(char dFileName[], char rFileName[]) {
	// cout<<"In parser ===="<<endl;
	readData(dFileName, 0);
	readData(rFileName, 1);
	makeInitEvtFun();
}

void readData(char str[], int dataRan) {
	std::vector<char> stVec;

	if (FILE *fp = fopen(str, "r")) {

		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp)){
			stVec.insert(stVec.end(), buf, buf + len);
		}

		fclose(fp);
	}

	std::string tempSt = "";
	char tempChar;
	for(int i = 0; i < stVec.size(); i++) {
		tempChar = stVec[i];

		if(tempChar == ' ' || tempChar == '\n' || isdigit(tempChar) == 0) {
			if(tempSt != "") {
				if(dataRan == 0)
					rawDataVec.push_back(atoi(tempSt.c_str()));
				else if (dataRan == 1)
					randomVec.push_back(atoi(tempSt.c_str()));
			}

			tempSt = "";
		} else tempSt = tempSt+tempChar;		
	}
}

void makeInitEvtFun() {
	int nTh;
	Event nEvt;
	int nID = 0;
	// cout<<"1111111111111"<<endl;
	for(int i = 0; i < rawDataVec.size(); i++) {
		nTh = i % 4;
		if(nTh == 0) {
			nEvt.ID = nID;
			nID ++;
			nEvt.evtTStamp = rawDataVec[i];			
		} 
		else if ( nTh == 1) nEvt.rem = rawDataVec[i];
		else if(nTh == 2) nEvt.cbLimit = rawDataVec[i];
		else if (nTh == 3) {
			nEvt.ibLimit = rawDataVec[i];
			nEvt.prio = getRan(4)-1;
			nEvt.staticPrio = nEvt.prio;
			nEvt.curState = "created";
			nEvt.tran = "ready"; 
			evtVec.push_back(nEvt);
		}
	}

	initEvtVec = evtVec;
}