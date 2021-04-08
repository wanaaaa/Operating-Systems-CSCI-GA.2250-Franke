#include <iostream>
using namespace std;

void parseErrWrite(int nErr, int nLine, int nOffSet);
void parseErrMess(int nErr, int nLine, int nOffSet);
int findLineNum(int ith);
int calculateOffSet(int start, int inEnd);
int findComingLineChange(int i);

void parseTwo() {

	allInfoStru.length = 0;
	string inStr;
	int *stcI = &allInfoStru.length;

	int defineStart = atoi(stringVecNoLineChange[0].c_str())*2+1;
	int useStart = -1;
	int programStart = 0;

	int firstModule = 1;
	int firstDefine = 1;
	int firstUse = 1;
	int firstProgram = 1;
	for(int i = 0; i < stringVecNoLineChange.size(); ++i) {
		
		inStr = stringVecNoLineChange[i];
		allInfoStru.allWords[i] = inStr;
		///////////////////////////////////
		allInfoStru.qSymbols[i] = 0;
		allInfoStru.qRelaAddress[i] = 0;
		allInfoStru.qUsedSyms[i] = 0;
		allInfoStru.qOpTypes[i] = 0;
		allInfoStru.qOpCodes[i] = 0;

		///for UseList
		if(defineStart == 0) useStart = atoi(inStr.c_str())+1;
		//for prgram test
		if(useStart == 0) programStart = atoi(inStr.c_str())*2+1;
		//for define
		if(programStart == 0) defineStart = atoi(inStr.c_str())*2+1;

		
		////////coding start
		if(defineStart > 0) {
			allInfoStru.qDefineList[i] = 1;
			if(firstDefine != 1) {
				if(defineStart % 2 == 0) {
					allInfoStru.qSymbols[i] = 1;
				} else {

					if(isDigit(inStr) == 0) {
						//Parse Error Parse Error Parse Error Parse Error
						//Parse Error Parse Error Parse Error Parse Error
						int nLine = findLineNum(i);
						int offsetFun = calculateOffSet(offSetStart, i);
						parseErrMess(0, nLine, offSetAt);
						//Parse Error Parse Error Parse Error Parse Error						
					}

					allInfoStru.qRelaAddress[i] = 1;
				}	
			} else {
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
		        if(i == stringVecNoLineChange.size()-1) {

					int nLine = findComingLineChange(i);
					if(nLine <= 1) parseErrMess(1, nLine, inStr.size()+1);
					else {
						int offsetFun = calculateOffSet(offSetStart, i);
						parseErrMess(1, nLine, offSetAt);
					}				        	
		        }
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
				if(atoi(inStr.c_str()) > 16) {
					int nLine = findLineNum(i);
					int offsetFun = calculateOffSet(offSetStart, i);
					parseErrMess(4, nLine, offSetAt);					
				}
				//Parse Error Parse Error Parse Error Parse Error
			}

			firstDefine = 0;
		} else allInfoStru.qDefineList[i] = 0;

		if(useStart > 0) {
			allInfoStru.qUseList[i] = 1;
			if(firstUse != 1 ) {
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
				////sym check
				if(isDigit(inStr) == 1) {
					int nLine = findLineNum(i);
					int offsetFun = calculateOffSet(offSetStart, i);
					parseErrMess(1, nLine, offSetAt);
				}
				//Parse Error Parse Error Parse Error Parse Error				
				allInfoStru.qUsedSyms[i] = 1;
			} else{
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
				if(atoi(inStr.c_str())> 16) {
					//cout<<"parseTwo /////////"<<endl;
					int nLine = findLineNum(i);
					int offsetFun = calculateOffSet(offSetStart, i);
					parseErrMess(5, nLine, offSetAt);
				}
				//Parse Error Parse Error Parse Error Parse Error
			}
			firstUse = 0;
			firstDefine = 1;
			firstProgram = 1;
		} else allInfoStru.qUseList[i] = 0;

		if(programStart > 0) {			
			
			allInfoStru.qProgramText[i] = 1;
			if(firstProgram != 1) {
				if(programStart % 2 == 0) allInfoStru.qOpTypes[i] = 1;
				else allInfoStru.qOpCodes[i] = 1;
			} else{
				//////////////////////////
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
				if(i == stringVecNoLineChange.size()-1) {
					int nLine = findLineNum(i);
					int offsetFun = calculateOffSet(offSetStart, i);
					parseErrMess(2, nLine, offSetAt+inStr.length());					
				}

				//cout<<"parseTwo programStart == "<<"inStr: "<< inStr<<" firstProgram: "<<firstProgram <<endl;
				
				if(atoi(inStr.c_str()) > 500) {
					int nLine = findLineNum(i);
					int offsetFun = calculateOffSet(offSetStart, i);
					parseErrMess(6, nLine, offSetAt);					
				}
				//Parse Error Parse Error Parse Error Parse Error
				//Parse Error Parse Error Parse Error Parse Error
			}
			firstProgram = 0;
			firstUse = 1;

		} else allInfoStru.qProgramText[i] = 0;

		if(i>= 1 && allInfoStru.qProgramText[i-1] == 1 &&allInfoStru.qDefineList[i] == 1) firstModule++;
		allInfoStru.moduleCycles[i] = firstModule;		

		// cout<<make4chars(inStr)<<" defSt: "<< defineStart<<" useStart: "<< useStart 
		// <<" programStart: "<< programStart<< endl;

		//for index
		programStart --;
		useStart --;
		defineStart --;


		*stcI += 1;
		
	}

	// for (int i = 0; i < allInfoStru.length; ++i){
	// 	cout<<"in passOne "<<allInfoStru.allWords[i] <<" isOpTypes "<<allInfoStru.isOpTypes[i] 
	// 	<<" isDigit "<< isDigit(allInfoStru.allWords[i])<<endl; 
	// }
}

