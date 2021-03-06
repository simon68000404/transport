#include "PerStopDataAnalyser.h"
#include "../PrePuncProcessor/PrePuncProcessor.h"

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

void PerStopDataAnalyser::updateTripNameRows(std::string strTripName, unsigned int iRow) {
    map<string, vector<unsigned int> >::iterator it = m_mapTripNameRows.find(strTripName);
    if (it == m_mapTripNameRows.end()) {
        // cout << "doesn't have" << strOnStopName << endl;
        vector<unsigned int> vec;
        vec.push_back(iRow);

        m_mapTripNameRows.insert(pair<string, vector<unsigned int> >(strTripName, vec));
    }
    else {
        it->second.push_back(iRow);
    }
}

std::map<std::string, std::vector<unsigned int> > PerStopDataAnalyser::getTripNameRows() {
    return m_mapTripNameRows;
}

std::vector<TrainTripStop> PerStopDataAnalyser::getTripStops() {
    return m_vecTripStops;
}

RoamPerStopResultAnalyser::RoamPerStopResultAnalyser() {
	m_iThisStationCol = 0;
	m_iUnboardingCountCol = 27;
    m_iBoardingCountCol = 28;

    m_iServiceDateCol = 2;
    m_iTripNameCol = 8;

    m_iArrivalTime = 3;

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

        cout << "Analysing " << m_strInfileNames[i] << endl;

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

                c++;
            }

            // this stop station
            getline(ss, value, m_cDivider);
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
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
            	getline(ss, value, m_cDivider);

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
                c++;
            }

            updatePerStationCount(strThisStopStation, nBoardingCount, nUnboardingCount);

            r++;
        }

        m_nTotalRows += r;

    	infile.close();
    }   
}

void RoamPerStopResultAnalyser::extractTripNameRows() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing " << m_strInfileNames[i] << endl;

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strServiceDate = "";
            string strTripName = "";

            while(c < m_iServiceDateCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    

            // Service Date in 2016-08-01
            getline(ss, value, m_cDivider);
            strServiceDate = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iTripNameCol) {
                getline(ss, value, m_cDivider);

                c++;
            }

            // Trip Name
            getline(ss, value, m_cDivider);
            strTripName = value.substr(1, value.length() - 2); // to remove the postfix "-JOINED"
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                c++;
            }

            updateTripNameRows(strServiceDate + "_" + strTripName, r);

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

    m_iServiceDateCol = 4;
    m_iTripNameCol = 1;

    m_iArrivalTime = 5;

    m_cDivider = ',';
}

void CvmPerStopResultAnalyser::calculatePerStationCount() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing aaaaaaaaaaa" << m_strInfileNames[i] << endl;

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

                c++;
            }
            // this stop station
            getline(ss, value, m_cDivider);
            strThisStopStation = value.substr(1, value.length() - 2);;
            c++;

            while(c < m_iUnboardingCountCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // unboard count
            getline(ss, value, m_cDivider);
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
                getline(ss, value, m_cDivider);

                c++;
            }

            // board count
            getline(ss, value, m_cDivider);
            nBoardingCount = atoi(value.c_str());
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                c++;
            }

            updatePerStationCount(strThisStopStation, nBoardingCount, nUnboardingCount);

            //             if (strThisStopStation == "Mindaribba") {
            //     cout << line << endl;
            // }

            r++;
        }

        m_nTotalRows += r;

        infile.close();
    }   
}

void CvmPerStopResultAnalyser::extractTripNameRows() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing " << m_strInfileNames[i] << endl;

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strServiceDate = "";
            string strTripName = "";

            while(c < m_iTripNameCol) {
                getline(ss, value, m_cDivider);

                c++;
            }

            // Trip Name
            getline(ss, value, m_cDivider);
            strTripName = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iServiceDateCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    

            // Service Date in 2016-08-01
            getline(ss, value, m_cDivider);
            strServiceDate = value.substr(1, value.length() - 2);
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                c++;
            }

            updateTripNameRows(strServiceDate + "_" + strTripName, r);

            r++;
        }

        m_nTotalRows += r;

        infile.close();
    }
}
void CvmPerStopResultAnalyser::calculatePerLineCount() {

}

