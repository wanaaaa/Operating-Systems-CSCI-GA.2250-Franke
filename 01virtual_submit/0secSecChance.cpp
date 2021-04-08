int findSecChanceFramePosi();
int nextNPosi() ;

void secSecChanceVMM(int sI)  {
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
			//frameRefZero();
			initialQFrameFull = 1;
			//cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}

		////////////////////////////
		nfposi = findSecChanceFramePosi();


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
int findSecChanceFramePosi() {
	int *fMapArr = frameStc.frameMap;
	// int *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	int returnInt = lastFramePos;

	if(initialQFrameFull == 1) {
		//cout<<"findSecChanceFramePosi 1111111111111111"<<endl;
		returnInt = nextNPosi();

	} else {
		lastFramePos ++;
		lastFramePos = lastFramePos % numFrame;
	}	

	return returnInt;

}

int nextNPosi() {

	lastFramePos = lastFramePos % numFrame;

	int *fMapArr = frameStc.frameMap;

	int returnInt, foundFramePosi ;
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;

	int qFound = 0;
	// int tempClockHand = clockHand;
	while(qFound == 0) {
		int vicV = fMapArr[lastFramePos];
		//cout<<"nextNPosi 22222222222222222222 vicV: "<<vicV<<" lastFramePos: "<<lastFramePos <<endl;
		if(VreferenceBit[vicV] == 1) {
			VreferenceBit[vicV] = 0;
			lastFramePos++;
			lastFramePos = lastFramePos % numFrame;
		} else qFound = 1;
	}

	returnInt = lastFramePos;
	lastFramePos ++;

	

	return returnInt;
}



// void clockHandTick(int ticks) {
// 	clockHand = clockHand + ticks;
// 	clockHand = clockHand % 64;
// }

// void vRefBitclear() {
// 	// cout<<" vRefBitclear ******************************"<<endl;
// 	int *VreferenceBit = vmmStc.referenceBit;
// 	for(int i = 0; i < 64; i++) {
// 		VreferenceBit[i] = 0;
// 	}
// }

// int qVRefBitFull() {
// 	// cout<<"qVRefBitFull******************************"<<endl;
// 	int *VreferenceBit = vmmStc.referenceBit;
// 	int numRefOne = 0;
// 	for(int i = 0; i < 64;i++) {
// 		if(VreferenceBit[i] ==1) {
// 			numRefOne ++;
// 		}
// 	}
// 	if(numRefOne == numFrame) return 1;
// 	else return 0;
// }













