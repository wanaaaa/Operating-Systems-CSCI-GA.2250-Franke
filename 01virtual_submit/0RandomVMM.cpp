int getRan();
void setFramePosi(int posi);

void randomVMM(int sI) {
	std::vector<int> RWVec = rawDataStc.RWoperVec; 
	std::vector<int> vPVec = rawDataStc.vPageVec;

	Event nEvt;
	nEvt.index = sI;
	nEvt.instruction[0] = RWVec[sI];
	nEvt.instruction[1] = vPVec[sI];

	int vmPosi = vPVec[sI];

	int *fMapArr = frameStc.frameMap;
	unsigned *fOrderArr = frameStc.frameOrder;
	int nfposi;// = findFramePosi();

	//statistics////////////////////////////////////////
	int *VpresentBit = vmmStc.presentBit;
	int *VreferenceBit = vmmStc.referenceBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	string *VpageOutBit = vmmStc.pageOutBit;
	///////////////////////////

	vMenInstrucArr[vmPosi] = nEvt.instruction[0];

	if(qMapped(vmPosi) != 1 ) {
		nfposi = getRan();

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
		//nfposi = findFramePosi();
		zeroSta.f = nfposi;
		//fMapArr[nfposi] = vmPosi;
		nEvt.statusVec.push_back(zeroSta);	
		

		Status mapStatus;
		mapStatus.pStatus = "MAP";
		mapStatus.v = vmPosi;
		mapStatus.f = nfposi;
		numMapping ++;
		fMapArr[nfposi] = vmPosi;
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

/////////////////////////////////////
int getRan() {
	int ranPos;
	if(!qFrameFull()) {
		ranPos = lastFramePos;
		lastFramePos ++;
	} else {
		if(ranI == 10002) ranI = 1;
		ranPos = ranVec[ranI] % numFrame;
		ranI ++;
	}

	return ranPos;
}