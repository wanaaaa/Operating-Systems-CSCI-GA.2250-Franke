int findAging_VFramePosi() ;
int VagingNextFrame();
void setVAgingBit(int posiInt);


void agingVMM_V(int sI)  {
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
	int *VsecRefBit = vmmStc.secRefBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	string *VpageOutBit = vmmStc.pageOutBit;
	///////////////////////////		

	vMenInstrucArr[vmPosi] = nEvt.instruction[0];

	if(qMapped(vmPosi) != 1 ) {

		if(qFrameRefFull() == 1) {
			//frameRefZero();
			initialQFrameFull = 1;
			//cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}
 
		////////////////////////////
		nfposi = findAging_VFramePosi();


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
int findAging_VFramePosi() {
	// cout<<"1111111111111111111111111111111111 "<<endl;

	int *fRefArr = frameStc.frameRef;
	int returnInt = lastFramePos;

	if(initialQFrameFull == 1) {
		returnInt = VagingNextFrame();

	} else {
		lastFramePos ++;
		lastFramePos = lastFramePos % numFrame;
	}	

	return returnInt;

}

void setVAgingBit(int posiInt) {
	unsigned *VAgingBits = vmmStc.agingBits;
	int *VreferenceBit = vmmStc.referenceBit;
	for(int i = 0; i < 64; i++) {
		VAgingBits[i] = VAgingBits[i] >> 1;
		if(VreferenceBit[i] == 1) {
			VAgingBits[i] = VAgingBits[i] | 0x80000000;
			VreferenceBit[i] = 0;
		}
	}

	VAgingBits[posiInt] = 0;
}

int VagingNextFrame() {
	int returnFPosi;
	unsigned *VAgingBits = vmmStc.agingBits;
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;

	int miniPosi;
	unsigned minAging = 4294967295;

	int numVRef = 0;
	for(int i = 0; i < 64; i++) {
		if(VreferenceBit[i] == 1 && VpresentBit[i] == 1) {
			numVRef ++;
			if(numVRef == numFrame) break;
		} 
		else {
			if (VAgingBits[i] < minAging && VpresentBit[i] == 1) {
				minAging = VAgingBits[i];
				miniPosi = i;
			}
		}

	}

	minAging = 4294967295;
	if(numVRef == numFrame) {		
		for(int i = 0; i < 64; i++) {
			if(VreferenceBit[i] == 1 && VpresentBit[i] == 1) {
				if (VAgingBits[i] < minAging) {
					minAging = VAgingBits[i];
					miniPosi = i;
				}
			} 
		}
	}

	returnFPosi = gMappedPosi(miniPosi);
	setVAgingBit(miniPosi);
	return returnFPosi;/////////////
}
















