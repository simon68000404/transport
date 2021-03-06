#include "TransportApplication.h"
#include "../PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"
#include "../PerStopDataAnalyser/PerStopDataAnalyser.h"
#include "../PrePuncProcessor/PrePuncProcessor.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <set>

using namespace std;

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

map<string, unsigned int> sumUpVecRowToUIntCount(map<string, vector<unsigned int> > mapVecRows) {
    map<string, unsigned int> mapResult;
    for (map<string, vector<unsigned int> >::iterator it = mapVecRows.begin(); it != mapVecRows.end(); it++) {
        string strKey = it->first;
        vector<unsigned int> vecRows = it->second;
        mapResult.insert(pair<string, unsigned int>(strKey, vecRows.size()));
    }
    return mapResult;
}

void exportCountComparisonWithGEH(string fileName, map<string, unsigned int> &map1, map<string, unsigned int> &map2, string strTitle) {
    float fGEH = 0.0f;
    float fTotalGEH = 0.0f;
    float fSumGEH = 0.0f;
    float fAverageGEH = 0.0f;
    bool bGEHLT5 = false;
    unsigned int nGEHLT5Count = 0;

    float fSumInaccuracy = 0.0f;
    float fAverageInaccuracy = 0.0f;
    float fTotalInaccuracy = 0.0f;
    float fAccuracy = 0.0f;
    float fInaccuracy = 0.0f;

    unsigned int nTotalCount1 = 0;
    unsigned int nTotalCount2 = 0;
    float fAverageCount1 = 0.0f;
    float fAverageCount2 = 0.0f;

    ofstream outfile;

    outfile.open(fileName);
    outfile << strTitle << endl;

    for (map<string, unsigned int>::iterator it = map1.begin(); it != map1.end(); it++) {
        string strKey = it->first;
        unsigned int count1 = it->second;
        nTotalCount1 += count1;

        map<string, unsigned int>::iterator it2 = map2.find(strKey);
        if (it2 != map2.end()) {
            unsigned int count2 = it2->second;
            nTotalCount2 += count2;
            fGEH = sqrt(2.0f * float((count2 - count1) * (count2 - count1)) / float(count2 + count1));
            bGEHLT5 = fGEH < 5 ? true : false;
            nGEHLT5Count += bGEHLT5 == true ? 1 : 0;

            if (count1 > 0) {
                fAccuracy = float(count2)/float(count1);
                fInaccuracy = 1 - fAccuracy;
            }
            else {
                fAccuracy = 0;
                fInaccuracy = 1;
            }

            outfile << strKey << ", " << count1 << ", " << count2 << ", " << fGEH << "," << bGEHLT5 << "," << fInaccuracy << endl;
        }
        else {
            fGEH = sqrt(2.0f * float(count1));
            bGEHLT5 = fGEH < 5 ? true : false;
            nGEHLT5Count += bGEHLT5 == true ? 1 : 0;

            fAccuracy = 0;
            fInaccuracy = 1;

            outfile << strKey << ", " << count1 << ", " << 0 << ", " << fGEH << "," << bGEHLT5 << "," << fInaccuracy << endl;
        }

        fSumInaccuracy += fInaccuracy;
        fSumGEH += fGEH;
    }

    fTotalGEH = sqrt(2.0f * float((nTotalCount2 - nTotalCount1) * (nTotalCount2 - nTotalCount1)) / float(nTotalCount2 + nTotalCount1));
    fAverageGEH = fSumGEH/map1.size();

    fTotalInaccuracy = float(nTotalCount2)/float(nTotalCount1);
    fAverageInaccuracy = fSumInaccuracy/map1.size();

    fAverageCount1 = float(nTotalCount1)/map1.size();
    fAverageCount2 = float(nTotalCount2)/map2.size();

    cout << "map1.size: " << map1.size() << "map2.size: " << map2.size() << endl;

    outfile << "Total" << "," << nTotalCount1 << "," << nTotalCount2 << "," << fTotalGEH << "," << nGEHLT5Count << "," << fTotalInaccuracy << endl;
    outfile << "Average" << "," << fAverageCount1 << "," << fAverageCount2 << "," << fAverageGEH << ",," << fAverageInaccuracy << endl;

    outfile.close();
}

