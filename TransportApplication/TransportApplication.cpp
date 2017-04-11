#include "TransportApplication.h"
#include "../PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"
#include "../PerStopDataAnalyser/PerStopDataAnalyser.h"
#include "../PrePuncProcessor/PrePuncProcessor.h"

#include <iostream>
#include <sstream>

using namespace std;

// void exportPerStationCountComparison(string fileName, map<string, vector<int> > &mapPerStationCount1, map<string, vector<int> > &mapPerStationCount2, string strTitle) {
//     float fSumInaccuracy = 0.0f;
//     float fAccuracy = 0.0f;
//     float fInaccuracy = 0.0f;
//     ofstream outfile;

//     outfile.open(fileName);
//     outfile << strTitle << endl;

//     for (map<string, vector<int> >::iterator it = mapPerStationCount1.begin(); it != mapPerStationCount1.end(); it++) {
//         string strStationName = it->first;
//         vector<int> vecOpal = it->second;

//         map<string, vector<int> >::iterator itRoam = mapPerStationCount2.find(strStationName);
//         if (itRoam != mapPerStationCount2.end()) {
//             vector<int> vecRoam = itRoam->second;
//             fAccuracy = float(vecRoam.size())/float(vecOpal.size());
//             fInaccuracy = 1 - fAccuracy;
//             outfile << strStationName << ", " << vecOpal.size() << ", " << itRoam->first << ", " << vecRoam.size() << ", " << fAccuracy << ", " << fInaccuracy << endl;
//         }
//         else {
//             fAccuracy = 0;
//             fInaccuracy = 1;
//             outfile << strStationName << ", " << vecOpal.size() << ", " << strStationName << ", " << 0 << ", " << 0 << ", " << 1 << endl;
//         }

//         fSumInaccuracy += fInaccuracy;
//     }

//     outfile << ",,,,," << fSumInaccuracy/mapPerStationCount1.size() << endl;

//     outfile.close();
// }

