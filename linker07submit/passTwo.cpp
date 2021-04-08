#include <iostream>
using namespace std;

int passTwoEcal(int ith, int resultI);
string targetSymFun(int thSym, int thModule);
int newEOpCodeFun(string sym, int Ecode, int resuI);
int passTwoRcal(string stRcode, int thModule, int resultI);
int AexceedErrFun(int addre, int resultI);
int IerrCheck(int addre, int resultI) ;

void passTwo() {
	string str;

	passTwoResStc.length = 0;
	int *stcI = &passTwoResStc.length;
	int AIaddress;

	for (int i = 0; i < allInfoStru.length - 1; i++) {
		str = allInfoStru.allWords[i];

		if(allInfoStru.qOpTypes[i] == 1) {
			passTwoResStc.opTypes[*stcI] = str;

			if(str == "I" || str == "A") {
				AIaddress = atoi(allInfoStru.allWords[i+1].c_str());

				if(str=="A") AIaddress = AexceedErrFun(AIaddress,*stcI);
				if(str=="I") AIaddress = IerrCheck(AIaddress, *stcI);

				passTwoResStc.nAddress[*stcI] = AIaddress;
				passTwoResStc.modules[*stcI] = allInfoStru.moduleCycles[i];
			}
				
			if(str == "E") {
				passTwoResStc.nAddress[*stcI] = passTwoEcal(i, *stcI);
				passTwoResStc.modules[*stcI] = allInfoStru.moduleCycles[i];			
			}	
			if(str == "R") {
				passTwoResStc.nAddress[*stcI] = passTwoRcal(allInfoStru.allWords[i+1], 
					allInfoStru.moduleCycles[i], *stcI);
				passTwoResStc.modules[*stcI] = allInfoStru.moduleCycles[i];
			}

			std::ostringstream stNum;

			stNum << *stcI;
			passTwoResStc.index[*stcI] = make3chars(stNum.str()) ;

			*stcI += 1;	
		}//End of if

		//cout<<"passTwo "<< *stcI<<str <<endl;
	}//End of for	

}//End of passTwo
///////////////////////////////////////////////////////////////////////////////
int passTwoRcal(string stRcode, int thModule, int resultI) {
	int intRcode = atoi(stRcode.c_str());
	int newRcode;

	if(intRcode > 9999) {
		newRcode = 9999;
		passTwoResStc.errMess[resultI] = " Error: Illegal opcode; treated as 9999";		
	} else if(intRcode % 100 > 10) {
		intRcode = (intRcode/1000) * 1000;
		newRcode = intRcode + numOpcodeFun(thModule);
		passTwoResStc.errMess[resultI] = " Error: Relative address exceeds module size; zero used";
	} else newRcode = intRcode + numOpcodeFun(thModule);

	// cout<<"passTwoRcal================== "<<"stRcode: "<<stRcode<<" intRcode: "<<intRcode <<" newRcode: " 
	// <<newRcode<<" thModule: "<<thModule << endl;

	return newRcode;
}

int passTwoEcal(int ith, int resultI) {
	int iModule = allInfoStru.moduleCycles[ith];
	int EopCode = atoi( allInfoStru.allWords[ith+1].c_str() ) ;
	int symIth = EopCode % 100;
	string targetSym = targetSymFun(symIth, iModule);

	int newEOpCode = newEOpCodeFun(targetSym, EopCode, resultI);
	// cout<<"passTwoEcal "<< "iModule: "<<iModule<<" EopCode: "<<EopCode<<" SymIth: "
	// <<symIth<<" targetSym: "<<targetSym<<" newOpCode: "<<newOpCode  <<endl;

	return newEOpCode;
}

string targetSymFun(int thSym, int thModule) {

	for(int i = 0; i < allInfoStru.length; i++) {
		if(allInfoStru.moduleCycles[i] == thModule && allInfoStru.qUsedSyms[i] == 1 ) {			
			if( thSym == 0){

				allInfoStru.qActullyUsedSyms[i] = 1;
				
				//cout<<"targetSymFun ========= "<<allInfoStru.allWords[i] <<" moduleCycles; "<< thModule<< endl;
				return allInfoStru.allWords[i] ;
			} 
				
			else thSym --;
		}
	}
	return "notFound" ;
}

int newEOpCodeFun(string sym, int Ecode, int resuI) {
	int addingAddress, returnEcode;
	int found = 0;

	//cout<<"newEOpCodeFun passOneResultStc.length;========  "<<passOneResultStc.length<<endl;

	for(int i = 0; i < passOneResultStc.length; i++) {
		if(passOneResultStc.symbols[i] == sym) {
			addingAddress = passOneResultStc.finalAddress[i];
			found = 1;
			//cout<<"newOpCodeFun For if /// "<<"sym "<< sym << "addingAddress: "<<addingAddress<<endl;
		}	
	}

	//////////////////////////////////////////////
	//////////Error Check
	if (found == 0) {
		addingAddress = 0;
		passTwoResStc.errMess[resuI] = " Error: "+sym+" is not defined; zero used";
	}

	returnEcode = (Ecode / 1000)*1000 + addingAddress;
	//////////////////////////////
	if(sym == "notFound") {
		returnEcode = Ecode;
		passTwoResStc.errMess[resuI] = " Error: External address exceeds length of uselist; treated as immediate";
	}

	//cout<<"newOpCodeFun "<<" string sym " << sym <<endl;
	return returnEcode;
}

int AexceedErrFun(int addre, int resultI) {
	int returnAddre;
	if(addre % 1000 >= 512) {
		returnAddre = (addre /1000) * 1000;
		passTwoResStc.errMess[resultI] = " Error: Absolute address exceeds machine size; zero used";
	} 
	else returnAddre = addre;

	//cout <<"AexceedErrFun "<<resultI << endl;
	return returnAddre;
}

int IerrCheck(int addre, int resultI) {
	int returnAddre;

	if(addre > 9999) {
		returnAddre = 9999;
		passTwoResStc.errMess[resultI] = " Error: Illegal immediate value; treated as 9999";
	} else  returnAddre = addre;

	return returnAddre;
}