void exportCountComparisonWithGEH2(string fileName, map<string, unsigned int> &map1, map<string, unsigned int> &map2, string strTitle) {
    float fGEH = 0.0f;
    float fTotalGEH = 0.0f;
    float fSumGEH = 0.0f;
    float fAverageGEH = 0.0f;
    bool bGEHLT5 = false;
    unsigned int nGEHLT5Count = 0;

    float fSumInaccuracy = 0.0f;
    float fAverageInaccuracy = 0.0f;
    float fTotalInaccuracy = 0.0f;
    float fAccuracy = 0.0f;
    float fInaccuracy = 0.0f;

    unsigned int nTotalCount1 = 0;
    unsigned int nTotalCount2 = 0;
    float fAverageCount1 = 0.0f;
    float fAverageCount2 = 0.0f;

    set<string> setAllStations;
    for (map<string, unsigned int>::iterator it = map1.begin(); it != map1.end(); it++) {
        setAllStations.insert(it->first);
    }
    for (map<string, unsigned int>::iterator it = map2.begin(); it != map2.end(); it++) {
        setAllStations.insert(it->first);
    }

    ofstream outfile;

    outfile.open(fileName);
    outfile << strTitle << endl;

    for (set<string>::iterator it = setAllStations.begin(); it != setAllStations.end(); it++) {
        unsigned int count1 = 0;
        unsigned int count2 = 0;

        map<string, unsigned int>::iterator it1 = map1.find(*it);
        if (it1 != map1.end()) {
            count1 = it1->second;
        }
        map<string, unsigned int>::iterator it2 = map2.find(*it);
        if (it2 != map2.end()) {
            count2 = it2->second;
        }

        if (count1 > 0 && count2 > 0) {
            fGEH = sqrt(2.0f * float((count2 - count1) * (count2 - count1)) / float(count2 + count1));

            fAccuracy = float(count2)/float(count1);
            fInaccuracy = 1 - fAccuracy;
        }
        else if (count1 > 0) {
            fGEH = sqrt(2.0f * float(count1));
            
            fAccuracy = 0;
            fInaccuracy = 1;
        }
        else if (count2 > 0) {
            fGEH = sqrt(2.0f * float(count2));
            
            fAccuracy = 0;
            fInaccuracy = 1;
        }

        bGEHLT5 = fGEH < 5 ? true : false;
        nGEHLT5Count += bGEHLT5 == true ? 1 : 0;

        nTotalCount1 += count1;
        nTotalCount2 += count2;

        outfile << *it << ", " << count1 << ", " << count2 << ", " << fGEH << "," << bGEHLT5 << "," << fInaccuracy << endl;

        fSumInaccuracy += fInaccuracy;
        fSumGEH += fGEH;
    }

    fTotalGEH = sqrt(2.0f * float((nTotalCount2 - nTotalCount1) * (nTotalCount2 - nTotalCount1)) / float(nTotalCount2 + nTotalCount1));
    fAverageGEH = fSumGEH/map1.size();

    fTotalInaccuracy = float(nTotalCount2)/float(nTotalCount1);
    fAverageInaccuracy = fSumInaccuracy/map1.size();

    fAverageCount1 = float(nTotalCount1)/map1.size();
    fAverageCount2 = float(nTotalCount2)/map2.size();

    cout << "map1.size: " << map1.size() << "map2.size: " << map2.size() << "combined size: " << setAllStations.size() << endl;

    outfile << "Total" << "," << nTotalCount1 << "," << nTotalCount2 << "," << fTotalGEH << "," << nGEHLT5Count << "," << fTotalInaccuracy << endl;
    outfile << "Average" << "," << fAverageCount1 << "," << fAverageCount2 << "," << fAverageGEH << ",," << fAverageInaccuracy << endl;

    outfile.close();
}

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
    map<string, unsigned int> mapOnOpalPerStationCount = sumUpVecRowToUIntCount(opal.getOnPerStationCount());
    map<string, unsigned int> mapOffOpalPerStationCount = sumUpVecRowToUIntCount(opal.getOffPerStationCount());

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculatePerStationCount();
    map<string, unsigned int> mapOnRoamPerStationCount = sumUpVecRowToUIntCount(roam.getOnPerStationCount());
    map<string, unsigned int> mapOffRoamPerStationCount = sumUpVecRowToUIntCount(roam.getOffPerStationCount());

    exportCountComparisonWithGEH(strOnOutputCSVName, mapOnOpalPerStationCount, mapOnRoamPerStationCount, "Station,Opal,Roam per person,GEH,GEH<5,Inaccuracy");
    exportCountComparisonWithGEH(strOffOutputCSVName, mapOffOpalPerStationCount, mapOffRoamPerStationCount, "Station,Opal,Roam per person,GEH,GEH<5,Inaccuracy");
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
    map<string, unsigned int> mapOpalPerODCount = sumUpVecRowToUIntCount(opal.getPerODCount());

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculatePerODCount();
    map<string, unsigned int> mapRoamPerODCount = sumUpVecRowToUIntCount(roam.getPerODCount());

    exportCountComparisonWithGEH(strOutputCSVName, mapOpalPerODCount, mapRoamPerODCount, "OD pair,Opal,Roam per person,GEH,GEH<5,Inaccuracy");
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

