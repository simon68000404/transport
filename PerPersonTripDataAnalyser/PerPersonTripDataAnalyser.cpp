#include "PerPersonTripDataAnalyser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

PerPersonTripDataAnalyser::PerPersonTripDataAnalyser() {
    this->m_nTotalRows = 0;

    this->m_vecStationsToSkip.push_back("Newcastle");
    this->m_vecStationsToSkip.push_back("Civic");
    this->m_vecStationsToSkip.push_back("Wickham");
}

bool PerPersonTripDataAnalyser::setFiles(vector<string> strFileNames) {
    this->m_strInfileNames = strFileNames;
}

void PerPersonTripDataAnalyser::setStationLines(std::map<std::string, std::vector<std::string> > mapStationLines) {
    this->m_mapStationLines = mapStationLines;
}

int PerPersonTripDataAnalyser::getTotalRowCount() {
    return m_nTotalRows;
}

std::map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getInterchangeStationCount() {
    return m_mapInterchangeStations;
}

OpalTripAnalyser::OpalTripAnalyser() {
	m_iOnDateCol = 25;
	m_iOnTimeCol = 26;
	m_iOffDateCol = 27;
	m_iOffTimeCol = 28;
	m_iOnStopNameCol = 54; // BC
	m_iOffStopNameCol = 60; // BI
    m_iTripTypeCol = 10;

    // m_strExceptionNoTapOn = "Didn't tap on";
    // m_strExceptionNoTapOff = "Didn't tap off";
}

bool OpalTripAnalyser::shouldSkipOpalRecord(std::string onStopName, std::string offStopName) {
    bool ret = false;
    for (int i = 0; i < m_vecStationsToSkip.size(); i++) {
        if (onStopName == m_vecStationsToSkip[i] || offStopName == m_vecStationsToSkip[i]) {
            ret = true;
            break;
        }
    }

    return ret;
}

void OpalTripAnalyser::calculatePerStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string tripType = "";

            while(c < m_iTripTypeCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            tripType = value;
            c++;

            while(c < m_iOnStopNameCol) {
            	getline(ss, value, '|');
                // cout << c << " " << value << endl;
            	c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iOffStopNameCol) {
            	getline(ss, value, '|');
            	c++;
            }    

            // off stop
            getline(ss, value, '|');
            offStopName = value;
            c++;
            
            while (getline(ss, value, '|')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            // if (onStopName == "Coalcliff Station") {
            //     cout << line << endl;
            // }

            if (tripType == "Train") {
                m_nTotalRows++;

                if ("UNKNOWN" != onStopName) {
                    if ("UNKNOWN" != offStopName) {
                        if (onStopName.compare(offStopName) != 0) {
                            onStopName = onStopName.substr(0, onStopName.length() - 8);
                            offStopName = offStopName.substr(0, offStopName.length() - 8);
                            if (!shouldSkipOpalRecord(onStopName, offStopName)) {
                                updatePerStationCount(onStopName, offStopName, r);
                            }
                        }
                    }
                }
                else {
                                // cout << "r: " << r << onStopName << "       " << offStopName << endl;
                }
            }

            r++;
        }

    	infile.close();
    }   
}

void OpalTripAnalyser::calculatePerODCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string tripType = "";

            while(c < m_iTripTypeCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            tripType = value;
            c++;

            while(c < m_iOnStopNameCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iOffStopNameCol) {
                getline(ss, value, '|');
                c++;
            }    

            // off stop
            getline(ss, value, '|');
            offStopName = value;
            c++;
            
            while (getline(ss, value, '|')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (tripType == "Train") {
                m_nTotalRows++;
                if ("UNKNOWN" != onStopName) {
                    if ("UNKNOWN" != offStopName) {
                        if (onStopName.compare(offStopName) != 0) {
                            onStopName = onStopName.substr(0, onStopName.length() - 8);
                            offStopName = offStopName.substr(0, offStopName.length() - 8);
                            if (!shouldSkipOpalRecord(onStopName, offStopName)) {
                                updatePerODCount(onStopName, offStopName, r);
                            }
                        }
                    }
                }
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        infile.close();
    }   
}

void OpalTripAnalyser::calculateExceptions() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string tripType = "";

            while(c < m_iTripTypeCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            tripType = value;
            c++;

            while(c < m_iOnStopNameCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iOffStopNameCol) {
                getline(ss, value, '|');
                c++;
            }    

            // off stop
            getline(ss, value, '|');
            offStopName = value;
            c++;
            
            while (getline(ss, value, '|')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (tripType == "Train") {
                m_nTotalRows++;

                if ("UNKNOWN" == onStopName) {
                    updateUnknownOnCount(r);
                }
                else if ("UNKNOWN" == offStopName) {
                    updateUnknownOffCount(r);
                }
                else if (onStopName.compare(offStopName) == 0) {
                    updateSameOnOffCount(r);
                }

                // cout << "r: " << r << onStopName << "       " << offStopName << endl;
            }

            r++;
        }

        infile.close();
    }
}