void exportCountComparison(string fileName, map<string, vector<int> > &map1, map<string, vector<int> > &map2, string strTitle) {
    float fSumInaccuracy = 0.0f;
    float fAccuracy = 0.0f;
    float fInaccuracy = 0.0f;
    ofstream outfile;

    outfile.open(fileName);
    outfile << strTitle << endl;

    for (map<string, vector<int> >::iterator it = map1.begin(); it != map1.end(); it++) {
        string strKey = it->first;
        vector<int> vec1 = it->second;

        map<string, vector<int> >::iterator itRoam = map2.find(strKey);
        if (itRoam != map2.end()) {
            vector<int> vec2 = itRoam->second;
            fAccuracy = float(vec2.size())/float(vec1.size());
            fInaccuracy = 1 - fAccuracy;
            outfile << strKey << ", " << vec1.size() << ", " << itRoam->first << ", " << vec2.size() << ", " << fAccuracy << ", " << fInaccuracy << endl;
        }
        else {
            fAccuracy = 0;
            fInaccuracy = 1;
            outfile << strKey << ", " << vec1.size() << ", " << strKey << ", " << 0 << ", " << 0 << ", " << 1 << endl;
        }

        fSumInaccuracy += fInaccuracy;
    }

    outfile << ",,,,," << fSumInaccuracy/map1.size() << endl;

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

// void TransportApplication::compareOpalAndRoamPerStationPerDay(vector<string> strOpalInputCSVNames, vector<string> strRoamInputCSVNames, 
// 	vector<string> strOnOutputCSVNames, vector<string> strOffOutputCSVNames, 
// 	std::string strMergedOnOutputCSVName, std::string strMergedOffOutputCSVName) {
// 	int nOpalCSVCount = strOpalInputCSVNames.size();
// 	int nRoamCSVCount = strRoamInputCSVNames.size();
// 	int nOnOutputCSVCount = strOnOutputCSVNames.size();
// 	int nOffOutputCSVCount = strOffOutputCSVNames.size();

// 	if (nOpalCSVCount != nRoamCSVCount || nOpalCSVCount != nOnOutputCSVCount || nOpalCSVCount != nOffOutputCSVCount) {
// 		cout << "The csv name arrays given are not consistent: " << endl;
// 		cout << "nOpalCSVCount: " << nOpalCSVCount << " nRoamCSVCount: " << nRoamCSVCount << " nOnOutputCSVCount: " << nOnOutputCSVCount << " nOffOutputCSVCount: " << nOffOutputCSVCount << endl;
// 		return;
// 	}

// 	int nDays = nOpalCSVCount;
// 	map<string, vector<int> > mapTotalOnOpalPerStationCount;
// 	map<string, vector<int> > mapTotalOffOpalPerStationCount;
// 	map<string, vector<int> > mapTotalOnRoamPerStationCount;
// 	map<string, vector<int> > mapTotalOffRoamPerStationCount;
	
// 	for (int i = 0; i < nDays; i++) {
//         string opalFile = strOpalInputCSVNames[i];
//         string roamFile = strRoamInputCSVNames[i];
//         string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);

//         OpalTripAnalyser opal;
//         opal.setFile(opalFile);
//         opal.calculatePerStationCount();
//         map<string, vector<int> > mapOnOpalPerStationCount = opal.getOnPerStationCount();
//         map<string, vector<int> > mapOffOpalPerStationCount = opal.getOffPerStationCount();

//         RoamResultAnalyser roam;
//         roam.setFile(roamFile);
//         roam.calculatePerStationCount();
//         map<string, vector<int> > mapOnRoamPerStationCount = roam.getOnPerStationCount();
//         map<string, vector<int> > mapOffRoamPerStationCount = roam.getOffPerStationCount();

// 		exportPerStationCountComparison(strOnOutputCSVNames[i], mapOnOpalPerStationCount, mapOnRoamPerStationCount, "Opal,Count,Roam,Count");
// 		exportPerStationCountComparison(strOffOutputCSVNames[i], mapOffOpalPerStationCount, mapOffRoamPerStationCount, "Opal,Count,Roam,Count");

//         if (strOnOutputCSVNames != 0 && strOnOutputCSVNames.size() != 0) {
//             // Merge per day station count into one file
//     		mergePerDayMap(mapTotalOnOpalPerStationCount, mapOnOpalPerStationCount);
//     		mergePerDayMap(mapTotalOffOpalPerStationCount, mapOffOpalPerStationCount);
//     		mergePerDayMap(mapTotalOnRoamPerStationCount, mapOnRoamPerStationCount);
//     		mergePerDayMap(mapTotalOffRoamPerStationCount, mapOffRoamPerStationCount);
//         }
//     }

//     if (strOnOutputCSVNames != 0 && strOnOutputCSVNames.size() != 0) {
// 	   exportPerStationCountComparison(strMergedOnOutputCSVName, mapTotalOnOpalPerStationCount, mapTotalOnRoamPerStationCount, "Opal,Count,Roam,Count");
// 	   exportPerStationCountComparison(strMergedOffOutputCSVName, mapTotalOffOpalPerStationCount, mapTotalOffRoamPerStationCount, "Opal,Count,Roam,Count");
//     }
// }

void TransportApplication::compareOpalAndRoamPerStationPerDay(vector<string> strOpalInputCSVNames, vector<string> strRoamInputCSVNames, 
    string strOnOutputCSVName, string strOffOutputCSVName) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nRoamCSVCount = strRoamInputCSVNames.size();

    if (nOpalCSVCount != nRoamCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nRoamCSVCount: " << nRoamCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.calculatePerStationCount();
    map<string, vector<int> > mapOnOpalPerStationCount = opal.getOnPerStationCount();
    map<string, vector<int> > mapOffOpalPerStationCount = opal.getOffPerStationCount();

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculatePerStationCount();
    map<string, vector<int> > mapOnRoamPerStationCount = roam.getOnPerStationCount();
    map<string, vector<int> > mapOffRoamPerStationCount = roam.getOffPerStationCount();

    exportCountComparison(strOnOutputCSVName, mapOnOpalPerStationCount, mapOnRoamPerStationCount, "Opal,Count,Roam,Count");
    exportCountComparison(strOffOutputCSVName, mapOffOpalPerStationCount, mapOffRoamPerStationCount, "Opal,Count,Roam,Count");
}

void TransportApplication::compareOpalAndRoamPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, 
    std::string strOutputCSVName) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nRoamCSVCount = strRoamInputCSVNames.size();
    if (nOpalCSVCount != nRoamCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nRoamCSVCount: " << nRoamCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.calculatePerODCount();
    map<string, vector<int> > mapOpalPerODCount = opal.getPerODCount();

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculatePerODCount();
    map<string, vector<int> > mapRoamPerODCount = roam.getPerODCount();

    exportCountComparison(strOutputCSVName, mapOpalPerODCount, mapRoamPerODCount, "Opal,Count,Roam,Count");
}