map<string, vector<string> > TransportApplication::importAllStationLinesOneToOne(std::string strAllLinesFileName, unsigned int iStationCol, unsigned int iLineCol) {
    ifstream infile;
    infile.open(strAllLinesFileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << strAllLinesFileName << " couldn't be opened." << endl;
        return map<string, vector<string>>();
    }

    cout << "Loading station to line mapping: " << strAllLinesFileName << endl;

    string value;
    string line;
    map<string, vector<string> > mapAllStationLines;

    int r = 1; // Avoid first row 
    struct tm tm = {};
    getline(infile, line);
    while (getline(infile, line)) {
        int c = 0;
        stringstream ss(line);

        string strStationName = "";
        string strLineName = "";

        while(c < iStationCol) {
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            c++;
        }

        // station
        getline(ss, value, ',');
        // cout << c << " " << value << endl;
        strStationName = value.substr(1, value.length() - 2);
        c++;

        while(c < iLineCol) {
            getline(ss, value, ',');
            c++;
        }    

        // line
        getline(ss, value, ',');
        // cout << c << " " << value << endl;
        strLineName = value.substr(1, value.length() - 2);
        c++;
        
        while (getline(ss, value, ',')) {
            // cout << r << " " << c << " " << value << endl;
            c++;
        }

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

        r++;
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

    map<string, unsigned int> mapOpalPerLineCount = sumUpVecRowToUIntCount(opal.getPerLineCount());

    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.setStationLines(mapStationLines);
    roam.calculatePerLineCount();
    map<string, unsigned int> mapRoamPerLineCount = sumUpVecRowToUIntCount(roam.getPerLineCount());

    // exportCountComparison(strOutputCSVName, mapOpalPerLineCount, mapRoamPerLineCount, "Opal,Count,Roam,Count");
    exportCountComparisonWithGEH(strOutputCSVName, mapOpalPerLineCount, mapRoamPerLineCount, "Line,Opal,Roam per person,GEH,GEH<5,Inaccuracy");
}

void exportPerPersonExceptions(string strOutputCSVBaseName, string strOutputCSVNameModifier, vector<int> vecCount, int totalRows, string strTitle) {
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

void exportPuncExceptions(string strOutputCSVBaseName, string strOutputCSVNameModifier, vector<pair<int, string> > vec, string strTitle, unsigned int nTotalRows) {
    cout << vec.size() << " " << nTotalRows << endl;
    ofstream outfile;

    outfile.open(strOutputCSVBaseName + strOutputCSVNameModifier);
    if (!outfile) {
      cerr << "Can't write to output file " << strOutputCSVBaseName + strOutputCSVNameModifier << endl;
      exit(1);
    }
    
    outfile << "Exception Count," << vec.size() << endl;
    outfile << "All Rows Count," << nTotalRows << endl;
    outfile << "Exception Rate," << float(vec.size()) / float(nTotalRows) << endl;
    outfile << "," << endl;

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

    exportPuncExceptions(strOutputCSVBaseName, "unknown_station_list.csv", ppp.getExceptionUnknownStations(), "Punctuality Unknown station List", ppp.getTotalRowCount());
    exportPuncExceptions(strOutputCSVBaseName, "missing_actual_stop_list.csv", ppp.getExceptionMissing(), "Punctuality Missing Actual Stop List", ppp.getTotalRowCount());
    exportPuncExceptions(strOutputCSVBaseName, "dprt_later_than_arrv_list.csv", ppp.getExceptionDprtLTArrv(), "Punctuality Departure Time Later Than Arrival Time List", ppp.getTotalRowCount());
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

void TransportApplication::comparerCvmPPAndCvmPSPerStationPerDay(std::vector<std::string> strCvmPersonInputCSVNames, std::vector<std::string> strCvmStopInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName) {
    CvmResultAnalyser cvm;
    cvm.setFiles(strCvmPersonInputCSVNames);
    cvm.calculatePerStationCountWithTransfers();
    map<string, unsigned int> mapOnCvmCount = sumUpVecRowToUIntCount(cvm.getOnPerStationCount());
    map<string, unsigned int> mapOffCvmCount = sumUpVecRowToUIntCount(cvm.getOffPerStationCount());

    CvmPerStopResultAnalyser cvmPerStop;
    cvmPerStop.setFiles(strCvmStopInputCSVNames);
    cvmPerStop.calculatePerStationCount();
    map<string, unsigned int> mapOnCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOnPerStationCount());
    map<string, unsigned int> mapOffCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOffPerStationCount());

    exportCountComparison(strOnOutputCSVName, mapOnCvmCount, mapOnCvmPerStopCount, "Per Person cvm,Count,Per Stop cvm,Count");
    exportCountComparison(strOffOutputCSVName, mapOffCvmCount, mapOffCvmPerStopCount, "Per Person cvm,Count,Per Stop cvm,Count");
}

/* Actually opal and cvm are not comparable to each other since cvm per stop data contains transfer the deducted stations, while opal is just raw tap on/off without transfer.
 * Haven't tried to really run this, will wait for discussion.
 * As discussed, compare cvm with roam per stop.
 */
void TransportApplication::compareRoamAndCvmPerStationFromPerStopData(vector<string> strRoamInputCSVNames, vector<string> strCvmInputCSVNames, 
    string strOnOutputCSVName, string strOffOutputCSVName) {

    RoamPerStopResultAnalyser roamPerStop;
    roamPerStop.setFiles(strRoamInputCSVNames);
    roamPerStop.calculatePerStationCount();
    map<string, unsigned int> mapOnRoamPerStopCount = sumUpVecCountToUIntCount(roamPerStop.getOnPerStationCount());
    map<string, unsigned int> mapOffRoamPerStopCount = sumUpVecCountToUIntCount(roamPerStop.getOffPerStationCount());

    CvmPerStopResultAnalyser cvmPerStop;
    cvmPerStop.setFiles(strCvmInputCSVNames);
    cvmPerStop.calculatePerStationCount();
    map<string, unsigned int> mapOnCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOnPerStationCount());
    map<string, unsigned int> mapOffCvmPerStopCount = sumUpVecCountToUIntCount(cvmPerStop.getOffPerStationCount());

    exportCountComparisonWithGEH(strOnOutputCSVName, mapOnRoamPerStopCount, mapOnCvmPerStopCount, "Station,Roam per stop,Cvm per stop,GEH,GEH<5,Inaccuracy");
    exportCountComparisonWithGEH(strOffOutputCSVName, mapOffRoamPerStopCount, mapOffCvmPerStopCount, "Station,Roam per stop,Cvm per stop,GEH,GEH<5,Inaccuracy");
}

