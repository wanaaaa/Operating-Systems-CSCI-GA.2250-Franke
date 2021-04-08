void readFileFun(char str[], int ii);
void initEveVecFun() ;
/////////////////////////////////////////
void parser(char fileName[]) {
	readFileFun(fileName, 0);
}
////////////////////////////////////
void readFileFun(char str[], int intFile) {
	std::vector<char> stVec;

	if (FILE *fp = fopen(str, "r")) {

		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp)){
			stVec.insert(stVec.end(), buf, buf + len);
			// cout<<"In readFileFun: "<<buf<<endl;
		}

		fclose(fp);
	}

	char tempChar;
	string tempSt = "";
	int qPound = 0;

	for(int i = 0; i < stVec.size(); i ++) {
		tempChar = stVec[i];

		if (tempChar == '#') qPound = 1;
		if(tempChar ==' '|| tempChar=='\n') {

			if(qPound == 1 ) {
				if(tempChar == '\n') {
					tempSt = "";
					qPound = 0;					
				}
			}
			else {
				if(tempChar == ' ') {
					rawDataStc.tSteps.push_back(atoi(tempSt.c_str()));
					tempSt = "";
				}
				else if(tempChar == '\n'){
					rawDataStc.tracks.push_back(atoi(tempSt.c_str()));
					tempSt = "";
				}

			}
		} else tempSt += tempChar;


	}//End of for

	// for(int i = 0; i < rawDataStc.tSteps.size(); i++) {
	// 	cout<<"readFileFun tStep: "<<rawDataStc.tSteps[i]<<" track: "<<rawDataStc.tracks[i] <<endl;
	// }

	initEveVecFun();
}

void initEveVecFun() {
	for(int i = 0; i < rawDataStc.tSteps.size(); i ++) {
		Event nEvt;
		nEvt.ID = i;
		nEvt.tStep = rawDataStc.tSteps[i];
		nEvt.operation = "add";
		nEvt.currentTrack = rawDataStc.tracks[i];
		evtVec.push_back(nEvt);
	}	
}