int findClock_VFramePosi() ;
int clockNextPosiHandMove();
void clockHandTick(int ticks);
void vRefBitclear();
int qVRefBitFull();

int findSChanceFramePosi();
void setFramePosi(int posi);
int qFrameRefFull();
void frameRefZero();
void frameShift(int aClick);


void clockVMM_V(int sI)  {
	//cout<<"secondChanceVMM ======="<<endl;
	
	std::vector<int> RWVec = rawDataStc.RWoperVec; 
	std::vector<int> vPVec = rawDataStc.vPageVec;

	Event nEvt;
	nEvt.index = sI;
	nEvt.instruction[0] = RWVec[sI];
	nEvt.instruction[1] = vPVec[sI];

	int vmPosi = vPVec[sI];

	int *fMapArr = frameStc.frameMap;
	unsigned *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	int *VpresentBit = vmmStc.presentBit;
	int *VreferenceBit = vmmStc.referenceBit;
	int nfposi;// = findFramePosi();

	//statistics////////////////////////////////////////
	//int *VpresentBit = vmmStc.presentBit;
	int *VsecRefBit = vmmStc.secRefBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	string *VpageOutBit = vmmStc.pageOutBit;
	///////////////////////////		

	vMenInstrucArr[vmPosi] = nEvt.instruction[0];

	if(qMapped(vmPosi) != 1 ) {
		// if(numMapping % numFrame == 0 && sI != 0) {
		if(qFrameRefFull() == 1) {
			frameRefZero();
			initialQFrameFull = 1;
			//cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}

		if(qVRefBitFull() == 1) {
			// cout<<"clockVMM_V qVRefBitFull == 1 ++++++++++++++++"<<endl;
			vRefBitclear();
		}	 
		////////////////////////////
		nfposi = findClock_VFramePosi();


		if(qFrameFull() == 1) {
			//cout<<"begin to unMap ======== "<<endl;
			Status unMapSta;
			unMapSta.pStatus = "UNMAP";
			unMapSta.v = fMapArr[nfposi];

			VpresentBit[unMapSta.v] = 0;
			VreferenceBit[unMapSta.v] = 0;

			unMapSta.f = nfposi;
			nEvt.statusVec.push_back(unMapSta);

			//statistics///////////////////
			numUnMap++;

			if(numMapping >= numFrame ) {
				if(qVwriteOutSec(unMapSta.v) == 1) {
		cout<<"clockVMM_V &&&&&&&&& statistics+++++++++++++++ "<<endl;
					numOut++;
				}
			}

			VsecRefBit[unMapSta.v] = -9;
			VmodifiedBit[unMapSta.v] = "-";
			///////////////
			///////////////

		} 	
		////////////////////////////	
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
		VpresentBit[vmPosi] = 1;
		VreferenceBit[vmPosi] = 1;
        ///////////////////////////////
		//cout<<"fifoVMM test ===================="<< vmPosi<<endl;	
		Status zeroSta;
		zeroSta.pStatus = "ZERO";
		zeroSta.v = -1;
		zeroSta.f = nfposi;
		nEvt.statusVec.push_back(zeroSta);	
		

		Status mapStatus;
		mapStatus.pStatus = "MAP";
		mapStatus.v = vmPosi;
		mapStatus.f = nfposi;

		fMapArr[nfposi] = vmPosi;
		///////////////

		fRefArr[nfposi] = 1;
		//////////////////////////////////
		setFramePosi(nfposi);
		nEvt.statusVec.push_back(mapStatus);	

		numMapping ++;

		//Statistics////////////////////
		numMap ++;

		VsecRefBit[mapStatus.v] = 1;
		if(RWVec[sI] ==1) VmodifiedBit[mapStatus.v] = "m";

		if(numMapping >= numFrame ) {
			if(qWriteIn(mapStatus.v, sI) == 1) {
				numIn ++;
			}
		}
		////////////////////////		

	} 
	else {
		int fPosi = gMappedPosi(vmPosi);
		fRefArr[fPosi] = 1;
		VreferenceBit[vmPosi] = 1;
		//statistics
		if(RWVec[sI] == 1) {
			VmodifiedBit[vmPosi] = "m";
		}
		//////////////////////////////

	};

	evtVec.push_back(nEvt);
} 

///////////////////////////////////////
int findClock_VFramePosi() {
	// int *fMapArr = frameStc.frameMap;
	// int *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	int returnInt = lastFramePos;

	if(initialQFrameFull == 1) {
		returnInt = clockNextPosiHandMove();

	} else {
		lastFramePos ++;
		lastFramePos = lastFramePos % numFrame;
	}	

	return returnInt;

}

int clockNextPosiHandMove() {
	int returnInt, foundFramePosi ;
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;

	int qFound = 0;
	int tempClockHand = clockHand;

	for(int i = tempClockHand; i < 64; i++) {
		// cout<<"in clockHandMove ~~~~~~~~~~~~~"<<"v: "<<i<<" refBit: "<<VreferenceBit[i] <<endl;
		if(VpresentBit[i] == 1) {
			if(VreferenceBit[i] == 1) {
				VreferenceBit[i] = 0;
			} else {
				foundFramePosi =  i;			
				break;
			}
			//VreferenceBit[i] = 0;
		}
		if(i == 63) {
			i = -1;
		} 
		if(i == clockHand -1) break;
		clockHandTick(1);
	}
	clockHandTick(1);
	// clockHandTick(1);

	returnInt = gMappedPosi(foundFramePosi);
	//cout<<"clockHand $$$$$$$$$$ returnInt: "<<returnInt<<endl;


	return returnInt;
}

void clockHandTick(int ticks) {
	clockHand = clockHand + ticks;
	clockHand = clockHand % 64;
}

void vRefBitclear() {
	// cout<<" vRefBitclear ******************************"<<endl;
	int *VreferenceBit = vmmStc.referenceBit;
	for(int i = 0; i < 64; i++) {
		VreferenceBit[i] = 0;
	}
}

int qVRefBitFull() {
	// cout<<"qVRefBitFull******************************"<<endl;
	int *VreferenceBit = vmmStc.referenceBit;
	int numRefOne = 0;
	for(int i = 0; i < 64;i++) {
		if(VreferenceBit[i] ==1) {
			numRefOne ++;
		}
	}
	if(numRefOne == numFrame) return 1;
	else return 0;
}