void TransportApplication::compareRoamAndCvmInterchangesPerDay(std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName) {
    RoamResultAnalyser roam;
    roam.setFiles(strRoamInputCSVNames);
    roam.calculateInterchangeStationCount();
    map<string, unsigned int> mapRoamInterchangeCount = sumUpVecRowToUIntCount(roam.getInterchangeStationCount());

    CvmResultAnalyser cvm;
    cvm.setFiles(strCvmInputCSVNames);
    cvm.calculateInterchangeStationCount();
    map<string, unsigned int> mapCvmInterchangeCount = sumUpVecRowToUIntCount(cvm.getInterchangeStationCount());

    cout << mapRoamInterchangeCount.size() << " " << mapCvmInterchangeCount.size() << endl;

    exportCountComparisonWithGEH2(strOutputCSVName, mapRoamInterchangeCount, mapCvmInterchangeCount, "Station,Roam per person,Cvm per person,GEH,GEH<5,Inaccuracy");
}

void exportCompleteness(vector<string> vecAllTripNames, map<string, map<string, unsigned int> > &mapTripNamesFromPathFindingMethods, string strOutputCSVName) {
    unsigned int nMapRoamCoverageCount = 0;
    unsigned int nMapCvmCoverageCount = 0;

    ofstream outfile;

    outfile.open(strOutputCSVName);
    if (!outfile) {
      cerr << "Can't write to output file " << strOutputCSVName << endl;
      exit(1);
    }
    outfile << "All trip names in Punctuality,Roam,Cvm" << endl;

    map<string, map<string, unsigned int> >::iterator itRoam = mapTripNamesFromPathFindingMethods.find("Roam");
    map<string, map<string, unsigned int> >::iterator itCvm = mapTripNamesFromPathFindingMethods.find("Cvm");

    map<string, unsigned int> mapRoam;
    map<string, unsigned int> mapCvm;

    if (itRoam != mapTripNamesFromPathFindingMethods.end()) {
        mapRoam = itRoam->second;
    }

    if (itCvm != mapTripNamesFromPathFindingMethods.end()) {
        mapCvm = itCvm->second;
    }

    for (int i = 0; i < vecAllTripNames.size(); i++) {
        outfile << vecAllTripNames[i];

        map<string, unsigned int>::iterator itMapRoam;
        map<string, unsigned int>::iterator itMapCvm;

        if (mapRoam.size() > 0) {
            itMapRoam = mapRoam.find(vecAllTripNames[i]);

            if (itMapRoam != mapRoam.end()) {
                outfile << "," << itMapRoam->second;
                nMapRoamCoverageCount++;
            }
            else {
                outfile << ",0";
            }
        }

        if (mapCvm.size() > 0) {
            itMapCvm = mapCvm.find(vecAllTripNames[i]);

            cout << vecAllTripNames[i] << endl;

            if (itMapCvm != mapCvm.end()) {
                outfile << "," << itMapCvm->second;
                nMapCvmCoverageCount++;
                if (vecAllTripNames[i] == "10-A") {
                    cout << "cvm" + vecAllTripNames[i] << " " << nMapCvmCoverageCount << endl;
                }
            }
            else {
                outfile << ",0";
                if (vecAllTripNames[i] == "10-A") {
                    cout << "cvm" + vecAllTripNames[i] << endl;
                }
            }
        }

        outfile << endl;
    }

    outfile << "Total," << nMapRoamCoverageCount << "," << nMapCvmCoverageCount << endl;
    outfile << "Completeness Rate," << float(nMapRoamCoverageCount)/float(vecAllTripNames.size()) << "," << float(nMapCvmCoverageCount)/float(vecAllTripNames.size()) << endl;

    outfile.close();
}

vector<TrainTripStop> findTheTrainTripStops(vector<TrainTripStop> &vec, TrainTripStop &tripStop, bool bFindAll) {
    vector<TrainTripStop> ret;
    for (int i = 0; i < vec.size(); i++) {
        if (tripStop.m_strServiceDate == vec[i].m_strServiceDate
            && tripStop.m_strTripID == vec[i].m_strTripID
            && tripStop.m_strStationName == vec[i].m_strStationName
            && tripStop.m_strDateTime == vec[i].m_strDateTime) {
            ret.push_back(vec[i]);
            if (!bFindAll)
                break;
        }
    }

    return ret;
}

vector<TrainTripStop> findTheNoTimeTrainTripStops(vector<TrainTripStop> &vec, TrainTripStop &tripStop, bool bFindAll) {
    vector<TrainTripStop> ret;
    // cout << "The stop: -----------------------------------------" << tripStop.m_strStationName << endl;
    for (int i = 0; i < vec.size(); i++) {
        // if (vec[i].m_strStationName == "Narara")
        //     cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << tripStop.m_strPlannedStationName << " " << vec[i].m_strStationName << " " << (tripStop.m_strPlannedStationName == vec[i].m_strStationName) << endl;
        if (tripStop.m_strServiceDate == vec[i].m_strServiceDate
            && tripStop.m_strTripID == vec[i].m_strTripID
            && tripStop.m_strPlannedStationName == vec[i].m_strStationName) {
            // cout << "pushed------------" << endl;
            ret.push_back(vec[i]);
            if (!bFindAll)
                break;
        }
    }

    return ret;
}

