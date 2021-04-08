int findClockFramePosi();
void setFramePosi(int posi);
int qFrameRefFull();
void frameRefZero();
// int qFrameHasMinus();
int nextHour();

void clockVMM(int sI)  {
	//cout<<"asdfasdfasdfasdfasdfasdfasdfasdfasf"<<endl;
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
			//frameRefZero();
			initialQFrameFull = 1;
			//cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}

		////////////////////////////
		nfposi = findSecChanceFramePosi();


		if(qFrameFull() == 1) {

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
int findClockFramePosi() {
	int *fMapArr = frameStc.frameMap;
	// int *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	int returnInt = clockHand;

	if(initialQFrameFull == 1) {
		returnInt = nextHour();

	} else {
		clockHand++;
		clockHand= clockHand% numFrame;
	}	

	return returnInt;

}

int nextHour() {

	clockHand= clockHand% numFrame;

	int *fMapArr = frameStc.frameMap;

	int returnInt, foundFramePosi ;
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;

	int qFound = 0;
	// int tempClockHand = clockHand;
	while(qFound == 0) {
		int vicV = fMapArr[clockHand];

		if(VreferenceBit[vicV] == 1) {
			VreferenceBit[vicV] = 0;
			clockHand++;
			clockHand= clockHand% numFrame;
		} else qFound = 1;
	}

	returnInt = clockHand;
	clockHand++;

	

	return returnInt;
}

int qFrameRefFull() {
	int *fRefArr = frameStc.frameRef;
	//int reutrnInt;
	for(int i = 0; i < numFrame; i++) {
		// if(fRefArr[i] != 1) return 0;
		if(fRefArr[i] != 1) return 0;
	}
	return 1;
}


void frameRefZero() {
	int *fRefArr = frameStc.frameRef;
	for(int i = 0; i < numFrame; i++) {
		fRefArr[i] = 0;	
	}
}
