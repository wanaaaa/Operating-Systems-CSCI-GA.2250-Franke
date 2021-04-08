void readFileFun(char str[], int ii);

void parser(char fileName[], char rfile[]) {
	readFileFun(fileName, 0);
	readFileFun(rfile, 1);
}

void readFileFun(char str[], int intFile) {
	std::vector<char> stVec;

	if (FILE *fp = fopen(str, "r")) {

		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp)){
			stVec.insert(stVec.end(), buf, buf + len);
			//cout<<"In readFileFun: "<<endl;
		}

		fclose(fp);
	}

	char tempChar;
	string tempSt = "";
	int qPound = 0;
	for(int i = 0; i < stVec.size(); i ++) {
		tempChar = stVec[i];

		if (tempChar == '#') qPound = 1;
		if(tempChar ==' '|| tempChar=='\n') {

			if(qPound == 1 ) {
				if(tempChar == '\n') {
					if(intFile == 0) rawDataStc.commentsVec.push_back(tempSt);
					tempSt = "";
					qPound = 0;					
				}
			}
			else {
				if(tempChar == ' ') {
					if(intFile == 0)
						rawDataStc.RWoperVec.push_back(atoi(tempSt.c_str()));
					tempSt = "";
				}
				else if(tempChar == '\n'){
					if(intFile == 0)
						rawDataStc.vPageVec.push_back(atoi(tempSt.c_str()));
					else ranVec.push_back(atoi(tempSt.c_str()));
					tempSt = "";
				}

			}
		} else tempSt += tempChar;


		//cout<<"readFileFun "<<stVec[i]<<endl;
	}

	// for(int i = 0; i < ranVec.size(); i++) {
	// 	cout<<"readFileFun ranVec: "<<ranVec[i]<<endl;
	// }

}