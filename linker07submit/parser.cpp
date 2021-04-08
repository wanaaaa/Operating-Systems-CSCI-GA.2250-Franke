#include <iostream>
using namespace std;

void toStrVector(string str);

void parser(char fileName[]) {
	char charIndi;
	string str;

    FILE *file = fopen(fileName, "r");
    size_t n = 0;
    int c;

    if (file == NULL) printf("There is no file"); 
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    while ((c = fgetc(file)) != EOF) {
        charIndi = (char) c;

        str = str + charIndi;
    }
    
    toStrVector(str);
}


void toStrVector(string str) {

    string strArr[str.length()+1];
    string tempStr = "";
    int arrLength = 0;
    for (int i = 0; i < str.length(); i++) {
        
        if(str[i] == ' ' || str[i] == '\n') {

            if(tempStr != " " && tempStr != "") {
                strArr[arrLength] = tempStr;
                arrLength ++;
                tempStr = "";
            }

        }
        if (str[i] != '\n' ) tempStr += str[i];
        else {
            strArr[arrLength] = "lineChange";
            arrLength ++;
        }
    }
    ///////////////////////////////////////
    strArr[arrLength] = tempStr;
    arrLength ++;

    int nArrLength = 0;
    int wordLength;
    int empty = 0;
    

    string strEle, noSpaceStr;
    for (int i = 0; i < arrLength ; i++) {
        noSpaceStr = "";
        empty = 0;
        strEle = strArr[i];
        wordLength = strArr[i].length();

        for(int j = 0; j < wordLength; j++) {

            if (strEle[j] != ' ' && strEle[j] != '\n' && strEle[j] != '\t') {
                empty ++;
                noSpaceStr += strEle[j];
             }   
        }

        if (empty != 0) {
            rawStringVector.push_back(noSpaceStr);

            if(noSpaceStr != "lineChange") stringVecNoLineChange.push_back(noSpaceStr);
            nArrLength ++;
        }
    }

}

void writeFile(int qExit) {
    
    string strNum = "";
    int flag0 = 0;
    for(int i = 0; i < fileName.length(); i++) {
        if(fileName[i] == '-' || flag0 >= 1) flag0 ++;

        if(flag0 > 1) strNum += fileName[i];
    }

    string newName = "./outdir/out-"+strNum;

    std::ofstream outfile (newName.c_str());
    //////////////////////////////////////////////////////

    //////////Write out file
    if(qExit == 1) {
        outfile<<parseErrString<<endl;
        cout<<parseErrString<<endl;
        
        exit(0);
    }

    /////////////////////////
    for(int i = 0; i < headWarnMessVec.size(); i ++) {
        outfile<<headWarnMessVec[i]<<endl;
        cout<<headWarnMessVec[i]<<endl;
    }

    outfile << "Symbol Table" << std::endl;
    cout << "Symbol Table" << std::endl;
    for(int i = 0; i < passOneResultStc.length; i++) {
        if(passOneResultStc.symbols[i].length() != 0) {
        outfile << passOneResultStc.symbols[i] << "="<< passOneResultStc.finalAddress[i] <<passOneResultStc.errMess[i] <<endl;
        cout << passOneResultStc.symbols[i] << "="<< passOneResultStc.finalAddress[i] <<passOneResultStc.errMess[i] <<endl;
        //cout << psOnePrintSc.symbols[i] << "="<< psOnePrintSc.addresses[i] <<psOnePrintSc.errMessages[i] <<endl;
        }
    }

    outfile<<"\nMemory Map" << endl;
    cout<<"\nMemory Map" << endl;
    for(int i = 0; i < passTwoResStc.length; i++) {

        string stNum = make4charFromInt(passTwoResStc.nAddress[i]);
        outfile <<passTwoResStc.index[i]<<": "<< stNum <<passTwoResStc.errMess[i] <<endl;
        cout <<passTwoResStc.index[i]<<": "<< stNum <<passTwoResStc.errMess[i] <<endl;

        //outfile<< passTwoError.errMessages[i] << endl;
    }
    
    outfile<<"\n";
    for(int i = 0; i < warnMessStc.length; i++ ) {
        outfile<< "Warning: Module "<<warnMessStc.modules[i]<<": "<<warnMessStc.symbols[i]<<warnMessStc.errMess[i];
        cout<< "Warning: Module "<<warnMessStc.modules[i]<<": "<<warnMessStc.symbols[i]<<warnMessStc.errMess[i];
    }

    outfile.close();

}