// std::map<std::string, std::vector<int> > OpalTripAnalyser::getExceptions() {
//     return m_mapExceptions;
// }

map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getOnPerStationCount() {
    return m_mapOnCountPerStation;
}
map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getOffPerStationCount() {
    return m_mapOffCountPerStation;
}
map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getPerODCount() {
    return m_mapCountPerOD;
}
map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getPerLineCount() {
    return m_mapCountPerLine;
}
std::vector<int> OpalTripAnalyser::getUnknownOnCount() {
    return m_vecUnknownTapOnCount;
}
std::vector<int> OpalTripAnalyser::getUnknownOffCount() {
    return m_vecUnknownTapOffCount;
}
std::vector<int> OpalTripAnalyser::getSameOnOffCount() {
    return m_vecSameOnOffCount;
}

void PerPersonTripDataAnalyser::updatePerStationCount(std::string strOnStopName, std::string strOffStopName, int nRow) {
    if (strOnStopName.size() > 0) {
    	map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(strOnStopName);
    	if (it == m_mapOnCountPerStation.end()) {
            // cout << "doesn't have" << strOnStopName << endl;
            vector<int> vecOn;
            vecOn.push_back(nRow);

    		m_mapOnCountPerStation.insert(pair<string, vector<int> >(strOnStopName, vecOn));
    	}
        else {
            it->second.push_back(nRow);
        }
    }

    if (strOffStopName.size() > 0) {
        map<string, vector<int> >::iterator it2 = m_mapOffCountPerStation.find(strOffStopName);
        if (it2 == m_mapOffCountPerStation.end()) {
            // cout << "doesn't have" << strOnStopName << endl;
            vector<int> vecOff;
            vecOff.push_back(nRow);

            m_mapOffCountPerStation.insert(pair<string, vector<int> >(strOffStopName, vecOff));
        }
        else {
            it2->second.push_back(nRow); 
        }
    }
}

void PerPersonTripDataAnalyser::updateInterchangeStationCount(std::string strStopName, int nRow) {
    if (strStopName.size() > 0) {
        map<string, vector<int> >::iterator it = m_mapInterchangeStations.find(strStopName);
        if (it == m_mapInterchangeStations.end()) {
            // cout << "doesn't have" << strStopName << endl;
            vector<int> vec;
            vec.push_back(nRow);

            m_mapInterchangeStations.insert(pair<string, vector<int> >(strStopName, vec));
        }
        else {
            it->second.push_back(nRow);
        }
    }
}

void PerPersonTripDataAnalyser::updatePerODCount(std::string strOnStopName, std::string strOffStopName, int nRow) {
    string strODPair = strOnStopName + " - " + strOffStopName;
    map<string, vector<int> >::iterator it = m_mapCountPerOD.find(strODPair);
    if (it == m_mapCountPerOD.end()) {
        // cout << "doesn't have" << strOnStopName << endl;
        vector<int> vec;
        vec.push_back(nRow);

        m_mapCountPerOD.insert(pair<string, vector<int> >(strODPair, vec));
    }
    else {
        it->second.push_back(nRow);
    }
}

