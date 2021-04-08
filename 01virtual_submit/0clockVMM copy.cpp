int findClockFramePosi();
void setFramePosi(int posi);
int qFrameRefFull();
void frameRefZero();
// int qFrameHasMinus();

void clockVMM(int sI)  {
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
	int nfposi;// = findFramePosi();

	//statistics////////////////////////////////////////
	int *VpresentBit = vmmStc.presentBit;
	int *VreferenceBit = vmmStc.referenceBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	string *VpageOutBit = vmmStc.pageOutBit;
	///////////////////////////	


	vMenInstrucArr[vmPosi] = nEvt.instruction[0];

	if(qMapped(vmPosi) != 1 ) {
		// if(numMapping % numFrame == 0 && sI != 0) {
		if(qFrameRefFull() == 1) {
			frameRefZero();
			cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}
		////////////////////////////
		nfposi = findClockFramePosi();

		if(qFrameFull() == 1) {
			//cout<<"begin to unMap ======== "<<endl;
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
		
		//cout<<"fifoVMM test ===================="<< vmPosi<<endl;	
		Status zeroSta;
		zeroSta.pStatus = "ZERO";
		zeroSta.v = -1;
		//nfposi = findFramePosi();
		zeroSta.f = nfposi;
		//fMapArr[nfposi] = vmPosi;
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
		///////////////////////////////////////		

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
		int fPosi = gMappedPosi(vmPosi);
		fRefArr[fPosi] = 1;
		
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
	// int *fMapArr = frameStc.frameMap;
	// int *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	int returnInt = lastFramePos;

	if(fRefArr[lastFramePos] == 1 && qFrameFull() == 1){

		fRefArr[returnInt] = 0;
		lastFramePos++;
		lastFramePos = lastFramePos % numFrame;

		returnInt ++;
		returnInt = returnInt % numFrame;
		int wI=0;
		while(wI < numFrame ) {
			if(fRefArr[returnInt] == 1) {
				// cout<<"I am in while findSChanceFramePosi ~~~~~~~~~~~~~~~"<<endl;
				fRefArr[returnInt] =0;			
				returnInt ++;	
				returnInt = returnInt % numFrame;
			} else {
				break;
			}
			wI ++;	
		}


	} else {

		if(lastFramePos == numFrame -1) lastFramePos = 0;
		else lastFramePos ++;
	}


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
	// cout<<"frameRefZero Function !!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	int *fRefArr = frameStc.frameRef;
	for(int i = 0; i < numFrame; i++) {
		fRefArr[i] = 0;	
	}
}