map<string, vector<string> > TransportApplication::importAllStationLines(std::string strAllLinesFileName) {
    ifstream infile;
    infile.open(strAllLinesFileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << strAllLinesFileName << " couldn't be opened." << endl;
        return map<string, vector<string>>();
    }

    string line = "";
    string value = "";

    map<string, vector<string> > mapAllStationLines;
    string strLineName = "";

    int r = 1; // Avoid first row 
    struct tm tm = {};
    getline(infile, line);
    while (getline(infile, line)) {
        int c = 0;
        stringstream ss(line);

        string strCol1 = "";
        string strCol2 = "";
        string strCol3 = "";
        string strTypeName = "";
        string strStationName = "";

        getline(ss, strCol1, ',');
        getline(ss, strCol2, ',');
        getline(ss, strCol3, ',');

        if (strCol1 != "") {
            strLineName = strCol1;
            continue;
        }
        else if (strCol3 == "") {
            continue;
        }
        else {
            strStationName = strCol3;
            map<string, vector<string> >::iterator it = mapAllStationLines.find(strStationName);
            if (it != mapAllStationLines.end()) {
                vector<string> &vec = it->second;
                vec.push_back(strLineName);
                // cout << "found " << strStationName << " " << vec.size() << endl;
            }
            else {
                vector<string> vec;
                vec.push_back(strLineName);
                mapAllStationLines.insert(pair<string, vector<string> >(strStationName, vec));
                // cout << "not found " << strStationName << endl;
            }
        }
    }

    return mapAllStationLines;
}

void TransportApplication::compareOpalAndRoamPerLinePerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, 
    std::string strOutputCSVName, std::map<std::string, vector<std::string> > mapStationLines) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nRoamCSVCount = strRoamInputCSVNames.size();
    if (nOpalCSVCount != nRoamCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nRoamCSVCount: " << nRoamCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.setStationLines(mapStationLines);

    opal.calculatePerLineCount();

    map<string, vector<int> > mapOpalPerLineCount = opal.getPerLineCount();

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.setStationLines(mapStationLines);
    roam.calculatePerLineCount();
    map<string, vector<int> > mapRoamPerLineCount = roam.getPerLineCount();

    exportCountComparison(strOutputCSVName, mapOpalPerLineCount, mapRoamPerLineCount, "Opal,Count,Roam,Count");
}


void exportPerPersonExceptions(string strOutputCSVBaseName, string strOutputCSVNameModifier, vector<int> vecCount, int totalRows, string strTitle) {
    // float fSumInaccuracy = 0.0f;
    // float fAccuracy = 0.0f;
    // float fInaccuracy = 0.0f;
    ofstream outfile;

    outfile.open(strOutputCSVBaseName + strOutputCSVNameModifier);
    outfile << strTitle << endl;

    outfile << "Exception Count," << vecCount.size() << endl;
    outfile << "All Rows Count," << totalRows << endl;
    outfile << "Exception Rate," << float(vecCount.size()) / float(totalRows) << endl;
    outfile << "," << endl;

    if (vecCount.size() > 0) {
        outfile << "Rows," << vecCount[0] + 1 << endl;

        for (int i = 1; i < vecCount.size(); i++) {
            outfile << "," << vecCount[i] + 1 << endl;
        }
    }


    // outfile << ",,,,," << fSumInaccuracy/map1.size() << endl;

    outfile.close();
}