void exportCompleteness3(vector<TrainTripStop> vecAllTripStops, map<string, vector<TrainTripStop> > &mapTripNamesFromPathFindingMethods, string strOutputCSVName) {
    unsigned int nRoamCoverageCount = 0;
    unsigned int nCvmCoverageCount = 0;

    ofstream outfile;

    outfile.open(strOutputCSVName);
    if (!outfile) {
      cerr << "Can't write to output file " << strOutputCSVName << endl;
      exit(1);
    }
    outfile << "Service Date,Trip ID, Planned Station, Actual Station, Actual Arrival Time, ROAM, CVM" << endl;

    map<string, vector<TrainTripStop> >::iterator itRoam = mapTripNamesFromPathFindingMethods.find("Roam");
    map<string, vector<TrainTripStop> >::iterator itCvm = mapTripNamesFromPathFindingMethods.find("Cvm");

    vector<TrainTripStop> vecRoam;
    vector<TrainTripStop> vecCvm;

    if (itRoam != mapTripNamesFromPathFindingMethods.end()) {
        vecRoam = itRoam->second;
    }

    if (itCvm != mapTripNamesFromPathFindingMethods.end()) {
        vecCvm = itCvm->second;
    }

    for (int i = 0; i < vecAllTripStops.size(); i++) {
        vector<TrainTripStop> vecRoamMatchedStops;
        TrainTripStop roamMatchedStop;
        bool bRoamFound = false;

        if (vecRoam.size() > 0) {
            if (vecAllTripStops[i].m_strDateTime == "") {
                vecRoamMatchedStops = findTheNoTimeTrainTripStops(vecRoam, vecAllTripStops[i], false);
                cout << "roam found interpolated " << vecAllTripStops[i].m_strTripID << endl;
                cout << "punc " << vecAllTripStops[i].m_strServiceDate << " planned:" << vecAllTripStops[i].m_strPlannedStationName << vecAllTripStops[i].m_strTripID << " " << vecAllTripStops[i].m_strStationName << " " << vecAllTripStops[i].m_strDateTime << vecAllTripStops[i].m_nUnboardingCount << " " << vecAllTripStops[i].m_nBoardingCount << endl;
            }
            else {
                vecRoamMatchedStops = findTheTrainTripStops(vecRoam, vecAllTripStops[i], false);
                cout << "roam found " << vecAllTripStops[i].m_strTripID << endl;
            }

            if (vecRoamMatchedStops.size() > 0) {
                roamMatchedStop = vecRoamMatchedStops[0];

                nRoamCoverageCount++;

                bRoamFound = true;
            }
        }

        vector<TrainTripStop> vecCvmMatchedStops;
        TrainTripStop cvmMatchedStop;
        bool bCvmFound = false;

        if (vecCvm.size() > 0) {
            if (vecAllTripStops[i].m_strDateTime == "") {
                cout << "cvm found interpolated " << vecAllTripStops[i].m_strTripID << endl;
                vecCvmMatchedStops = findTheNoTimeTrainTripStops(vecCvm, vecAllTripStops[i], false);
            }
            else {
                vecCvmMatchedStops = findTheTrainTripStops(vecCvm, vecAllTripStops[i], false);
                cout << "cvm found " << vecAllTripStops[i].m_strTripID << endl;
            }

            if (vecCvmMatchedStops.size() > 0) {
                cvmMatchedStop = vecCvmMatchedStops[0];

                nCvmCoverageCount++;

                bCvmFound = true;
            }
        }

        // cout << (bRoamFound ? "ROAM found" : "ROAM not found") << endl;

        outfile << vecAllTripStops[i].m_strServiceDate 
            << "," << vecAllTripStops[i].m_strTripID 
            << "," << vecAllTripStops[i].m_strPlannedStationName
            << "," << vecAllTripStops[i].m_strStationName 
            << "," << vecAllTripStops[i].m_strDateTime;

        if (bRoamFound) {
            outfile << ",yes";
        }
        else {
            outfile << ",";
        } 

        if (bCvmFound) {
            outfile << ",yes";
        }
        else {
            outfile << ",";
        } 
            
        outfile << endl;
    }

    outfile << "Total," << nRoamCoverageCount << "," << nCvmCoverageCount << endl;
    outfile << "Completeness Rate," << float(nRoamCoverageCount)/float(vecAllTripStops.size()) << "," << float(nCvmCoverageCount)/float(vecAllTripStops.size()) << endl;

    outfile.close();
}

