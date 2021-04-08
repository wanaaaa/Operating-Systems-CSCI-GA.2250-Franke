int findFramePosi();
int findFramePosi();
void setFramePosi(int posi);

void fifoVMM(int sI) {
	std::vector<int> RWVec = rawDataStc.RWoperVec; 
	std::vector<int> vPVec = rawDataStc.vPageVec;

	Event nEvt;
	nEvt.index = sI;
	nEvt.instruction[0] = RWVec[sI];
	nEvt.instruction[1] = vPVec[sI];

	int vmPosi = vPVec[sI];

	int *fMapArr = frameStc.frameMap;
	unsigned *fOrderArr = frameStc.frameOrder;
	int nfposi;

	//statistics////////////////////////////////////////
	int *VpresentBit = vmmStc.presentBit;
	int *VreferenceBit = vmmStc.referenceBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	string *VpageOutBit = vmmStc.pageOutBit;
	///////////////////////////	

	vMenInstrucArr[vmPosi] = nEvt.instruction[0];

	if(qMapped(vmPosi) != 1 ) {
		nfposi = findFramePosi();

		if(qFrameFull() == 1) {

			Status unMapSta;
			unMapSta.pStatus = "UNMAP";
			unMapSta.v = fMapArr[nfposi];
			unMapSta.f = nfposi;
			nEvt.statusVec.push_back(unMapSta);

			//statistics///////////////////
			numUnMap++;

			if(numMapping >= numFrame ) {
				if(qVwriteOut(unMapSta.v) == 1) {
					numOut++;
				}
			}

			VreferenceBit[unMapSta.v] = -9;
			VmodifiedBit[unMapSta.v] = "-";
			///////////////
			///////////////

		} 			
		
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
		
		numMapping ++;
		setFramePosi(nfposi);
		nEvt.statusVec.push_back(mapStatus);	

		//Statistics////////////////////
		numMap ++;

		VreferenceBit[mapStatus.v] = 1;
		if(RWVec[sI] ==1) VmodifiedBit[mapStatus.v] = "m";

		if(numMapping >= numFrame ) {
			if(qWriteIn(mapStatus.v, sI) == 1) {
				numIn ++;
			}
		}
		////////////////////////			

	} 
	else {
		//statistics
		if(RWVec[sI] == 1) {
			VmodifiedBit[vmPosi] = "m";
		}
		//////////////////////////////
	};

	evtVec.push_back(nEvt);
} 

///////////////////////////////////////
int findFramePosi() {
	// int *fMapArr = frameStc.frameMap;
	// int *fOrderArr = frameStc.frameOrder;
	int returnInt = lastFramePos;
	if(lastFramePos == numFrame -1) lastFramePos = 0;
	else lastFramePos ++;

	return returnInt;
}

void setFramePosi(int posi) {
	int *fMapArr = frameStc.frameMap;
	unsigned *fOrderArr = frameStc.frameOrder;

	if(fOrderArr[posi] == 100) {
		fOrderArr[posi] = maxFOrder;
		maxFOrder ++;
	} 
	else {
		for(int i = 0; i < numFrame; i++) {
			if(fOrderArr[i] == numFrame-1) {
				fOrderArr[i] = 0;
			} else {
				fOrderArr[i] ++;
			}
		}
	}
}


















