void PerPersonTripDataAnalyser::updatePerLineCount(std::string strOnStopName, std::string strOffStopName, int nRow) {
    map<string, vector<string> >::iterator itStationLines;
    string strMountAbbr = "Mt";
    string strNowra = " (Nowra)";
    string strUniversity = " (University)";
    string strAirport = " Airport";
    string strRacecourse = " Racecourse";

    itStationLines = m_mapStationLines.find(strOnStopName);
    vector<string> vecOnStationLines;

    if (itStationLines != m_mapStationLines.end()) {
        vecOnStationLines = itStationLines->second;
    }
    else {
        bool bUnknown = true;
        if (strOnStopName.length() > 7) {
            string strMt = strOnStopName;
            strMt = strMt.replace(0, 5, strMountAbbr);
            
            map<string, vector<string> >::iterator it1 = m_mapStationLines.find(strMt);
            if (it1 != m_mapStationLines.end()) {
// cout << strMt << " " << strOnStopName << endl;
//                 cout << "found" << endl;
                vecOnStationLines = it1->second;
                bUnknown = false;
            }
        }
        
        // Bomaderry -> Bomaderry (Nowra)
        if (bUnknown == true) {
            string str2;
            str2 = strOnStopName + strNowra;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOnStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // Warabrook -> Warabrook (University)
        if (bUnknown == true) {
            string str2;
            str2 = strOnStopName + strUniversity;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOnStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // International -> International Airport
        // Domestic -> Domestic Airport
        if (bUnknown == true) {
            string str2;
            str2 = strOnStopName + strAirport;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOnStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // Kembla Grange -> Kembla Grange Racecourse
        if (bUnknown == true) {
            string str2;
            str2 = strOnStopName + strRacecourse;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOnStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        if (bUnknown) {
            cout << "Unknown on station: " << strOnStopName << endl;
        }
    }

    // cout << m_mapStationLines.begin()->first << " " << m_mapStationLines.begin()->second[0] << " " << strOnStopName << " " << strOffStopName << endl;


    for (int i = 0; i < vecOnStationLines.size(); i++) {
        map<string, vector<int> >::iterator itOn = m_mapCountPerLine.find(vecOnStationLines[i]);
        if (itOn == m_mapCountPerLine.end()) {
            // cout << "doesn't have" << strOnStopName << endl;
            vector<int> vec;
            vec.push_back(nRow);

            m_mapCountPerLine.insert(pair<string, vector<int> >(vecOnStationLines[i], vec));
        }
        else {
            itOn->second.push_back(nRow);
        }
    }

    itStationLines = m_mapStationLines.find(strOffStopName);
    vector<string> vecOffStationLines;

    if (itStationLines != m_mapStationLines.end()) {
        vecOffStationLines = itStationLines->second;
    }
    else {
        bool bUnknown = true;
        if (strOffStopName.length() > 7) {
            string strMt = strOffStopName;
            strMt = strMt.replace(0, 5, strMountAbbr);
            
            map<string, vector<string> >::iterator it1 = m_mapStationLines.find(strMt);
            if (it1 != m_mapStationLines.end()) {
// cout << strMt << " " << strOffStopName << endl;
//                 cout << "found" << endl;
                vecOffStationLines = it1->second;
                bUnknown = false;
            }
        }
        
        if (bUnknown == true) {
            string str2;
            str2 = strOffStopName + strNowra;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOffStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // Warabrook -> Warabrook (University)
        if (bUnknown == true) {
            string str2;
            str2 = strOffStopName + strUniversity;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOffStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // International -> International Airport
        // Domestic -> Domestic Airport
        if (bUnknown == true) {
            string str2;
            str2 = strOffStopName + strAirport;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOffStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        // Kembla Grange -> Kembla Grange Racecourse
        if (bUnknown == true) {
            string str2;
            str2 = strOffStopName + strRacecourse;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOffStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        if (bUnknown) {
            cout << "Unknown off station: " << strOffStopName << endl;
        }
    }
    for (int i = 0; i < vecOffStationLines.size(); i++) {
        map<string, vector<int> >::iterator itOff = m_mapCountPerLine.find(vecOffStationLines[i]);
        if (itOff == m_mapCountPerLine.end()) {
            // cout << "doesn't have" << strOffStopName << endl;
            vector<int> vec;
            vec.push_back(nRow);

            m_mapCountPerLine.insert(pair<string, vector<int> >(vecOffStationLines[i], vec));
        }
        else {
            itOff->second.push_back(nRow);
        }
    }
}

void OpalTripAnalyser::updateUnknownOnCount(int nRow) {
    m_vecUnknownTapOnCount.push_back(nRow);
}
void OpalTripAnalyser::updateUnknownOffCount(int nRow) {
    m_vecUnknownTapOffCount.push_back(nRow);
}
void OpalTripAnalyser::updateSameOnOffCount(int nRow) {
    m_vecSameOnOffCount.push_back(nRow);
}

RoamResultAnalyser::RoamResultAnalyser() {
    m_iOriginStopCol = 2;
    m_iDestStopCol = 1;
    m_iTransferStationCol = 19;
    m_iFirstTripLineCol = 40;
}

void RoamResultAnalyser::calculatePerStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string strFirstTripLineName = "";

            while(c < m_iDestStopCol) {
                getline(ss, value, ',');
                c++;
            }    

            // off stop
            getline(ss, value, ',');
            offStopName = value;
            c++;

            while(c < m_iOriginStopCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;

            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName != "NA") {
                onStopName = onStopName.substr(1, onStopName.length() - 2);
                offStopName = offStopName.substr(1, offStopName.length() - 2);
                updatePerStationCount(onStopName, offStopName, r);
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::calculatePerStationCountWithTransfers() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string strTransferStationName = "";
            string strFirstTripLineName = "";

            while(c < m_iDestStopCol) {
                getline(ss, value, ',');
                c++;
            }    

            // off stop
            getline(ss, value, ',');
            offStopName = value;
            c++;

            while(c < m_iOriginStopCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iTransferStationCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // Transfer station
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strTransferStationName = value;
            c++;

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;
            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName != "NA") {
                onStopName = onStopName.substr(1, onStopName.length() - 2);
                offStopName = offStopName.substr(1, offStopName.length() - 2);
                updatePerStationCount(onStopName, offStopName, r);
                if (strTransferStationName != "NA") {
                    strTransferStationName = strTransferStationName.substr(1, strTransferStationName.length() - 2);
                    cout << strTransferStationName << endl;
                    updatePerStationCount(strTransferStationName, strTransferStationName, r);
                }
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::calculatePerODCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string strFirstTripLineName = "";

            while(c < m_iDestStopCol) {
                getline(ss, value, ',');
                c++;
            }    

            // off stop
            getline(ss, value, ',');
            offStopName = value;
            c++;

            while(c < m_iOriginStopCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;

            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName != "NA") {
                onStopName = onStopName.substr(1, onStopName.length() - 2);
                offStopName = offStopName.substr(1, offStopName.length() - 2);
                updatePerODCount(onStopName, offStopName, r);
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::calculatePerLineCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string strFirstTripLineName = "";

            while(c < m_iDestStopCol) {
                getline(ss, value, ',');
                c++;
            }    

            // off stop
            getline(ss, value, ',');
            offStopName = value;
            c++;

            while(c < m_iOriginStopCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;

            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName != "NA") {
                onStopName = onStopName.substr(1, onStopName.length() - 2);
                offStopName = offStopName.substr(1, offStopName.length() - 2);
                updatePerLineCount(onStopName, offStopName, r);
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::calculateInterchangeStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string strTransferStationName = "";
            string strFirstTripLineName = "";

            while(c < m_iDestStopCol) {
                getline(ss, value, ',');
                c++;
            }    

            // off stop
            getline(ss, value, ',');
            offStopName = value;
            c++;

            while(c < m_iOriginStopCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iTransferStationCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // Transfer station
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strTransferStationName = value;
            c++;

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;
            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName != "NA") {
                if (strTransferStationName != "NA") {
                    strTransferStationName = strTransferStationName.substr(1, strTransferStationName.length() - 2);
                    // cout << strTransferStationName << endl;
                    updateInterchangeStationCount(strTransferStationName, r);
                }
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void OpalTripAnalyser::calculatePerLineCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string onStopName = "";
            string offStopName = "";
            string tripType = "";

            while(c < m_iTripTypeCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            tripType = value;
            c++;

            while(c < m_iOnStopNameCol) {
                getline(ss, value, '|');
                // cout << c << " " << value << endl;
                c++;
            }

            // on stop
            getline(ss, value, '|');
            // cout << c << " " << value << endl;
            onStopName = value;
            c++;

            while(c < m_iOffStopNameCol) {
                getline(ss, value, '|');
                c++;
            }    

            // off stop
            getline(ss, value, '|');
            offStopName = value;
            c++;
            
            while (getline(ss, value, '|')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (tripType == "Train") {
                if ("UNKNOWN" != onStopName) {
                    if ("UNKNOWN" != offStopName) {
                        if (onStopName.compare(offStopName) != 0) {
                            onStopName = onStopName.substr(0, onStopName.length() - 8);
                            offStopName = offStopName.substr(0, offStopName.length() - 8);
                            if (!shouldSkipOpalRecord(onStopName, offStopName)) {
                                updatePerLineCount(onStopName, offStopName, r);
                            }
                        }
                    }
                }
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::calculateExceptions() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strFirstTripLineName = "";

            while(c < m_iFirstTripLineCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }

            // First trip line
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strFirstTripLineName = value;
            c++;

            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strFirstTripLineName == "NA") {
                updateMT2TripsCount(r);
            }
            else {
                // m_mapExceptions.insert(pair<string, vector<int> >(m_strExceptionNotAbleToFindPath, r));
                // cout << "NA: " << r << " ";
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r - 1;

        infile.close();
    }
}

void RoamResultAnalyser::updateMT2TripsCount(int nRow) {
    m_vecMT2TripsCount.push_back(nRow);
}

vector<int> RoamResultAnalyser::getMT2TripsCount() {
    return m_vecMT2TripsCount;    
}

CvmResultAnalyser::CvmResultAnalyser() {
    m_iRunIdCol = 4;
    m_iSubTripIndex = 8;
    m_iOrigStopStationCol = 21;
    m_iDestStopStationCol = 22;
    m_iBoardingStopStationCol = 23;
    m_iAlightingStopStationCol = 24;
    m_iPassengerBehaviourCol = 25;
    m_iPassengerTripIdCol = 26;

    m_cDivider = ',';
}
void CvmResultAnalyser::calculatePerStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing per person cvm: " << m_strInfileNames[i] << endl;

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strRunId = "";
            string strBoardingStopStation = "";
            string strAlightingStopStation = "";
            string strPassengerBehaviour = "";
            unsigned int iPassengerTripId = 0;

            while(c < m_iRunIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strRunId = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iOrigStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // orig stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            string strOriginStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iBoardingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // boarding stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strBoardingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iAlightingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // alighting stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strAlightingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerBehaviourCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger behaviour
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strPassengerBehaviour = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerTripIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            iPassengerTripId = atoi(value.c_str());
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            // if (strOriginStation == "Coalcliff") {
            //     cout << line << endl;
            // }

            if (strPassengerBehaviour == "SINGLE TRIP") {
                updatePerStationCount(strBoardingStopStation, strAlightingStopStation, r);
            }
            else if (strPassengerBehaviour == "BOARDING TRIP") {
                updatePerStationCount(strBoardingStopStation, "", r);
            }
            else if (strPassengerBehaviour == "ALIGHTING TRIP") {
                updatePerStationCount("", strAlightingStopStation, r);
            }
            else if (strPassengerBehaviour == "INTERMEDIATE TRIP") {
            }
            else {
                cout << "Wrong passenger behaviour: " << strPassengerBehaviour << endl;
            }

            r++;
        }

        infile.close();
    }   
}
void CvmResultAnalyser::calculatePerStationCountWithTransfers() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing per person cvm: " << m_strInfileNames[i] << endl;

        string value;
        string line;

        string strLastPassengerTripId = "";

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strRunId = "";
            string strBoardingStopStation = "";
            string strAlightingStopStation = "";
            string strPassengerBehaviour = "";
            string strPassengerTripId = "";

            while(c < m_iRunIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strRunId = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iOrigStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // orig stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            string strOriginStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iBoardingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // boarding stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strBoardingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iAlightingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // alighting stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strAlightingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerBehaviourCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger behaviour
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strPassengerBehaviour = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerTripIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strPassengerTripId = value;
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }


            // if (strLastPassengerTripId != strPassengerTripId) {
            //                     cout << strLastPassengerTripId << endl;
            //     cout << strPassengerTripId << endl;
            //     cout << "last: " + strLastPassengerTripId + " this time: " + strPassengerTripId + " !=? " << (strLastPassengerTripId != strPassengerTripId) << endl;
 
            //     updatePerStationCount(strBoardingStopStation, strAlightingStopStation, r);
            // }
            // else {
            //    updatePerStationCount("", strAlightingStopStation, r);
            // }

            // if (strPassengerBehaviour == "SINGLE TRIP" || strPassengerBehaviour == "BOARDING TRIP") {
            //     updatePerStationCount(strBoardingStopStation, strAlightingStopStation, r);
            //     cout << "s or b: " << strBoardingStopStation << "----" << strAlightingStopStation << "----" << line << endl;
            // }
            // else {
            //     updatePerStationCount("", strAlightingStopStation, r);
            //     cout << "others: " << strBoardingStopStation << "----" << strAlightingStopStation << "----" << line << endl;
            // }

            updatePerStationCount(strBoardingStopStation, strAlightingStopStation, r);

            // if (strBoardingStopStation == "Mindaribba") {
            //     cout << line << endl;
            // }

            r++;
            // strLastPassengerTripId = strPassengerTripId;
        }

        infile.close();
    }  
}

void CvmResultAnalyser::calculateInterchangeStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing per person cvm: " << m_strInfileNames[i] << endl;

        string value;
        string line;

        string strLastPassengerTripId = "";

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strRunId = "";
            string strBoardingStopStation = "";
            string strAlightingStopStation = "";
            string strPassengerBehaviour = "";
            string strPassengerTripId = "";

            while(c < m_iRunIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strRunId = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iOrigStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // orig stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            string strOriginStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iBoardingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // boarding stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strBoardingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iAlightingStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // alighting stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strAlightingStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerBehaviourCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger behaviour
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strPassengerBehaviour = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iPassengerTripIdCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // passenger trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strPassengerTripId = value;
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strPassengerBehaviour != "SINGLE TRIP" && strPassengerBehaviour != "BOARDING TRIP") {
                updateInterchangeStationCount(strBoardingStopStation, r);
                // cout << "others: " << strBoardingStopStation << "----" << strAlightingStopStation << "----" << line << endl;
            }
            // if (strBoardingStopStation == "Mindaribba") {
            //     cout << line << endl;
            // }

            r++;
        }

        infile.close();
    }  
}

void CvmResultAnalyser::calculatePerODCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strSubTripId = "";
            string strOrigStopStation = "";
            string strDestStopStation = "";

            while(c < m_iSubTripIndex) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // sub boarding trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strSubTripId = value;
            c++;

            while(c < m_iOrigStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // orig stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strOrigStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iDestStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // dest stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strDestStopStation = value.substr(1, value.length() - 2);
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strSubTripId == "1") {
                updatePerODCount(strOrigStopStation, strDestStopStation, r);
            }

            r++;
        }

        infile.close();
    }   
}
void CvmResultAnalyser::calculatePerLineCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strSubTripId = "";
            string strOrigStopStation = "";
            string strDestStopStation = "";

            while(c < m_iSubTripIndex) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // sub boarding trip id
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strSubTripId = value;
            c++;

            while(c < m_iOrigStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // orig stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strOrigStopStation = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iDestStopStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }
            // dest stop
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strDestStopStation = value.substr(1, value.length() - 2);
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strSubTripId == "1") {
                updatePerLineCount(strOrigStopStation, strDestStopStation, r);
            }

            r++;
        }

        infile.close();
    }  
}