void TransportApplication::generateOpalExceptions(std::vector<std::string> strOpalInputCSVNames, std::string strOutputCSVBaseName) {
    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.calculateExceptions();
    // vector<int> vecUnknownOnCount = opal.getUnknownOnCount();
    // vector<int> vecUnknownOffCount = opal.getUnknownOffCount();
    // vector<int> vecSameOnOffCount = opal.getSameOnOffCount();

    exportPerPersonExceptions(strOutputCSVBaseName, "unknown_on_stats.csv", opal.getUnknownOnCount(), opal.getTotalRowCount(), "Opal UNKNOWN tap on stats");
    exportPerPersonExceptions(strOutputCSVBaseName, "unknown_off_stats.csv", opal.getUnknownOffCount(), opal.getTotalRowCount(), "Opal UNKNOWN tap off stats");
    exportPerPersonExceptions(strOutputCSVBaseName, "same_on_off_stats.csv", opal.getSameOnOffCount(), opal.getTotalRowCount(), "Opal same tap on/off stats");
}

void TransportApplication::generateRoamExceptions(std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVBaseName) {
    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculateExceptions();

    exportPerPersonExceptions(strOutputCSVBaseName, "more_than_2_trips_stats.csv", roam.getMT2TripsCount(), roam.getTotalRowCount(), "Roam more than 2 trips stats");
}

void exportPuncExceptions(string strOutputCSVBaseName, string strOutputCSVNameModifier, vector<pair<int, string> > vec, string strTitle) {
    ofstream outfile;

    outfile.open(strOutputCSVBaseName + strOutputCSVNameModifier);
    outfile << strTitle << endl;
    outfile << "Row number";
    outfile << ",Business Centre,Service Date,Segment Direction,Trip Name,Service Type,Service Line,Trip Zone,Orig. Station,Dest. Station,Trip Dprt Status,Trip Arrv Status,Leading Set Number,Leading Set Type,Node Seq Order,Planned Stop Node,Planned Stop Station,Planned Station Dprt Time,Planned Station Arrv Time,Actual Stop Node,Actual Stop Station,Actual Station Dprt Time,Actual Station Arrv Time,Station Arrv Status,Station Dprt Status";
    outfile << endl;

    for (int i = 0; i < vec.size(); i++) {
        outfile << vec[i].first + 1 << "," << vec[i].second << endl;
    }

    outfile.close();
}

void TransportApplication::generatePuncExceptions(std::vector<std::string> strPuncInputCSVNames, std::string strOutputCSVBaseName) {
    PrePuncProcessor ppp;
    ppp.setFiles(strPuncInputCSVNames);
    ppp.calculateExceptions();

    exportPuncExceptions(strOutputCSVBaseName, "unknown_station_list.csv", ppp.getExceptionUnknownStations(), "Punctuality Unknown station List");
    exportPuncExceptions(strOutputCSVBaseName, "missing_actual_stop_list.csv", ppp.getExceptionMissing(), "Punctuality Missing Actual Stop List");
    exportPuncExceptions(strOutputCSVBaseName, "dprt_later_than_arrv_list.csv", ppp.getExceptionDprtLTArrv(), "Punctuality Departure Time Later Than Arrival Time List");
}

void exportLines(string strFileName, map<string, Line> mapLines, string strTitle) {
    ofstream outfile;

    outfile.open(strFileName);

    outfile << strTitle << endl;

    for (map<string, Line>::iterator it = mapLines.begin(); it != mapLines.end(); it++) {
        string strLineName = it->first;
        Line line = it->second;
        outfile << line.strName << "," << line.strType << endl;
        
        for (set<string>::iterator it2 = line.strStationNames.begin(); it2 != line.strStationNames.end(); it2++) {
            outfile << ",," << *it2 << endl;
        }

        outfile << endl;
    }

    outfile.close();
}

