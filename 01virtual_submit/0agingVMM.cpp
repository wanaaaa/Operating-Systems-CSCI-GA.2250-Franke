int findAgingFramePosi() ;
int findMinAgingPosi();
void setAgingBit(int posiInt);
int qFOrderAllZero() ;
int gNumRefBit();

void agingVMM(int sI) {
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
		nfposi = findAgingFramePosi();

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
		numMapping++;
		
		// setFramePosi(nfposi);
		nEvt.statusVec.push_back(mapStatus);	

		fOrderArr[nfposi] = 0;

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
		//fOrderArr[fPosi] = fOrderArr[fPosi] >> 1;
		fRefArr[fPosi] = 1;

		//statistics
		if(RWVec[sI] == 1) {
			VmodifiedBit[vmPosi] = "m";
		}
		//////////////////////////////
 
	};

	evtVec.push_back(nEvt);
} 

/////////////////////////////////////
int findAgingFramePosi() {
	int agingPosi;
	if(!qFrameFull()) {
		agingPosi = lastFramePos;
		lastFramePos ++;
	} else {
		agingPosi = findMinAgingPosi();
		//cout<<"before setAgingBit ~~~~~~~~~~~~~~~~~~~~~"<<endl;
		setAgingBit(agingPosi);

	}

	return agingPosi;
}

int findMinAgingPosi() {
	int returnPosi;
	unsigned *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;
	unsigned minAging = 4294967295;
	int minPosi = 0;
	for(int i = 0; i < numFrame; i ++) {
		if(qFOrderAllZero() == 1) {
			minPosi = 0;
			lastFramePos ++;
		}
		else if(minAging > fOrderArr[i] ) {

			if(gNumRefBit() == numFrame) {
				minAging = fOrderArr[i];
				minPosi = i;
			}
			else  {
				if( fOrderArr[i] == 0 || fRefArr[i] == 1) {}
				else {
					minAging = fOrderArr[i];
					minPosi = i;
					
				}
				
			}			

		}
	}

	fOrderArr[minPosi] = 0;
	return minPosi;
}

void setAgingBit(int posiInt) {
	unsigned *fOrderArr = frameStc.frameOrder;
	int *fRefArr = frameStc.frameRef;

	for(int i = 0; i < numFrame; i++) {
		fOrderArr[i] = fOrderArr[i] >> 1;
		if(fRefArr[i] == 1) {
			//fOrderArr[i] = fOrderArr[i] | 0x80000000;
			fOrderArr[i] = fOrderArr[i] | 0x80000000;
			fRefArr[i] = 0;
		}

		if(fOrderArr[i] == 0) fOrderArr[i] = fOrderArr[i] | 0x80000000;

		//cout<<"setAgingBit fOrderArr[i]; "<<i<<" : "<<fOrderArr[i]<<endl;
	}

}

int qFOrderAllZero() {
	int numZero = 0;
	unsigned *fOrderArr = frameStc.frameOrder;
	for(int i = 0; i < numFrame; i++) {
		if(fOrderArr[i] != 0) numZero ++;
	}	

	if(numZero == 0) return 1;
	else return 0;
}

int gNumRefBit() {
	int *fRefArr = frameStc.frameRef;
	int returnInt = 0;
	for(int i = 0; i < numFrame; i++) {
		if(fRefArr[i] == 1) returnInt ++;
	}
	return returnInt;
}

void setRefBitZero() {
	int *fRefArr = frameStc.frameRef;
	for(int i = 0; i < numFrame; i++) {
		fRefArr[i] = 0;
	}
}





























































