#include "TransportApplication.h"
#include "../PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"

#include <iostream>

using namespace std;

void exportPerStationCountComparison(string fileName, map<string, vector<int> > &mapPerStationCount1, map<string, vector<int> > &mapPerStationCount2, string strTitle) {
    float fSumInaccuracy = 0.0f;
    float fAccuracy = 0.0f;
    float fInaccuracy = 0.0f;
    ofstream outfile;

    outfile.open(fileName);
    outfile << strTitle << endl;

    for (map<string, vector<int> >::iterator it = mapPerStationCount1.begin(); it != mapPerStationCount1.end(); it++) {
        string strStationName = it->first;
        vector<int> vecOpal = it->second;

        map<string, vector<int> >::iterator itRoam = mapPerStationCount2.find(strStationName);
        if (itRoam != mapPerStationCount2.end()) {
            vector<int> vecRoam = itRoam->second;
            fAccuracy = float(vecRoam.size())/float(vecOpal.size());
            fInaccuracy = 1 - fAccuracy;
            outfile << strStationName << ", " << vecOpal.size() << ", " << itRoam->first << ", " << vecRoam.size() << ", " << fAccuracy << ", " << fInaccuracy << endl;
        }
        else {
            fAccuracy = 0;
            fInaccuracy = 1;
            outfile << strStationName << ", " << vecOpal.size() << ", " << strStationName << ", " << 0 << ", " << 0 << ", " << 1 << endl;
        }

        fSumInaccuracy += fInaccuracy;
    }

    outfile << ",,,,," << fSumInaccuracy/mapPerStationCount1.size() << endl;

    outfile.close();
}

void mergePerDayMap(map<string, vector<int> > &mapTo, map<string, vector<int> > &mapFrom) {
	for (map<string, vector<int> >::iterator itFrom = mapFrom.begin(); itFrom != mapFrom.end(); itFrom++) {
        string strStationName = itFrom->first;
        vector<int> vecFrom = itFrom->second;
        map<string, vector<int> >::iterator itTo = mapTo.find(strStationName);
        if (itTo != mapTo.end()) {
        	vector<int> &vecTo = itTo->second;
        	cout << "before vecto size: " << vecTo.size() << endl;
        	vecTo.reserve(vecTo.size() + vecFrom.size());

        	vecTo.insert(vecTo.end(), vecFrom.begin(), vecFrom.end());
        	        	cout << "after vecto size: " << vecTo.size() << endl;
        	// cout << "++++++++++++++++++++++++++++" << endl;
        }
        else {
        	vector<int> newVec(vecFrom);
	        mapTo.insert(pair<string, vector<int> >(strStationName, newVec));
	        // cout << "couldn't find: " << strStationName << " vecFrom size: "<< newVec.size() << endl;
        }
    }
}

void TransportApplication::compareOpalAndRoamPerStationPerDay(vector<string> strOpalInputCSVNames, vector<string> strRoamInputCSVNames, 
	vector<string> strOnOutputCSVNames, vector<string> strOffOutputCSVNames, 
	std::string strMergedOnOutputCSVName, std::string strMergedOffOutputCSVName) {
	int nOpalCSVCount = strOpalInputCSVNames.size();
	int nRoamCSVCount = strRoamInputCSVNames.size();
	int nOnOutputCSVCount = strOnOutputCSVNames.size();
	int nOffOutputCSVCount = strOffOutputCSVNames.size();

	if (nOpalCSVCount != nRoamCSVCount || nOpalCSVCount != nOnOutputCSVCount || nOpalCSVCount != nOffOutputCSVCount) {
		cout << "The csv name arrays given are not consistent: " << endl;
		cout << "nOpalCSVCount: " << nOpalCSVCount << " nRoamCSVCount: " << nRoamCSVCount << " nOnOutputCSVCount: " << nOnOutputCSVCount << " nOffOutputCSVCount: " << nOffOutputCSVCount << endl;
		return;
	}

	int nDays = nOpalCSVCount;
	map<string, vector<int> > mapTotalOnOpalPerStationCount;
	map<string, vector<int> > mapTotalOffOpalPerStationCount;
	map<string, vector<int> > mapTotalOnRoamPerStationCount;
	map<string, vector<int> > mapTotalOffRoamPerStationCount;
	
	for (int i = 0; i < nDays; i++) {
        string opalFile = strOpalInputCSVNames[i];
        string roamFile = strRoamInputCSVNames[i];
        string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);

        OpalTripAnalyser opal;
        opal.setFile(opalFile);
        opal.calculatePerStationCount();
        map<string, vector<int> > mapOnOpalPerStationCount = opal.getOnPerStationCount();
        map<string, vector<int> > mapOffOpalPerStationCount = opal.getOffPerStationCount();

        RoamResultAnalyser roam;
        roam.setFile(roamFile);
        roam.calculatePerStationCount();
        map<string, vector<int> > mapOnRoamPerStationCount = roam.getOnPerStationCount();
        map<string, vector<int> > mapOffRoamPerStationCount = roam.getOffPerStationCount();

		exportPerStationCountComparison(strOnOutputCSVNames[i], mapOnOpalPerStationCount, mapOnRoamPerStationCount, "Opal,Count,Roam,Count");
		exportPerStationCountComparison(strOffOutputCSVNames[i], mapOffOpalPerStationCount, mapOffRoamPerStationCount, "Opal,Count,Roam,Count");

        if (strOnOutputCSVNames != 0 && strOnOutputCSVNames.size() != 0) {
            // Merge per day station count into one file
    		mergePerDayMap(mapTotalOnOpalPerStationCount, mapOnOpalPerStationCount);
    		mergePerDayMap(mapTotalOffOpalPerStationCount, mapOffOpalPerStationCount);
    		mergePerDayMap(mapTotalOnRoamPerStationCount, mapOnRoamPerStationCount);
    		mergePerDayMap(mapTotalOffRoamPerStationCount, mapOffRoamPerStationCount);
        }
    }

    if (strOnOutputCSVNames != 0 && strOnOutputCSVNames.size() != 0) {
	   exportPerStationCountComparison(strMergedOnOutputCSVName, mapTotalOnOpalPerStationCount, mapTotalOnRoamPerStationCount, "Opal,Count,Roam,Count");
	   exportPerStationCountComparison(strMergedOffOutputCSVName, mapTotalOffOpalPerStationCount, mapTotalOffRoamPerStationCount, "Opal,Count,Roam,Count");
    }
}

void TransportApplication::compareOpalAndRoamPerStationPerDays(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strOnOutputCSVNames, std::vector<std::string> strOffOutputCSVNames) {

}