void TransportApplication::generateAllLines(string strPuncFileName, string strOutputCSVName) {
    int iTypeCol = 4;
    int iLineCol = 5;
    int iActStopNameCol = 19;

    map<string, Line> mapLines;

    ifstream infile;
    infile.open(strPuncFileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << strPuncFileName << " couldn't be opened." << endl;
        return;
    }

    string value;
    string line;

    int r = 1; // Avoid first row 
    getline(infile, line);
    while (getline(infile, line)) {
        int c = 0;
        stringstream ss(line);

        string strType = "";
        string strLine = "";
        string strActStopName = "";

        while(c < iTypeCol) {
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            c++;
        }

        // Service Type
        getline(ss, value, ',');
        strType = value.substr(1, value.length() - 2);
        c++;

        while(c < iLineCol) {
            getline(ss, value, ',');
            c++;
        }

        // Service Line
        getline(ss, value, ',');
        // cout << r << " " << c << " " << value;
        strLine = value.substr(1, value.length() - 2);
        c++;

        while(c < iActStopNameCol) {
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            c++;
        }

        // Actual Stop Station
        getline(ss, value, ',');
        // cout << c << " " << value << endl;
        strActStopName = value.substr(1, value.length() - 2);
        c++;
        
        while (getline(ss, value, ',')) {
            // cout << r << " " << c << " " << value << endl;
            c++;
        }

        if (strActStopName != "Missing") {
            map<string, Line>::iterator it = mapLines.find(strLine);

            if (it != mapLines.end()) {
                Line &line = it->second;
                line.strStationNames.insert(strActStopName);

                // cout << "found, act stop: " << strActStopName << " " << line.strStationNames.size() << endl;
            }
            else {
                Line line;
                line.strType = strType;
                line.strName = strLine;
                line.strStationNames.insert(strActStopName);

                mapLines.insert(pair<string, Line>(strLine, line));
                            // cout << "didn't found, act stop: " << strActStopName << " " << line.strStationNames.size() << endl;

            }
        }

        r++;
    }

    infile.close();

    exportLines(strOutputCSVName, mapLines, "Service Line,Service Type,Stops");
}

map<string, unsigned int> sumUpVecCountToUIntCount(map<string, vector<int> > mapVecCount) {
    map<string, unsigned int> mapResult;
    for (map<string, vector<int> >::iterator it = mapVecCount.begin(); it != mapVecCount.end(); it++) {
        string strKey = it->first;
        vector<int> vecCounts = it->second;
        unsigned int count = 0;
        for (int i = 0; i < vecCounts.size(); i++) {
            count += vecCounts[i];
        }
        mapResult.insert(pair<string, unsigned int>(strKey, count));
    }
    return mapResult;
}

map<string, unsigned int> sumUpVecRowToUIntCount(map<string, vector<int> > mapVecRows) {
    map<string, unsigned int> mapResult;
    for (map<string, vector<int> >::iterator it = mapVecRows.begin(); it != mapVecRows.end(); it++) {
        string strKey = it->first;
        vector<int> vecRows = it->second;
        mapResult.insert(pair<string, unsigned int>(strKey, vecRows.size()));
    }
    return mapResult;
}


