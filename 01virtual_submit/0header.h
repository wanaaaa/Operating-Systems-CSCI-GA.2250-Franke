int numFrame;
int maxFOrder = 0;
int lastFramePos = 0;
int numMapping = 0;
int ranI = 1;
int clockHand = 0;
int initialQFrameFull = 0;
int int10thPageNRU = 0;

//Statistics
int numUnMap = 0; int numMap = 0; int numIn = 0;int numOut = 0;int numZero = 0 ;

////////////
int vMenInstrucArr[64];
std::vector<int> ranVec;

struct FrameStc{
	int frameMap[32];
	unsigned frameOrder[32];
	int frameRef[32];
	unsigned agingBits[32];
	int frameID[32];
} frameStc;

struct VMMStc{
	int presentBit[64];
	int referenceBit[64];
	int secRefBit[64];
	string modifiedBit[64];
	string pageOutBit[64];
	unsigned agingBits[64];
	int NRUstate[64];
} vmmStc;

struct RawData {
	std::vector<string> commentsVec;
	std::vector<int> RWoperVec;
	std::vector<int> vPageVec;
} rawDataStc;
/////////////////////////////////////
class Status{
public:
	string pStatus;
	int v;
	int f;	
};
////////////////////////
class Event{
public:
	int index;
	int instruction[2];
	std::vector<Status> statusVec;
	std::vector<string> vVec;
	int vArr[10];
	int fArr[10];
};
/////////////////////////////////////
std::vector<Event> evtVec;
/////////////////////////////////////
void initializeVMem() {
	for(int i = 0; i < 64; i++) {
		vMenInstrucArr[i] = -1;
		if(i < numFrame) {
			frameStc.frameMap[i] = -1;
			frameStc.frameOrder[i] = 100;
			frameStc.frameRef[i] = -1;
			frameStc.agingBits[i] = 0;
		} 
		vmmStc.referenceBit[i] = -9;
		vmmStc.secRefBit[i] = -9;
		vmmStc.presentBit[i] = -9;
		vmmStc.modifiedBit[i] = "-";
		vmmStc.pageOutBit[i] = "-";
		vmmStc.agingBits[i] = 0;
	}
}
////////////////////////
int qMapped(int qInt) {
	for(int i = 0; i < numFrame;i++) {
		if(frameStc.frameMap[i] == qInt) return 1;
		// cout<<"qMapped ~~~~ qInt: "<<qInt<<
	}
	// ansInt = 0;
	return 0;	
}

int qFrameFull() {
	for(int i =0; i < numFrame; i++) {
		if(frameStc.frameMap[i] == -1) return 0;
	}
	return 1;
}

int gMappedPosi(int vPosi) {
	int returnInt = -1;
	for(int i = 0; i < numFrame; i ++) {
		if(frameStc.frameMap[i] == vPosi) returnInt = i;
	}

	return returnInt;
}

int qWriteOut(int vPage, int sI) {
	std::vector<int> RWVec = rawDataStc.RWoperVec; 
	std::vector<int> vPVec = rawDataStc.vPageVec;
	for(int i = sI -1; i >= 0; i--) {
		if(vPVec[i] == vPage ) {
			if(RWVec[i] == 1) return 1;
			else return 0;
		}
	}

	return 0;
}

int qWriteIn(int vPage, int sI) {
	std::vector<int> RWVec = rawDataStc.RWoperVec; 
	std::vector<int> vPVec = rawDataStc.vPageVec;
	for(int i = sI -1; i >= 0; i--) {
		if(vPVec[i] == vPage ) {
			if(RWVec[i] == 1) return 1;
		}
	}
	return 0;
}

int qVwriteOut(int vPage) {
	int *VreferenceBit = vmmStc.referenceBit;
	string *VmodifiedBit = vmmStc.modifiedBit;

	if(VreferenceBit[vPage] == 1 && VmodifiedBit[vPage] == "m") return 1;
	return 0;
}

int qVwriteOutSec(int vPage) {
	int *VreferenceBit = vmmStc.secRefBit;
	string *VmodifiedBit = vmmStc.modifiedBit;
	
	if(VreferenceBit[vPage] == 1 && VmodifiedBit[vPage] == "m") return 1;
	return 0;
}

















