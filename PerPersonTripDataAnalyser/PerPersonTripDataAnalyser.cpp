#include "PerPersonTripDataAnalyser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool PerPersonTripDataAnalyser::setFile(string fileName) {
	if (fileName.length() == 0) {
        cout << "File name is empty.";
        return false;
    }
	this->m_infileName = fileName;
}

OpalTripAnalyser::OpalTripAnalyser() {
	m_iOnDateCol = 25;
	m_iOnTimeCol = 26;
	m_iOffDateCol = 27;
	m_iOffTimeCol = 28;
	m_iOnStopNameCol = 54; // BC
	m_iOffStopNameCol = 60; // BI
    m_iTripTypeCol = 10;
}

void OpalTripAnalyser::calculatePerStationCount() {
    ifstream infile;
    infile.open(m_infileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << m_infileName << " couldn't be opened.";
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
            onStopName = onStopName.substr(0, onStopName.length() - 8);
            offStopName = offStopName.substr(0, offStopName.length() - 8);
            updateCount(onStopName, offStopName, r);
        }
        // cout << onStopName << " " << offStopName << endl;

        r++;
        // if (r == 1000) {
        //     break;
        // }
    }

	infile.close();   

    // for (map<string, vector<int> >::iterator it = m_mapOnCountPerStation.begin(); it != m_mapOnCountPerStation.end(); ++it) {
    //     string strStationName = it->first;
    //     vector<int> vec = it->second;
    //     std::cout << it->first << ", " << vec.size() << ", " << endl;
    // }

    // cout << "stations count: " << m_mapOnCountPerStation.size() << endl;

    // string ie = "Grand Hotel Pacific Hwy near Alison Rd";
    // map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(ie);
    // vector<int> vec = it->second;

    // for (int i = 0; i < vec.size(); i++) {
    //     cout << vec[i] << endl;
    // }
}

map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getOnPerStationCount() {
    return m_mapOnCountPerStation;
}
map<std::string, std::vector<int> > PerPersonTripDataAnalyser::getOffPerStationCount() {
    return m_mapOffCountPerStation;
}

void PerPersonTripDataAnalyser::updateCount(std::string strOnStopName, std::string strOffStopName, int nRow) {
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

RoamResultAnalyser::RoamResultAnalyser() {
    m_iOnStopFirstTripCol = 2;
    m_iOffStopLastTripCol = 1;
}

void RoamResultAnalyser::calculatePerStationCount() {
    ifstream infile;
    infile.open(m_infileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << m_infileName << " couldn't be opened." << endl;
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

        while(c < m_iOffStopLastTripCol) {
            getline(ss, value, ',');
            c++;
        }    

        // off stop
        getline(ss, value, ',');
        offStopName = value;
        c++;

        while(c < m_iOnStopFirstTripCol) {
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            c++;
        }

        // on stop
        getline(ss, value, ',');
        // cout << c << " " << value << endl;
        onStopName = value;
        c++;
        
        while (getline(ss, value, ',')) {
            // cout << r << " " << c << " " << value << endl;
            c++;
        }

        onStopName = onStopName.substr(1, onStopName.length() - 2);
        offStopName = offStopName.substr(1, offStopName.length() - 2);
        updateCount(onStopName, offStopName, r);
        // cout << onStopName << " " << offStopName << endl;

        r++;
        // if (r == 1000) {
        //     break;
        // }
    }

    infile.close();

    // On
    // for (map<string, vector<int> >::iterator it = m_mapOnCountPerStation.begin(); it != m_mapOnCountPerStation.end(); ++it) {
    //     vector<int> vec = it->second;
    //     std::cout << it->first << ", " << vec.size() << ", " << endl;
    // }

    // Off
    // for (map<string, vector<int> >::iterator it = m_mapOffCountPerStation.begin(); it != m_mapOffCountPerStation.end(); ++it) {
    //     vector<int> vec = it->second;
    //     std::cout << it->first << ", " << vec.size() << ", " << endl;
    // }

    // cout << "stations count: " << m_mapOnCountPerStation.size() << endl;

    // string ie = "Grand Hotel Pacific Hwy near Alison Rd";
    // map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(ie);
    // vector<int> vec = it->second;

    // for (int i = 0; i < vec.size(); i++) {
    //     cout << vec[i] << endl;
    // }
}