void exportCompleteness2(vector<TrainTripStop> vecAllTripStops, map<string, vector<TrainTripStop> > &mapTripNamesFromPathFindingMethods, string strOutputCSVName) {
    unsigned int nRoamCoverageCount = 0;
    unsigned int nCvmCoverageCount = 0;

    ofstream outfile;

    outfile.open(strOutputCSVName);
    if (!outfile) {
      cerr << "Can't write to output file " << strOutputCSVName << endl;
      exit(1);
    }
    outfile << "Service Date,Trip ID, Station, Actual Arrival Time, ROAM unboarding count, ROAM boarding count, CVM unboarding count, CVM boarding count" << endl;

    map<string, vector<TrainTripStop> >::iterator itRoam = mapTripNamesFromPathFindingMethods.find("Roam");
    map<string, vector<TrainTripStop> >::iterator itCvm = mapTripNamesFromPathFindingMethods.find("Cvm");

    vector<TrainTripStop> vecRoam;
    vector<TrainTripStop> vecCvm;

    if (itRoam != mapTripNamesFromPathFindingMethods.end()) {
        vecRoam = itRoam->second;
    }

    if (itCvm != mapTripNamesFromPathFindingMethods.end()) {
        vecCvm = itCvm->second;
    }

    for (int i = 0; i < vecAllTripStops.size(); i++) {
        vector<TrainTripStop> vecRoamMatchedStops;
        TrainTripStop roamMatchedStop;
        bool bRoamFound = false;

        if (vecRoam.size() > 0) {
            if (vecAllTripStops[i].m_strDateTime == "") {
                vecRoamMatchedStops = findTheTrainTripStops(vecRoam, vecAllTripStops[i], true);
                cout << "found " << vecAllTripStops[i].m_strTripID << endl;
                cout << "punc " << vecAllTripStops[i].m_strServiceDate << " " << vecAllTripStops[i].m_strTripID << " " << vecAllTripStops[i].m_strStationName << " " << vecAllTripStops[i].m_strDateTime << vecAllTripStops[i].m_nUnboardingCount << " " << vecAllTripStops[i].m_nBoardingCount << endl;
            }
            else {
                vecRoamMatchedStops = findTheNoTimeTrainTripStops(vecRoam, vecAllTripStops[i], true);
                cout << "found interpolated " << vecAllTripStops[i].m_strTripID << endl;
            }

            if (vecRoamMatchedStops.size() > 0) {
                roamMatchedStop = vecRoamMatchedStops[0];

                nRoamCoverageCount++;

                if (vecRoamMatchedStops.size() > 1) {
                    // for (int j = 1; j < vecRoamMatchedStops.size(); j++) {
                    //     cout << "Roam " << vecRoamMatchedStops[j].m_strServiceDate << " " << vecRoamMatchedStops[j].m_strTripID << " " << vecRoamMatchedStops[j].m_strStationName << " " << vecRoamMatchedStops[j].m_strDateTime << vecRoamMatchedStops[j].m_nUnboardingCount << " " << vecRoamMatchedStops[j].m_nBoardingCount << endl;
                    //     roamMatchedStop.m_nUnboardingCount += vecRoamMatchedStops[i].m_nUnboardingCount;
                    //     roamMatchedStop.m_nBoardingCount += vecRoamMatchedStops[i].m_nBoardingCount;
                    // }
                    cout << "Multiple rows matched." << endl;
                }

                bRoamFound = true;
            }
        }

        vector<TrainTripStop> vecCvmMatchedStops;
        TrainTripStop cvmMatchedStop;
        bool bCvmFound = false;

        if (vecCvm.size() > 0) {
            if (vecAllTripStops[i].m_strDateTime == "") {
                vecCvmMatchedStops = findTheTrainTripStops(vecCvm, vecAllTripStops[i], true);
            }
            else {
                vecCvmMatchedStops = findTheNoTimeTrainTripStops(vecCvm, vecAllTripStops[i], true);
            }

            if (vecCvmMatchedStops.size() > 0) {
                cvmMatchedStop = vecCvmMatchedStops[0];

                nCvmCoverageCount++;

                if (vecCvmMatchedStops.size() > 1) {
                    // for (int j = 1; j < vecCvmMatchedStops.size(); j++) {
                    //     cout << "Cvm " << vecCvmMatchedStops[j].m_strServiceDate << " " << vecCvmMatchedStops[j].m_strTripID << " " << vecCvmMatchedStops[j].m_strStationName << " " << vecCvmMatchedStops[j].m_strDateTime << vecCvmMatchedStops[j].m_nUnboardingCount << " " << vecCvmMatchedStops[j].m_nBoardingCount << endl;
                    //     cvmMatchedStop.m_nUnboardingCount += vecCvmMatchedStops[i].m_nUnboardingCount;
                    //     cvmMatchedStop.m_nBoardingCount += vecCvmMatchedStops[i].m_nBoardingCount;
                    // }
                    cout << "Multiple rows matched." << endl;
                }

                bCvmFound = true;
            }
        }

        outfile << vecAllTripStops[i].m_strServiceDate 
            << "," << vecAllTripStops[i].m_strTripID 
            << "," << vecAllTripStops[i].m_strStationName 
            << "," << vecAllTripStops[i].m_strDateTime;

        if (bRoamFound) {
            outfile << "," << roamMatchedStop.m_nUnboardingCount
                << "," << roamMatchedStop.m_nBoardingCount;
        }
        else {
            outfile << ",,";
        } 

        if (bCvmFound) {
            outfile << "," << cvmMatchedStop.m_nUnboardingCount
                << "," << cvmMatchedStop.m_nBoardingCount;
        }
        else {
            outfile << ",,";
        } 
            
        outfile << endl;
    }

    outfile << "Total," << nRoamCoverageCount << "," << nCvmCoverageCount << endl;
    outfile << "Completeness Rate," << float(nRoamCoverageCount)/float(vecAllTripStops.size()) << "," << float(nCvmCoverageCount)/float(vecAllTripStops.size()) << endl;

    outfile.close();
}