void PerStopDataAnalyser::updateTripStops(TrainTripStop tripStop) {
    // cout << tripStop.m_strServiceDate << " " << tripStop.m_strTripID << " " << tripStop.m_strStationName << " " << tripStop.m_strDateTime << endl;
    m_vecTripStops.push_back(tripStop);
}

void RoamPerStopResultAnalyser::extractTripStops() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing " << m_strInfileNames[i] << endl;

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strStationName = "";
            string strServiceDate = "";
            string strArrivalTime = "";
            string strTripName = "";

            unsigned int nUnboardingCount = 0;
            unsigned int nBoardingCount = 0;

            while(c < m_iThisStationCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // This station
            getline(ss, value, m_cDivider);
            strStationName = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iServiceDateCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // Service Date in 2016-08-01
            getline(ss, value, m_cDivider);
            strServiceDate = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iArrivalTime) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // Arrival Time
            getline(ss, value, m_cDivider);
            strArrivalTime = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iTripNameCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Trip Name
            getline(ss, value, m_cDivider);
            strTripName = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iUnboardingCountCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Unboarding count
            getline(ss, value, m_cDivider);
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Boarding count
            getline(ss, value, m_cDivider);
            nBoardingCount = atoi(value.c_str());
            c++;
            
            while (getline(ss, value, m_cDivider)) {
                c++;
            }

            TrainTripStop tripStop(strServiceDate, strTripName, strStationName, strStationName, strArrivalTime, nUnboardingCount, nBoardingCount);
            updateTripStops(tripStop);

            r++;
        }

        m_nTotalRows += r;

        infile.close();
    }
}

void CvmPerStopResultAnalyser::extractTripStops() {
    ifstream infile;
    for (int i = 0; i < m_strInfileNames.size(); i++) {
        infile.open(m_strInfileNames[i].c_str());
        if (!infile.is_open()) {
            cout << "File " << m_strInfileNames[i] << " couldn't be opened." << endl;
            return;
        }

        cout << "Analysing " << m_strInfileNames[i] << endl;

        string value;
        string line;

        int r = 1; // Avoid first row 
        struct tm tm = {};
        getline(infile, line);
        while (getline(infile, line)) {
            int c = 0;
            stringstream ss(line);

            string strStationName = "";
            string strServiceDate = "";
            string strArrivalTime = "";
            string strTripName = "";

            unsigned int nUnboardingCount = 0;
            unsigned int nBoardingCount = 0;

            while(c < m_iTripNameCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Trip Name
            getline(ss, value, m_cDivider);
            strTripName = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iThisStationCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // This station
            getline(ss, value, m_cDivider);
            strStationName = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iServiceDateCol) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // Service Date in 2016-08-01
            getline(ss, value, m_cDivider);
            strServiceDate = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iArrivalTime) {
                getline(ss, value, m_cDivider);
                c++;
            }    
            // Arrival Time
            getline(ss, value, m_cDivider);
            strArrivalTime = value.substr(1, value.length() - 2);
            c++;

            while(c < m_iUnboardingCountCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Unboarding count
            getline(ss, value, m_cDivider);
            nUnboardingCount = atoi(value.c_str());
            c++;

            while(c < m_iBoardingCountCol) {
                getline(ss, value, m_cDivider);

                c++;
            }
            // Boarding count
            getline(ss, value, m_cDivider);
            nBoardingCount = atoi(value.c_str());
            c++;

            while (getline(ss, value, m_cDivider)) {
                c++;
            }

            TrainTripStop tripStop(strServiceDate, strTripName, strStationName, strStationName, strArrivalTime, nUnboardingCount, nBoardingCount);
                // cout << tripStop.m_strServiceDate << " " << tripStop.m_strTripID << " " << tripStop.m_strStationName << " " << tripStop.m_strDateTime << endl;
            updateTripStops(tripStop);
            r++;
        }

        m_nTotalRows += r;

        infile.close();
    }
}