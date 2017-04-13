#include "PrePuncProcessor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

// PrePuncProcessor::PrePuncProcessor(int tripNameCol, 
//         int plannedStopNodeCol, 
//         int actStopNodeCol,
//         int plannedStopStationCol,
//         int actStopStationCol,
//         int destStationCol, 
//         int plannedDprtTimeCol, 
//         int plannedArrvTimeCol, 
//         int actDprtTimeCol, 
//         int actArrvTimeCol) {
//     m_tripNameCol = tripNameCol;
//     m_plannedStopNodeCol = plannedStopNodeCol;
//     m_actStopNodeCol = actStopNodeCol;

//     m_plannedStopStationCol = plannedStopStationCol;
//     m_actStopStationCol = actStopStationCol;

//     m_destStationCol = destStationCol;

//     m_plannedDprtTimeCol = plannedDprtTimeCol;
//     m_plannedArrvTimeCol = plannedArrvTimeCol;
//     m_actDprtTimeCol = actDprtTimeCol;
//     m_actArrvTimeCol = actArrvTimeCol;
// }

PrePuncProcessor::PrePuncProcessor() {
    m_iServiceDateCol = 1;
    m_iTripNameCol = 3;
    m_iDestStationCol = 8;
    m_iServiceTypeCol = 4;
    m_iServiceLineCol = 5;

    m_iPlannedStopNodeCol = 14;
    m_iPlannedStopStationCol = 15;
    
    m_iPlannedDprtTimeCol = 16;
    m_iPlannedArrvTimeCol = 17;    
    
    m_iActStopNodeCol = 18;
    m_iActStopStationCol = 19;

    m_iActDprtTimeCol = 20;
    m_iActArrvTimeCol = 21;

    m_nTotalRows = 0;
}

void PrePuncProcessor::setFiles(vector<string> strFileNames) {
    this->m_strInfileNames = strFileNames;
}

void PrePuncProcessor::updateTripNames(std::string strTripName) {
    m_setTripNames.insert(strTripName);
}

void PrePuncProcessor::extractTripNames() {
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

            string strServiceDate;
            string strTripName;

            string strDay;
            string strMonth;
            string strYear;

            while(c < m_iServiceDateCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strServiceDate = value.substr(1, value.length() - 2);
            // cout << strServiceDate << " ";

            stringstream ssServiceDate(strServiceDate);
            getline(ssServiceDate, strDay, '/');
            getline(ssServiceDate, strMonth, '/');
            getline(ssServiceDate, strYear);
            strServiceDate = strYear + '-' + strMonth + '-' + strDay;

            // cout << strServiceDate << endl;
            // strServiceDate = // 01/08/2016 to 2016-08-01
            c++;

            while(c < m_iTripNameCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strTripName = value.substr(1, value.length() - 2);
            c++;
            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            updateTripNames(strServiceDate + "_" + strTripName);

            r++;
        }

        infile.close();
    }
}

std::vector<std::string> PrePuncProcessor::getTripNames() {
    vector<string> vecAllTripNames;
    for (set<string>::iterator it = m_setTripNames.begin(); it != m_setTripNames.end(); it++) {
        vecAllTripNames.push_back(*it);
    }    

    return vecAllTripNames;
}

void PrePuncProcessor::calculateExceptions() {
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

            string strActualStopStation;
            string strActualDprtTime;
            string strActualArrvTime;

            while(c < m_iActStopStationCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strActualStopStation = value;
            c++;

            while(c < m_iActDprtTimeCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strActualDprtTime = value;
            c++;

            while(c < m_iActArrvTimeCol) {
                getline(ss, value, ',');
                // cout << c << " " << value << endl;
                c++;
            }
            getline(ss, value, ',');
            // cout << c << " " << value << endl;
            strActualArrvTime = value;
            c++;
            
            while (getline(ss, value, ',')) {
                // cout << r << " " << c << " " << value << endl;
                c++;
            }

            if (strActualStopStation == "\"Missing\"") {
                updateExceptionMissing(r, line);
            }
            else if (strActualStopStation == "\"Unknown\"") {
                updateExceptionUnknownStations(r, line);
            }
            else if (this->compareTimeStringLT(strActualDprtTime.substr(1, strActualDprtTime.length() - 2), strActualArrvTime.substr(1, strActualDprtTime.length() - 2)) == -1) {
                updateExceptionDprtLTArrv(r, line);
            }

            r++;
        }

        infile.close();

        m_nTotalRows += (r - 1);
    }
}

unsigned int PrePuncProcessor::getTotalRowCount() {
    return m_nTotalRows;
}

vector<pair<int, string> > PrePuncProcessor::getExceptionUnknownStations() {
    return m_vecExceptionUnknownStation;
}
vector<pair<int, string> > PrePuncProcessor::getExceptionMissing() {
    return m_vecExceptionMissing;
}
vector<pair<int, string> > PrePuncProcessor::getExceptionDprtLTArrv() {
    return m_vecExceptionDprtLTArrv;
}

void PrePuncProcessor::updateExceptionUnknownStations(int nRow, std::string strThatRow) {
    m_vecExceptionUnknownStation.push_back(pair<int, string>(nRow, strThatRow));
}
void PrePuncProcessor::updateExceptionMissing(int nRow, std::string strThatRow) {
    m_vecExceptionMissing.push_back(pair<int, string>(nRow, strThatRow));
}
void PrePuncProcessor::updateExceptionDprtLTArrv(int nRow, std::string strThatRow) {
    m_vecExceptionDprtLTArrv.push_back(pair<int, string>(nRow, strThatRow));
}

int PrePuncProcessor::compareTimeStringLT(string strT1, string strT2) {
    struct tm tm1 = {};
    struct tm tm2 = {};
    time_t t1 = 0;
    time_t t2 = 0;
    if (strptime(strT1.c_str(), "%d/%m/%Y %H:%M:%S", &tm1) == NULL) {
        return -2;
    }
    if (strptime(strT2.c_str(), "%d/%m/%Y %H:%M:%S", &tm2) == NULL) {
        return -2;
    }

    t1 = mktime(&tm1);
    t2 = mktime(&tm2);

    return difftime(t1,t2) >= 0.0 ? 1 : -1; 
}