void exportCountComparison(string fileName, map<string, unsigned int> &map1, map<string, unsigned int> &map2, string strTitle) {
    float fSumInaccuracy = 0.0f;
    float fAccuracy = 0.0f;
    float fInaccuracy = 0.0f;
    ofstream outfile;

    outfile.open(fileName);
    outfile << strTitle << endl;

    for (map<string, unsigned int>::iterator it = map1.begin(); it != map1.end(); it++) {
        string strKey = it->first;
        unsigned int count1 = it->second;

        map<string, unsigned int>::iterator it2 = map2.find(strKey);
        if (it2 != map2.end()) {
            unsigned int count2 = it2->second;
            fAccuracy = float(count2)/float(count1);
            fInaccuracy = 1 - fAccuracy;
            outfile << strKey << ", " << count1 << ", " << it2->first << ", " << count2 << ", " << fAccuracy << ", " << fInaccuracy << endl;
        }
        else {
            fAccuracy = 0;
            fInaccuracy = 1;
            outfile << strKey << ", " << count1 << ", " << strKey << ", " << 0 << ", " << 0 << ", " << 1 << endl;
        }

        fSumInaccuracy += fInaccuracy;
    }

    outfile << ",,,,," << fSumInaccuracy/map1.size() << endl;

    outfile.close();
}

void TransportApplication::comparerRoamPPAndRoamPSPerStationPerDay(std::vector<std::string> strRoamPersonInputCSVNames, std::vector<std::string> strRoamStopInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName) {
    int nRoamPersonCSVCount = strRoamPersonInputCSVNames.size();
    int nRoamStopCSVCount = strRoamStopInputCSVNames.size();

    if (nRoamPersonCSVCount != nRoamStopCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nRoamPersonCSVCount: " << nRoamPersonCSVCount << " nRoamStopCSVCount: " << nRoamStopCSVCount << endl;
        return;
    }

    RoamResultAnalyser roam;
    roam.setFiles(strRoamPersonInputCSVNames);
    roam.calculatePerStationCountWithTransfers();
    map<string, unsigned int> mapOnRoamCount = sumUpVecRowToUIntCount(roam.getOnPerStationCount());
    map<string, unsigned int> mapOffRoamCount = sumUpVecRowToUIntCount(roam.getOffPerStationCount());

    RoamPerStopResultAnalyser roamPerStop;
    roamPerStop.setFiles(strRoamStopInputCSVNames);
    roamPerStop.calculatePerStationCount();
    map<string, unsigned int> mapOnRoamPerStopCount = sumUpVecCountToUIntCount(roamPerStop.getOnPerStationCount());
    map<string, unsigned int> mapOffRoamPerStopCount = sumUpVecCountToUIntCount(roamPerStop.getOffPerStationCount());

    exportCountComparison(strOnOutputCSVName, mapOnRoamCount, mapOnRoamPerStopCount, "Per Person Roam,Count,Per Stop Roam,Count");
    exportCountComparison(strOffOutputCSVName, mapOffRoamCount, mapOffRoamPerStopCount, "Per Person Roam,Count,Per Stop Roam,Count");
}

/* Actually opal and cvm are not comparable to each other since cvm per stop data contains transfer the deducted stations, while opal is just raw tap on/off without transfer.
 * Haven't tried to really run this, will wait for discussion.
 */
void TransportApplication::compareOpalAndCvmPerStationPerDay(vector<string> strOpalInputCSVNames, vector<string> strCvmInputCSVNames, 
    string strOnOutputCSVName, string strOffOutputCSVName) {

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.calculatePerStationCount();
    map<string, unsigned int> mapOnOpalCount = sumUpVecRowToUIntCount(roam.getOnPerStationCount());
    map<string, unsigned int> mapOffOpalCount = sumUpVecRowToUIntCount(roam.getOffPerStationCount());

    CvmPerStopResultAnalyser cvmPerStop;
    cvmPerStop.setFiles(strCvmInputCSVNames);
    cvmPerStop.calculatePerStationCount();
    map<string, unsigned int> mapOnCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOnPerStationCount());
    map<string, unsigned int> mapOffCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOffPerStationCount());

    exportCountComparison(strOnOutputCSVName, mapOnOpalPerStationCount, mapOnCvmPerStopCount, "Opal,Count,Cvm,Count");
    exportCountComparison(strOffOutputCSVName, mapOffOpalPerStationCount, mapOffCvmPerStopCount, "Opal,Count,Cvm,Count");
}