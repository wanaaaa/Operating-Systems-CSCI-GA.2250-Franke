#include <iostream>
using namespace std;

std::vector<string> definedSymVec, usedSymVec, actuallyUseVec,
	useListNotAcuallyUsedVec, definedNotusedVec;

void makeStcVecFun ();
void resultVecFun();
void warnStcFun();
int findFirstDefinedFun(string sym);
void warnStcFun();
//////////////////////////////////
void warnMessFun() {
	makeStcVecFun();
	resultVecFun();
	warnStcFun();

}//End of warnMessFun

void makeStcVecFun () {
	string stSym, actuallyUsed;
	int usedModule, actuUsedModule;


	for (int i = 0; i < allInfoStru.length; ++i) {
		stSym = allInfoStru.allWords[i];
		usedModule = allInfoStru.moduleCycles[i];

		if(allInfoStru.qDefineList[i]==1 &&allInfoStru.qSymbols[i] == 1) 
			definedSymVec.push_back(stSym);

		if(allInfoStru.qUsedSyms[i] == 1) {

			allInfoStru.qUsedNotActuUsed[i] = 1;
			
			for(int j= 0; j < allInfoStru.length; j ++) {
				actuallyUsed = allInfoStru.allWords[j];
				actuUsedModule = allInfoStru.moduleCycles[j];
				if(stSym == actuallyUsed && usedModule == actuUsedModule && allInfoStru.qActullyUsedSyms[j] == 1 ) {
					allInfoStru.qUsedNotActuUsed[i] = 0;
				}
			}

			usedSymVec.push_back(stSym);
		}  

		if(allInfoStru.qActullyUsedSyms[i] == 1){

			 actuallyUseVec.push_back(stSym);
		}
	}//End of for
	/////////////////////////////////
	int intFoundModule;
	stringstream stModule;
	for(int i = 0; i < allInfoStru.length; i++) {

		if(allInfoStru.qUsedNotActuUsed[i] == 1) {
			intFoundModule = allInfoStru.moduleCycles[i];

			for(int j = 0; j < passTwoResStc.length -1; j ++) {
				if (passTwoResStc.modules[j] != passTwoResStc.modules[j+1] && passTwoResStc.modules[j] == intFoundModule) {//
					stModule << intFoundModule;	

					passTwoResStc.errMess[j] = 
						"\nWarning: Module "+stModule.str()+": "+allInfoStru.allWords[i]+" appeared in the uselist but was not actually used";

				}

			}

		}

	}


}

void resultVecFun() {
	string stUsedSym, stAcUsed;
	int qUsed;
	for (int i = 0; i < usedSymVec.size(); i++){
		qUsed = 0;
		stUsedSym = usedSymVec[i];

		for (int j = 0; j < actuallyUseVec.size(); j++){
			stAcUsed = actuallyUseVec[j];

			if(stUsedSym == stAcUsed) qUsed --;
			//cout<<"stUsed; "<< stUsedSym <<" stAcUsed; "<<stAcUsed <<" qUsed; " <<qUsed<< endl;
		}

		if(qUsed == 0) useListNotAcuallyUsedVec.push_back(stUsedSym);
	}

	string stDefineSym;
	for (int i = 0; i < definedSymVec.size(); i++){
		qUsed = 0;
		stDefineSym = definedSymVec[i];
		for (int j = 0; j < usedSymVec.size(); j++){
			stUsedSym = usedSymVec[j];
			if(stDefineSym == stUsedSym) qUsed --;
			
		}

		if(qUsed == 0) definedNotusedVec.push_back(stDefineSym);
	}


}

int findFirstDefinedFun(string sym) {
	string stCompare;
	for(int i = 0; i < allInfoStru.length; i++) {
		stCompare = allInfoStru.allWords[i];
		if(sym == stCompare && allInfoStru.qSymbols[i] == 1) return allInfoStru.moduleCycles[i]; 
	}

	return -1;
}

void warnStcFun() {
	//cout<<"warnStcFun  "<<endl;

	warnMessStc.length = 0;
	int *scI = &warnMessStc.length;

	for(int i = 0; i < definedNotusedVec.size(); i++) {
		warnMessStc.symbols[*scI] = definedNotusedVec[i];
		warnMessStc.modules[*scI] = findFirstDefinedFun(definedNotusedVec[i]);
		warnMessStc.errMess[*scI] = " was defined but never used \n";

		*scI += 1;
	}

	for(int i = 0; i < useListNotAcuallyUsedVec.size(); i++) {
		warnMessStc.symbols[*scI] = useListNotAcuallyUsedVec[i];
		///////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		warnMessStc.modules[*scI] = findFirstDefinedFun(useListNotAcuallyUsedVec[i])+2;
		warnMessStc.errMess[*scI] = " appeared in the uselist but was not actually used \n";

		*scI += 1;
	}

}