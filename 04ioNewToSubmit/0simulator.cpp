void simulator(string algo, string options) {	

	void (*sortIssue)(Event);
	if(algo.find("i") != string::npos) sortIssue = &sortIssueFIFO;
	else if(algo.find("j") != string::npos) sortIssue = &sortIssueSSTF;
	else if(algo.find("c") != string::npos) sortIssue = &sortIssueCSCAN;
	else if(algo.find("s") != string::npos) sortIssue = &sortIssueSCAN;
	else if(algo.find("f") != string::npos) sortIssue = &sortIssueSCAN;
	// else if(algo.find("f") != string::npos) sortIssue = &sortIssueFSCAN;
	

	int evtVecI;
	// while (simuTime <= 1928) {
	while (!allDone) {
		
		evtVecI = 0;
		while(evtVecI < evtVec.size()) {
			//rEvt = evtVec[evtVecI];
			
			if(evtVec[evtVecI].tStep == simuTime) {
			 	Event rEvt = evtVec[evtVecI];
				///////////////////////////////////////////////////////////////////////////	
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
				// cout<<endl;
				// showEvtInformation(rEvt, "");

			 	expiredEvtVec.push_back(rEvt);
				////////////////////////////////////////////////////////////////////////////
			 	evtVec.erase(evtVec.begin()+evtVecI);
				
				if(rEvt.operation == "add") {
					Event iEvt = makeNewIssue(rEvt);

					if(haveFutureFinish()) {

						if(algo == "f") {
							if(firstOrSecIssueVec == 1) {
								secIssueVec.push_back(iEvt);
							} else {
								issueVec.push_back(iEvt);
							}
						} else {
							issueVec.push_back(iEvt);												
						}
			
					} else {

						iEvt.tStep = simuTime;

						iEvt.preTrack = expiredIssueVec[0].currentTrack;
						expiredIssueVec.push_back(iEvt);
						////////////////////////////////////////////////
						//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
						//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
						// showEvtInformation(iEvt, "" );
						// if(goForward == 1) goForward = 0;
						// else goForward = 1;
						//////////////////////////////////////////////
						expiredEvtVec.push_back(iEvt);
						expiredIssueVec.erase(expiredIssueVec.begin());
						doIssueToFinish(iEvt);
					}
					
				} 
				else if(rEvt.operation == "finish") {
					if(algo == "f") {
						if(issueVec.size() == 0) {
							issueVec.swap(secIssueVec);
							goForward = 1;
						}
						
					}

					if(issueVec.size()) {
						//**********************************************	
						//showAllelementInVec(secIssueVec, "secIssueVec ||||");
						// showAllelementInVec(issueVec, "issueVec ====== ");

						(*sortIssue) (rEvt);

						//**********************************************
						firstOrSecIssueVec = 1;

						////////////////////////////////////////////////
						Event iEvt = issueVec[0];
						issueVec.erase(issueVec.begin());
						iEvt.tStep = simuTime;
						iEvt.preTrack = expiredIssueVec[0].currentTrack;
						expiredIssueVec.push_back(iEvt);

						///////////////////////////////////////////////
						//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
						//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
						// showEvtInformation(iEvt, "" );
						/////////////////////////////////
						expiredEvtVec.push_back(iEvt);
						expiredIssueVec.erase(expiredIssueVec.begin());
						doIssueToFinish(iEvt);						
					}

					if(evtVec.size() == 0) allDone = 1;
											
				}

				//Handle same time event
				if(evtVec.size() > 0 && evtVec[0].tStep == simuTime) {
					simuTime --;
				}
				
			}

			if(evtVec[0].tStep > simuTime) break;
			/////////////
			evtVecI++;
		}// end of while evtVecI 

		simuTime ++;
	}//end of while simuTime	

// 
}//End of simulator ////

