#include "PerStopDataAnalyser.h"

#include <iostream>
#include <sstream>

using namespace std;

void PerStopDataAnalyser::updatePerStationCount(std::string strThisStopStation, unsigned int nBoardingCount, unsigned int nUnboardingCount) {
    map<string, vector<int> >::iterator it = m_mapOnCountPerStation.find(strThisStopStation);
    if (it == m_mapOnCountPerStation.end()) {
        // cout << "doesn't have" << strOnStopName << endl;
        vector<int> vecOn;
        vecOn.push_back(nBoardingCount);

        m_mapOnCountPerStation.insert(pair<string, vector<int> >(strThisStopStation, vecOn));
    }
    else {
        it->second.push_back(nBoardingCount);
    }

    map<string, vector<int> >::iterator it2 = m_mapOffCountPerStation.find(strThisStopStation);
    if (it2 == m_mapOffCountPerStation.end()) {
        // cout << "doesn't have" << strOnStopName << endl;
        vector<int> vecOff;
        vecOff.push_back(nUnboardingCount);

        m_mapOffCountPerStation.insert(pair<string, vector<int> >(strThisStopStation, vecOff));
    }
    else {
        it2->second.push_back(nUnboardingCount); 
    }
}

RoamPerStopResultAnalyser::RoamPerStopResultAnalyser() {
	m_iThisStationCol = 0;
	m_iUnboardingCountCol = 27;
    m_iBoardingCountCol = 28;

    m_cDivider = ',';
}

void RoamPerStopResultAnalyser::calculatePerStationCount() {
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

            string strThisStopStation = "";
            unsigned int nBoardingCount = 0;
            unsigned int nUnboardingCount = 0;

            while(c < m_iThisStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // this stop station
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strThisStopStation = value.substr(1, value.length() - 2);;
            c++;

            while(c < m_iUnboardingCountCol) {
                getline(ss, value, m_cDivider);
                if (value.substr(0, 15) == "\"Missing stops:") {
                    getline(ss, value, '"');
                    getline(ss, value, m_cDivider);
                }
                c++;
            }    

            // unboard count
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
            	getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
            	c++;
            }

            // board count
            getline(ss, value, m_cDivider);
            // if (strThisStopStation == "Coniston") {
            //     cout << value << m_cDivider;
            // }
            nBoardingCount = atoi(value.c_str());
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            updatePerStationCount(strThisStopStation, nBoardingCount, nUnboardingCount);

            r++;
        }

        m_nTotalRows += r;

    	infile.close();
    }   
}

void RoamPerStopResultAnalyser::calculatePerLineCount() {

}

CvmPerStopResultAnalyser::CvmPerStopResultAnalyser() {
    m_iThisStationCol = 3; // station_nm
    m_iUnboardingCountCol = 11; // unboarding_psngr_cnt
    m_iBoardingCountCol = 12; // boarding_psngr_cnt

    m_cDivider = ';';
}

void CvmPerStopResultAnalyser::calculatePerStationCount() {
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

            string strThisStopStation = "";
            unsigned int nBoardingCount = 0;
            unsigned int nUnboardingCount = 0;

            while(c < m_iThisStationCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // this stop station
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            strThisStopStation = value.substr(1, value.length() - 2);;
            c++;

            while(c < m_iUnboardingCountCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    

            // unboard count
            getline(ss, value, m_cDivider);
            // cout << c << " " << value << endl;
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
                getline(ss, value, m_cDivider);
                // cout << c << " " << value << endl;
                c++;
            }

            // board count
            getline(ss, value, m_cDivider);
            nBoardingCount = atoi(value.c_str());
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            updatePerStationCount(strThisStopStation, nBoardingCount, nUnboardingCount);

            r++;
        }

        m_nTotalRows += r;

        infile.close();
    }   
}

void CvmPerStopResultAnalyser::calculatePerLineCount() {

}