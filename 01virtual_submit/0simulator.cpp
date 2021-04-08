//simulator(algorithm, options, numFrame, fileName );
void simulator(string algo, string options, string strFrameNum, string inputName) {
	initializeVMem();

	Event nEvt;
	int sI = 0;
	//////////////////////////////////////////////////////////////////////////
	string fileName = "out_"+inputName+"_"+strFrameNum+"_"+options;
	fileName = "./myOutFile/"+fileName;
	std::ofstream outfile (fileName.c_str());

	//////////////////////////////////////////////////////////////////////////
	// while (sI < 100) {
	while (sI < rawDataStc.RWoperVec.size()) {		
		
		if(algo == "f") fifoVMM(sI);
		else if (algo == "r") randomVMM(sI);
		else if (algo == "a") agingVMM(sI);
		else if (algo == "c") clockVMM(sI);
		else if (algo == "X") clockVMM_V(sI);
		// else if (algo == "s") secondChanceVMM(sI);
		else if (algo == "s") secSecChanceVMM(sI);
		else if (algo == "Y") agingVMM_V(sI);
		else if (algo == "N") nruVMM(sI);

		////////////////////////////////////////////////////
		nEvt = evtVec[sI];
		int pStaV;
		if(options.find("O") != string::npos ) {
			cout<<"==> inst: "<<nEvt.instruction[0]<<" "<<nEvt.instruction[1]<<endl;
			outfile<<"==> inst: "<<nEvt.instruction[0]<<" "<<nEvt.instruction[1]<<endl;

			Status pSta;
			for(int i = 0; i < nEvt.statusVec.size(); i++) {
				//cout<<"for nEvt.statusVec //////////"<<endl;
				pSta = nEvt.statusVec[i];
				if(pSta.v == -1) {
					cout<<sI<<": "<<left<<setw(5)<<setfill(' ')<< pSta.pStatus<<"   "<<" "<<"  "<<right<<setw(2)
					<<setfill(' ')<<pSta.f<<endl;		

					outfile<<sI<<": "<<left<<setw(5)<<setfill(' ')<< pSta.pStatus<<"   "<<" "<<"  "<<right<<setw(2)
					<<setfill(' ')<<pSta.f<<endl;		

				} else {				
					cout<<sI<<": "<<left<<setw(5)<<setfill(' ') <<pSta.pStatus<<"  "<<right<<setw(2)<<setfill(' ') <<pSta.v<<"  "
					<<right<<setw(2)<<setfill(' ')<<pSta.f<<endl;

					outfile<<sI<<": "<<left<<setw(5)<<setfill(' ') <<pSta.pStatus<<"  "<<right<<setw(2)<<setfill(' ') <<pSta.v<<"  "
					<<right<<setw(2)<<setfill(' ')<<pSta.f<<endl;					
				}
			}
		}


		int vmPosi = nEvt.instruction[1];
		// for(int i = 0; i < 64; i++) {
		// 	// cout<<i<<":"<<vmmStc.presentBit[i]<<":"<<vmmStc.referenceBit[i]<<vmmStc.modifiedBit[i]<<" ";
		// 	// if(vmmStc.referenceBit[i] != -9)
		// 	cout<<i<<":"<<vmmStc.secRefBit[i]<<vmmStc.modifiedBit[i]<<" ";
		// 	// if(vmmStc.presentBit[i]==1)
		// 	// cout<<i<<":"<<vmmStc.referenceBit[i]<<";"<<vmmStc.presentBit[i]<<"/"<<vmmStc.modifiedBit[i] <<" ";
		// 	// cout<<i<<":"<<vmmStc.referenceBit[i]<<"/"<<vmmStc.modifiedBit[i] <<" ";

		// }
		// cout<<endl;
		

		//cout<<"current VMVec instruction: "<<vmPosi<<":"<<vMenInstrucArr[vmPosi]<<"--"<<endl;
		// for(int i = 0; i < numFrame; i++) cout<<frameStc.frameMap[i]<<" "; cout<<" || ";
		// for(int i = 0; i < numFrame; i++) cout<<frameStc.frameOrder[i]<<" ";cout<<endl;
		// for(int i = 0; i < numFrame; i++) cout<<frameStc.frameRef[i]<<" "; 
		// cout<<" clockHand: "<<clockHand;
		// for(int i = 0; i < 64; i++) cout<< i<<":"<<vmmStc.agingBits[i]<<" ";
		// cout<<endl;
		////////////////////////////////////////////////////////////////////////

		sI ++;
	}

	//cout<<"last V"<<endl;
	if(options.find("P") != string::npos ) {
		int *VrefBits = vmmStc.referenceBit;
		string *VmodiBits = vmmStc.modifiedBit;
		string refStr; string modiStr;

		for(int i = 0; i < 64; i++ ) {
			if(VrefBits[i] == -9) cout<<"*"<<" ";
			else {
				if(VrefBits[i] == 1) refStr = "R";
				else refStr = "-";

				if(VmodiBits[i] == "m") modiStr = "M";
				else modiStr = "-";

				cout<<refStr<<modiStr<<" ";

				outfile<<refStr<<modiStr<<" ";
			}
		}
		cout<<endl;
		outfile<<endl;
	}

	// cout<<"last Frame"<<endl;
	if(options.find("F") != string::npos ){
		for(int i = 0; i < numFrame; i++) {
			cout<<frameStc.frameMap[i]<<" ";	
			outfile<<frameStc.frameMap[i]<<" ";
		} 
		cout<<endl;
		outfile<<endl;
	}
	// unsigned long long total = sI+numUnMap*400+numMap*400+numIn*3000+numOut*3000+(numMap-numIn)*150;
	if(options.find("S") != string::npos ) {
		unsigned long total = sI+numUnMap*400+numMap*400+numIn*3000+numOut*3000+(numMap-numIn)*150;
		
		printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %lu\n",
		sI, numUnMap, numMap, numIn, numOut, numMap-numIn, total);	

		// cout<<"SUM "<<sI<<" U="<<numUnMap<<" M="<<numMap<<" I="<<numIn<<" O="<<numOut
		// <<" Z="<<numMap-numIn<<" ===> "<<total<<endl;

		outfile<<"SUM "<<sI<<" U="<<numUnMap<<" M="<<numMap<<" I="<<numIn<<" O="<<numOut
		<<" Z="<<numMap-numIn<<" ===> "<<total<<endl;

	}

	outfile.close();
	//cout<<"aaaaaaaaaaaaa find   "<<options.find("t")<<"   "<<string::npos <<endl;
}
