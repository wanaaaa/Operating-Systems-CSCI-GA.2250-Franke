int findNRUFramePosi();
int NRUNextFrame();
int ranVPosiFun();
int gRanV(int num);
void referBitZero() ;

void nruVMM(int sI)  {

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
			initialQFrameFull = 1;
			//cout<<"frameRefZero sI % numFrame = 0"<<endl;
		}
 
		////////////////////////////
		nfposi = findNRUFramePosi();


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
int findNRUFramePosi() {
	// cout<<"1111111111111111111111111111111111 "<<endl;
	int *fRefArr = frameStc.frameRef;
	int returnInt = lastFramePos;

	if(initialQFrameFull == 1) {
		//cout<<"findNRUFramePosi ////////////////// 10th page; "<<int10thPageNRU<<endl;

		returnInt = NRUNextFrame();
	} else {
		lastFramePos ++;
		lastFramePos = lastFramePos % numFrame;
	}	

	return returnInt;
}


int NRUNextFrame() {
	int returnFPosi;
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;
	string *VmodifiedBit = vmmStc.modifiedBit;

	//std::vector<int> selectPoolVec = gSelectPoolFun();
	
	// int miniPosi;
	int ranVPosi = ranVPosiFun();

	returnFPosi = gMappedPosi(ranVPosi);
	// setVAgingBit(miniPosi);
	return returnFPosi;/////////////
}

int ranVPosiFun() {
	std::vector<int> returnVec;

	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;
	string *VmodifiedBit = vmmStc.modifiedBit;

	std::vector<int> classZeroVec, classOneVec, classTwoVec, classThreeVec;
	for(int i = 0; i < 64; i++) {
		if(VreferenceBit[i] == 0 && VmodifiedBit[i] == "-" && VpresentBit[i] == 1) classZeroVec.push_back(i);
		else if (VreferenceBit[i]==0 && VmodifiedBit[i]=="m"&& VpresentBit[i] == 1) classOneVec.push_back(i);
		else if (VreferenceBit[i]==1 && VmodifiedBit[i]=="-"&& VpresentBit[i] == 1) classTwoVec.push_back(i);
		else if (VreferenceBit[i]==1 && VmodifiedBit[i]=="m"&& VpresentBit[i] == 1) classThreeVec.push_back(i);

		// if(VpresentBit[i] == 1)
		// cout<<"gSelectPoolFun "<<i<<" referBit: "<<VreferenceBit[i]<<" VmodifiedBit; "
		// <<VmodifiedBit[i] <<endl;
	}

	if(classZeroVec.size() != 0) {
		returnVec = classZeroVec;
		cout<<"classZeroVec selected sixe "<< classZeroVec.size()<<endl;
	}
	else if(classOneVec.size() != 0){
		returnVec = classOneVec;
		cout<<"classOneVec selected "<<endl;
	} 
	else if (classTwoVec.size() != 0) {
		returnVec = classTwoVec;
		cout<<"classTwoVec selected "<<endl;
	} 
	else if(classThreeVec.size() != 0) {
		returnVec = classThreeVec;
		cout<<"classThree selected "<<endl;
	} 

	int ranVI = gRanV(returnVec.size());
	int ranVPosi = returnVec[ranVI];

	if(int10thPageNRU % 10 == 9) {
		referBitZero();
		int10thPageNRU = 0;
	} else int10thPageNRU ++;


	return ranVPosi;
}

int gRanV(int vSize) {
	int ranNum;
	ranNum = ranVec[ranI] % vSize; 
	
	ranI ++;
	if (ranI == 10001) ranI = 1;
	return ranNum;
}

void referBitZero() {
	int *VreferenceBit = vmmStc.referenceBit;
	int *VpresentBit = vmmStc.presentBit;
	for(int i = 0; i < 63; i++) {
		if(VreferenceBit[i] == 1 && VpresentBit[i] == 1) {
			VreferenceBit[i] = 0;
		}
	}
}











