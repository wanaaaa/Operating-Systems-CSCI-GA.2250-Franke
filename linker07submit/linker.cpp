#include <iostream>
#include <string>
#include <sstream>

#include <fstream> 
#include <vector>
#include <cstdlib>

using namespace std;

#include "header.h"

#include "parser.cpp"
#include "passOne.cpp"
#include "passTwo.cpp"
#include "warnMessages.cpp"
#include "parserTwo.cpp"


int main(int argc, char *argv[]) {

	fileName = argv[1];
	parser(argv[1]);
	parseTwo();
	passOne();
	passTwo();
	warnMessFun();

	writeFile(0);

	return 0;
}

