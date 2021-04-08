

int numOpcodeFun(int thModule); 
void passOneErrCheckFun();
void passOneDuplicateCheck();
int symAddressErrCheck(string sym, int relaAddre, int module) ;

void passOne() {
	string str, stSym;
	int relaAddre, nthModule, numOpcode, finalAddress, stcI;
	int firstDefinSym = 0;
	passOneResultStc.length = 0;

	for (int i = 0; i < allInfoStru.length - 1; i++) {
		str = allInfoStru.allWords[i];
		if(allInfoStru.qDefineList[i] == 1 && allInfoStru.qSymbols[i]== 1) {
			stSym = str;
			////////////////////////////////////////////
			nthModule = allInfoStru.moduleCycles[i];			

			relaAddre = symAddressErrCheck(str, atoi(allInfoStru.allWords[i+1].c_str()), nthModule);
			//relaAddre =atoi(allInfoStru.allWords[i+1].c_str());
			////////////////////////////////////////////
			numOpcode = numOpcodeFun(nthModule);
			finalAddress = relaAddre + numOpcode;

			passOneResultStc.symbols[passOneResultStc.length] = str;
			passOneResultStc.finalAddress[passOneResultStc.length] = finalAddress;
			passOneResultStc.length++;

			// cout<<"passOne "<<make4chars(str)<< " relaAddre; "<< relaAddre 
			// <<" numOpcode: "<<numOpcode<<"finalAddress: "<<finalAddress<<    endl;
		}

	}//end of for

	passOneErrCheckFun();
}


int numOpcodeFun(int thModule) {
	int returnInt;
	int numOpcode = 0;
	if (thModule == 0) return numOpcode;

	for(int i = 0; i < allInfoStru.length; i++) {
		if(allInfoStru.moduleCycles[i] == thModule){
			
			return numOpcode;
		} 
		if(allInfoStru.qOpTypes[i] == 1) numOpcode ++;
	}

	return -1;
}

void passOneErrCheckFun() {

	passOneDuplicateCheck();
	//passOneBigAddreCheck();
}

void passOneDuplicateCheck() {
	int qUnique;
	string stControl, stCompare;
	
	psOneResulTemStc.length = 0;
	int *sI = &psOneResulTemStc.length; 
	//*sI = 0;

	for(int i = 0; i < passOneResultStc.length -1; i++) {
		stControl = passOneResultStc.symbols[i];
		qUnique = 1;

		for(int j = i+1; j < passOneResultStc.length; j++) {
			stCompare = passOneResultStc.symbols[j];
			if(stControl == stCompare){
				passOneResultStc.finalAddress[j] = passOneResultStc.finalAddress[i];
				passOneResultStc.errMess[j] = " Error: This variable is multiple times defined; first value used";
				qUnique--; 
			} 

			// cout<<"passOneDuplicateCheck "<<"stControl; "<<stControl <<"stCompare: "<<stCompare
			// <<" qUnique; "<<qUnique <<endl;
		}

		if(qUnique == 1) {
			psOneResulTemStc.symbols[*sI] = passOneResultStc.symbols[i];
			psOneResulTemStc.finalAddress[*sI] = passOneResultStc.finalAddress[i];
			psOneResulTemStc.errMess[*sI] = passOneResultStc.errMess[i];
			*sI += 1;
		}		
	}//End of for

	int intLast = passOneResultStc.length - 1;
	psOneResulTemStc.symbols[*sI] = passOneResultStc.symbols[intLast];
	psOneResulTemStc.finalAddress[*sI] = passOneResultStc.finalAddress[intLast];
	psOneResulTemStc.errMess[*sI] = passOneResultStc.errMess[intLast];
	*sI += 1;

	int *ssI = &passOneResultStc.length;
	*ssI = 0;
	for (int i = 0; i < psOneResulTemStc.length; i++) {
		passOneResultStc.symbols[i] = psOneResulTemStc.symbols[i];
		passOneResultStc.finalAddress[i] = psOneResulTemStc.finalAddress[i];
		passOneResultStc.errMess[i] = psOneResulTemStc.errMess[i];
		*ssI += 1;
		//cout<< "psOneResulTemStc.symbols "<<psOneResulTemStc.symbols[i]<< <<endl;
	}

}//End of passOneDup...
/////////////////////////////////////////////
int symAddressErrCheck(string sym, int relaAddre, int module) {
	int returnAddre;
	int maxAddreNum = 0;
	string stErr;
	ostringstream stRelaAddre, stModule, stMax;

	for(int i = 0; i < allInfoStru.length; i++) {
		if(allInfoStru.qOpCodes[i] == 1) {
			maxAddreNum++;
		}
	}



	if(relaAddre >= maxAddreNum  ) {
		returnAddre = 0;
		stRelaAddre << relaAddre;
		stModule << module;
		stMax << maxAddreNum -1;
		stErr = "Warning: Module "+stModule.str()+": "+sym+" too big "+stRelaAddre.str()+" (max="+stMax.str()+") assume zero relative";

		//cout<<"symAddressErrCheck "<<sym<<"stErr; "<<stErr<<endl;

		headWarnMessVec.push_back(stErr);
	} else returnAddre = relaAddre;

	return returnAddre;
}