void parseErrMess(int nErr, int nLine, int nOffSet) {
	string returnStr;
	ostringstream stLineNum, stOffset;

	string errMess[] = {
		"NUM_EXPECTED", "SYM_EXPECTED", "ADDR_EXPECTED", "SYM_TOO_LONG",
		"TO_MANY_DEF_IN_MODULE", "TO_MANY_USE_IN_MODULE", "TO_MANY_INSTR"
	};

	stLineNum << nLine;
	stOffset << nOffSet;

	returnStr="Parse Error line "+stLineNum.str()+" offset "+stOffset.str()+
		": "+errMess[nErr];

	parseErrString = returnStr;

	//cout<<"parseErrMess <<<<<<<<< "<< "nOffSet "<<nOffSet<<endl;
	writeFile(1);
}

int findLineNum(int ith) {
	// cout<<"findLineNum >>>>>>>>>>>> "<<ith<<endl;
	// for(int i = 0; i < rawStringVector.size(); i++) {
	// 	cout<<"rawStringVector findLineNum "<<rawStringVector[i] << endl;
	// }


	std::vector<int> iLineChangeVec;
	string str;
	int numLine = ith;
	int lineNum = 0;

	for(int i = 0; i <= ith; i++) {
		str = rawStringVector[i];
		if(str == "lineChange"){
			ith ++;
			lineNum ++;
			iLineChangeVec.push_back(i);

			//cout<<str<<" "<<"findLineNum "<<"lineNum: "<<lineNum<<" i: "<<i<<endl;
		} 

	}
	//offSetStart = rawStringVector[rawStringVector.size()-1] -lineNum;
	if(iLineChangeVec.size() == 0) return 1;
	
	//cout<<"findLineNum ????????????"<<"iLineChangeVec.size()-1 "<<iLineChangeVec.size() <<endl;
	offSetStart = iLineChangeVec[iLineChangeVec.size()-1] - lineNum+1;
	
	// cout<<"findLineNum "<<" numLine; "<<numLine<<"lineNum: "<<lineNum<<" offSetStart: "
	// <<offSetStart << endl;
	return ith - numLine+1;
	
}

int calculateOffSet(int start, int inEnd) {
	string sumSt = "";
	for(int i = start; i <= inEnd; i ++) {
		sumSt += stringVecNoLineChange[i];
	}

	offSetAt = sumSt.length()-stringVecNoLineChange[inEnd].length()+1+(inEnd-start);
	//cout<<"calculateOffSet " <<sumSt<<" offSetAt: "<<offSetAt <<endl;
}

int findComingLineChange(int ii) {
	int nlineChange = 0;
	for(int i = 0; i < rawStringVector.size(); i ++) {
		if(rawStringVector[i] == "lineChange") nlineChange ++;
	}

	//cout<<"sssssssssssssssssss "<<nlineChange<<endl;
	return nlineChange;
}