void exportCompleteness(vector<TrainTripStop> vecAllTripStopNames, map<string, map<string, unsigned int> > &mapTripStopsFromPathFindingMethods, string strOutputCSVName) {
    // unsigned int nMapRoamCoverageCount = 0;
    // unsigned int nMapCvmCoverageCount = 0;

    // ofstream outfile;

    // outfile.open(strOutputCSVName);
    // if (!outfile) {
    //   cerr << "Can't write to output file " << strOutputCSVName << endl;
    //   exit(1);
    // }
    // outfile << "All trip names in Punctuality,Roam,Cvm" << endl;

    // map<string, map<string, unsigned int> >::iterator itRoam = mapTripNamesFromPathFindingMethods.find("Roam");
    // map<string, map<string, unsigned int> >::iterator itCvm = mapTripNamesFromPathFindingMethods.find("Cvm");

    // map<string, unsigned int> mapRoam;
    // map<string, unsigned int> mapCvm;

    // if (itRoam != mapTripStopsFromPathFindingMethods.end()) {
    //     mapRoam = itRoam->second;
    // }

    // if (itCvm != mapTripStopsFromPathFindingMethods.end()) {
    //     mapCvm = itCvm->second;
    // }

    // for (int i = 0; i < vecAllTripNames.size(); i++) {
    //     outfile << vecAllTripNames[i];

    //     map<string, unsigned int>::iterator itMapRoam;
    //     map<string, unsigned int>::iterator itMapCvm;

    //     if (mapRoam.size() > 0) {
    //         itMapRoam = mapRoam.find(vecAllTripNames[i]);

    //         if (itMapRoam != mapRoam.end()) {
    //             outfile << "," << itMapRoam->second;
    //             nMapRoamCoverageCount++;
    //         }
    //         else {
    //             outfile << ",0";
    //         }
    //     }

    //     if (mapCvm.size() > 0) {
    //         itMapCvm = mapCvm.find(vecAllTripNames[i]);

    //         cout << vecAllTripNames[i] << endl;

    //         if (itMapCvm != mapCvm.end()) {
    //             outfile << "," << itMapCvm->second;
    //             nMapCvmCoverageCount++;
    //             if (vecAllTripNames[i] == "10-A") {
    //                 cout << "cvm" + vecAllTripNames[i] << " " << nMapCvmCoverageCount << endl;
    //             }
    //         }
    //         else {
    //             outfile << ",0";
    //             if (vecAllTripNames[i] == "10-A") {
    //                 cout << "cvm" + vecAllTripNames[i] << endl;
    //             }
    //         }
    //     }

    //     outfile << endl;
    // }

    // outfile << "Total," << nMapRoamCoverageCount << "," << nMapCvmCoverageCount << endl;
    // outfile << "Completeness Rate," << float(nMapRoamCoverageCount)/float(vecAllTripNames.size()) << "," << float(nMapCvmCoverageCount)/float(vecAllTripNames.size()) << endl;

    // outfile.close();
}

void TransportApplication::checkCompleteness(std::vector<std::string> strPuncInputCSVNames, std::map<std::string, std::vector<std::string> > mapPerStopInputCSVNames, std::string strOutputCSVName) {
    PrePuncProcessor punc;
    punc.setFiles(strPuncInputCSVNames);
    punc.extractTripNames();
    vector<string> vecAllTripNames = punc.getTripNames();

    map<string, vector<string> >::iterator it;
    vector<string> vecRoamInputFiles;
    vector<string> vecCvmInputFiles;
    
    it = mapPerStopInputCSVNames.find("Roam");
    if (it != mapPerStopInputCSVNames.end()) {
        vecRoamInputFiles = it->second;
    }

    it = mapPerStopInputCSVNames.find("Cvm");
    if (it != mapPerStopInputCSVNames.end()) {
        vecCvmInputFiles = it->second;
    }

    map<string, unsigned int> mapRoamTripNames;
    map<string, unsigned int> mapCvmTripNames;
    map<string, map<string, unsigned int> > mapTripNamesFromPathFindingMethods;

    if (vecRoamInputFiles.size() > 0) {
        RoamPerStopResultAnalyser roam;
        roam.setFiles(vecRoamInputFiles);
        roam.extractTripNameRows();
        mapRoamTripNames = sumUpVecRowToUIntCount(roam.getTripNameRows());
        
        mapTripNamesFromPathFindingMethods.insert(pair<string, map<string, unsigned int> >("Roam", mapRoamTripNames));
    }

    if (vecCvmInputFiles.size() > 0) {
        CvmPerStopResultAnalyser cvm;
        cvm.setFiles(vecCvmInputFiles);
        cvm.extractTripNameRows();
        mapCvmTripNames = sumUpVecRowToUIntCount(cvm.getTripNameRows());

        mapTripNamesFromPathFindingMethods.insert(pair<string, map<string, unsigned int> >("Cvm", mapCvmTripNames));
    }

    exportCompleteness(vecAllTripNames, mapTripNamesFromPathFindingMethods, strOutputCSVName);
}

