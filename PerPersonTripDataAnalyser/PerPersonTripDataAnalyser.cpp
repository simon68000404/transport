#include "PerPersonTripDataAnalyser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

PerPersonTripDataAnalyser::PerPersonTripDataAnalyser() {
    this->m_nTotalRows = 0;
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

            if (tripType == "Train") {
                if ("UNKNOWN" != onStopName) {
                    if ("UNKNOWN" != offStopName) {
                        if (onStopName.compare(offStopName) != 0) {
                            onStopName = onStopName.substr(0, onStopName.length() - 8);
                            offStopName = offStopName.substr(0, offStopName.length() - 8);
                            updatePerStationCount(onStopName, offStopName, r);
                        }
                    }
                }
                else {
                                // cout << "r: " << r << onStopName << "       " << offStopName << endl;
                }
            }

            r++;
        }

        m_nTotalRows += r;

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
                if ("UNKNOWN" != onStopName) {
                    if ("UNKNOWN" != offStopName) {
                        if (onStopName.compare(offStopName) != 0) {
                            onStopName = onStopName.substr(0, onStopName.length() - 8);
                            offStopName = offStopName.substr(0, offStopName.length() - 8);
                            updatePerODCount(onStopName, offStopName, r);
                        }
                    }
                }
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r;

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

        m_nTotalRows += r;

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
        
        if (bUnknown == true) {
            string str2;
            str2 = strOnStopName + strNowra;
            
            map<string, vector<string> >::iterator it2 = m_mapStationLines.find(str2);
            if (it2 != m_mapStationLines.end()) {
                vecOnStationLines = m_mapStationLines.find(str2)->second;
                bUnknown = false;
            }
        }

        if (bUnknown) {
            cout << "Unknown station: " << strOnStopName << endl;
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

        if (bUnknown) {
            cout << "Unknown station: " << strOffStopName << endl;
        }
    }
    for (int i = 0; i < vecOffStationLines.size(); i++) {
        map<string, vector<int> >::iterator itOff = m_mapCountPerLine.find(vecOffStationLines[i]);
        if (itOff == m_mapCountPerLine.end()) {
            // cout << "doesn't have" << strOnStopName << endl;
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

        m_nTotalRows += r;

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

        m_nTotalRows += r;

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

        m_nTotalRows += r;

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
                            updatePerLineCount(onStopName, offStopName, r);
                        }
                    }
                }
            }
            // cout << onStopName << " " << offStopName << endl;

            r++;
        }

        m_nTotalRows += r;

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

        m_nTotalRows += r;

        infile.close();
    }
}

void RoamResultAnalyser::updateMT2TripsCount(int nRow) {
    m_vecMT2TripsCount.push_back(nRow);
}

vector<int> RoamResultAnalyser::getMT2TripsCount() {
    return m_vecMT2TripsCount;    
}