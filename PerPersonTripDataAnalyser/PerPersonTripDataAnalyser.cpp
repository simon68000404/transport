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

        updateCount(onStopName, offStopName, r);
        // cout << onStopName << " " << offStopName << endl;

        r++;
        // if (r == 1000) {
        //     break;
        // }
    }

	infile.close();    

    for (map<string, vector<int> >::iterator it = m_mapOnCountPerStation.begin(); it != m_mapOnCountPerStation.end(); ++it) {
        vector<int> vec = it->second;
        std::cout << it->first << " => " << vec.size() << endl;
    }

    cout << "stations count: " << m_mapOnCountPerStation.size() << endl;

    // string ie = "Grand Hotel Pacific Hwy near Alison Rd";
    // map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(ie);
    // vector<int> vec = it->second;

    // for (int i = 0; i < vec.size(); i++) {
    //     cout << vec[i] << endl;
    // }
}

void PerPersonTripDataAnalyser::updateCount(std::string strOnStopName, std::string strOffStopName, int nRow) {
	map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(strOnStopName);
	if (it == m_mapOnCountPerStation.end()) {
        cout << "doesn't have" << strOnStopName << endl;
        vector<int> vecOn;
        vector<int> vecOff;
        vecOn.push_back(nRow);
        vecOff.push_back(nRow);

		m_mapOnCountPerStation.insert(pair<string, vector<int> >(strOnStopName, vecOn));
		m_mapOffCountPerStation.insert(pair<string, vector<int> >(strOffStopName, vecOff));
	}
    else {
        it->second.push_back(nRow);
    }
}

RoamResultAnalyser::RoamResultAnalyser() {
    m_iOnStopFirstTripCol = 12;
    m_iOffStopLastTripCol = 22;
}

void RoamResultAnalyser::calculatePerStationCount() {
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

        while(c < m_iOffStopLastTripCol) {
            getline(ss, value, ',');
            c++;
        }    

        // off stop
        getline(ss, value, ',');
        offStopName = value;
        c++;
        
        while (getline(ss, value, ',')) {
            // cout << r << " " << c << " " << value << endl;
            c++;
        }

        updateCount(onStopName, offStopName, r);
        // cout << onStopName << " " << offStopName << endl;

        r++;
        // if (r == 1000) {
        //     break;
        // }
    }

    infile.close();    

    for (map<string, vector<int> >::iterator it = m_mapOnCountPerStation.begin(); it != m_mapOnCountPerStation.end(); ++it) {
        vector<int> vec = it->second;
        std::cout << it->first << " => " << vec.size() << endl;
    }

    cout << "stations count: " << m_mapOnCountPerStation.size() << endl;

    // string ie = "Grand Hotel Pacific Hwy near Alison Rd";
    // map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(ie);
    // vector<int> vec = it->second;

    // for (int i = 0; i < vec.size(); i++) {
    //     cout << vec[i] << endl;
    // }
}