void TransportApplication::checkCompleteness2(std::vector<std::string> strPuncInputCSVNames, std::map<std::string, std::vector<std::string> > mapPerStopInputCSVNames, std::string strOutputCSVName) {
    PrePuncProcessor punc;
    punc.setFiles(strPuncInputCSVNames);
    punc.extractTripStops();
    vector<TrainTripStop> &vecAllTripStops = punc.getTripStops();

    map<string, vector<string> >::iterator it;
    vector<string> vecRoamInputFiles;
    vector<string> vecCvmInputFiles;
    
    it = mapPerStopInputCSVNames.find("Roam");
    if (it != mapPerStopInputCSVNames.end()) {
        vecRoamInputFiles = it->second;
    }

    it = mapPerStopInputCSVNames.find("Cvm");
    if (it != mapPerStopInputCSVNames.end()) {
        vecCvmInputFiles = it->second;
    }

    vector<TrainTripStop> vecRoamTripStops;
    vector<TrainTripStop> vecCvmTripStops;
    map<string, vector<TrainTripStop> > mapTripStopNamesFromPathFindingMethods;

    if (vecRoamInputFiles.size() > 0) {
        RoamPerStopResultAnalyser roam;
        roam.setFiles(vecRoamInputFiles);
        roam.extractTripStops();
        vecRoamTripStops = roam.getTripStops();
        
        mapTripStopNamesFromPathFindingMethods.insert(pair<string, vector<TrainTripStop> >("Roam", vecRoamTripStops));
    }

    if (vecCvmInputFiles.size() > 0) {
        CvmPerStopResultAnalyser cvm;
        cvm.setFiles(vecCvmInputFiles);
        cvm.extractTripStops();
        vecCvmTripStops = cvm.getTripStops();

        mapTripStopNamesFromPathFindingMethods.insert(pair<string, vector<TrainTripStop> >("Cvm", vecCvmTripStops));
    }

    cout << vecAllTripStops.size() << " " << vecRoamTripStops.size() << " " << vecCvmTripStops.size() << endl;

    exportCompleteness3(vecAllTripStops, mapTripStopNamesFromPathFindingMethods, strOutputCSVName);
}

void TransportApplication::compareOpalAndCvmPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nCvmCSVCount = strCvmInputCSVNames.size();

    if (nOpalCSVCount != nCvmCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nCvmCSVCount: " << nCvmCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames); 
    opal.calculatePerStationCount();
    map<string, unsigned int> mapOnOpalPerStationCount = sumUpVecRowToUIntCount(opal.getOnPerStationCount());
    map<string, unsigned int> mapOffOpalPerStationCount = sumUpVecRowToUIntCount(opal.getOffPerStationCount());

    CvmResultAnalyser cvm;
    cvm.setFiles(strCvmInputCSVNames);
    cvm.calculatePerStationCount();
    map<string, unsigned int> mapOnCvmPerStationCount = sumUpVecRowToUIntCount(cvm.getOnPerStationCount());
    map<string, unsigned int> mapOffCvmPerStationCount = sumUpVecRowToUIntCount(cvm.getOffPerStationCount());

    cout << "cvm: " << mapOnOpalPerStationCount.size() << " " << mapOffCvmPerStationCount.size() << endl;

    exportCountComparisonWithGEH(strOnOutputCSVName, mapOnOpalPerStationCount, mapOnCvmPerStationCount, "Station,Opal,Cvm per person,GEH,GEH<5,Inaccuracy");
    exportCountComparisonWithGEH(strOffOutputCSVName, mapOffOpalPerStationCount, mapOffCvmPerStationCount, "Station,Opal,Cvm per person,GEH,GEH<5,Inaccuracy");
}
void TransportApplication::compareOpalAndCvmPerLinePerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName, std::map<std::string, std::vector<std::string> > mapStationLines) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nCvmCSVCount = strCvmInputCSVNames.size();
    if (nOpalCSVCount != nCvmCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nCvmCSVCount: " << nCvmCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.setStationLines(mapStationLines);

    opal.calculatePerLineCount();

    map<string, unsigned int> mapOpalPerLineCount = sumUpVecRowToUIntCount(opal.getPerLineCount());

    CvmResultAnalyser cvm;
    cvm.setFiles(strCvmInputCSVNames);
    cvm.setStationLines(mapStationLines);
    cvm.calculatePerLineCount();
    map<string, unsigned int> mapCvmPerLineCount = sumUpVecRowToUIntCount(cvm.getPerLineCount());

    // exportCountComparison(strOutputCSVName, mapOpalPerLineCount, mapRoamPerLineCount, "Opal,Count,Roam,Count");
    exportCountComparisonWithGEH(strOutputCSVName, mapOpalPerLineCount, mapCvmPerLineCount, "Line,Opal,Cvm per person,GEH,GEH<5,Inaccuracy");
}
void TransportApplication::compareOpalAndCvmPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName) {
    int nOpalCSVCount = strOpalInputCSVNames.size();
    int nCvmCSVCount = strCvmInputCSVNames.size();
    if (nOpalCSVCount != nCvmCSVCount) {
        cout << "The csv name arrays given are not consistent: " << endl;
        cout << "nOpalCSVCount: " << nOpalCSVCount << " nCvmCSVCount: " << nCvmCSVCount << endl;
        return;
    }

    OpalTripAnalyser opal;
    opal.setFiles(strOpalInputCSVNames);
    opal.calculatePerODCount();
    map<string, unsigned int> mapOpalPerODCount = sumUpVecRowToUIntCount(opal.getPerODCount());

    CvmResultAnalyser cvm;
    cvm.setFiles(strCvmInputCSVNames);
    cvm.calculatePerODCount();
    map<string, unsigned int> mapCvmPerODCount = sumUpVecRowToUIntCount(cvm.getPerODCount());

    exportCountComparisonWithGEH(strOutputCSVName, mapOpalPerODCount, mapCvmPerODCount, "OD pair,Opal,Cvm per person,GEH,GEH<5,Inaccuracy");

}
