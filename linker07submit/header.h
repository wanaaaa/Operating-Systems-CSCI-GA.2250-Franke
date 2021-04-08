#include <iostream>
using namespace std;

int offSetStart, offSetAt ;

string parseErrString;
string fileName;
///////////////////////////////////////
std::vector<string> rawStringVector;
std::vector<string> stringVecNoLineChange;
std::vector<string> headWarnMessVec;
////////////////////////////////////////////////

struct AlldataStru{
	int length;
	string allWords[600];
	int qSymbols[600];
	int qRelaAddress[600];
	int qDefineList[600];

	int qUsedSyms[600];
	int qActullyUsedSyms[600];
	int qUseList[600];

	int qOpTypes[600];
	int qOpCodes[600];
	int qProgramText[600];
	string errMess[600];
	int moduleCycles[600];

	int qUsedNotActuUsed[600];

} allInfoStru;

struct PassOneResult {
	int length;
	string symbols[100];
	int finalAddress[100];
	string errMess[100];
} passOneResultStc, psOneResulTemStc;

struct PassTwoResult{
	int length;
	string opTypes[100];
	int nAddress[100];
	string errMess[100];
	string index[100];
	int modules[100];
} passTwoResStc;

struct WarnMessStc {
	int length;
	int modules[100];
	string symbols[100];
	string errMess[100];

} warnMessStc;

//////////////////////////////////////////////////////////////


int isDigit(string str) {
	int isDigit = 0;
	for(int i = 0; i < str.length(); i++) {
		if(isdigit(str[i]) == 0) isDigit ++;
		//cout<<" isDigit "<< str[i] << "//" << isdigit(str[i]) << endl;
	}

	if (isDigit == 0) return 1;
	else return 0;
}

string make4chars(string str) {
	string returnSt;
	if(str.length() == 1) returnSt = str+ "   ";
	if(str.length() == 2) returnSt = str+"  ";
	if(str.length() == 3) returnSt = str+" ";
	if(str.length() == 4) returnSt = str;
	return returnSt;
}

string make3chars(string str) {
	string returnSt;
	if(str.length() == 1) returnSt = "00"+str;
	if(str.length() == 2) returnSt = "0"+str;
	if(str.length() == 3) returnSt = str;

	return returnSt;
}

string make4charFromInt(int num) {	
	string stReturn;
	ostringstream convert;

	if(num <10) convert <<"000"<< num;
	else if(num < 100) convert<<"00"<< num;
	else if(num < 1000) convert << "0" << num;
	else convert << num;

	stReturn = convert.str();

	return 	stReturn;
}