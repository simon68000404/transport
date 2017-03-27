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
}

void PrePuncProcessor::setFiles(vector<string> strFileNames) {
    this->m_strInfileNames = strFileNames;
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
    }
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

// bool PrePuncProcessor::isActDprtLaterThanArrv(string fileName) {



//     if (fileName.length() == 0) {
//         cout << "File name is empty.";
//         return false;
//     }

//     ifstream infile;
//     infile.open(fileName.c_str());
//     if (!infile.is_open()) {
//         cout << "File " << fileName << " couldn't be opened.";
//         return false;
//     }

//     string value;
//     string line;

//     int r = 0;
//     struct tm tm = {};
//     while (getline(infile, line)) {
//         int c = 0;
//         stringstream ss(line);
//         string plannedStopNode = "";
//         string actStopNode = "";
//         string destStation = "";
//         string plannedStopStation = "";
//         string actStopStation = "";
//         string actDprtTime = "";
//         string actArrvTime = "";
        
//         while (getline(ss, value, ',')) {
//             if (c == m_plannedStopNodeCol) {
//                 plannedStopNode = value;
//             }
//             else if (c == m_actStopNodeCol) {
//                 actStopNode = value;
//             }
//             else if (c == m_plannedStopStationCol) {
//                 plannedStopStation = value;
//             }
//             else if (c == m_actStopStationCol) {
//                 actStopStation = value;
//             }
//             else if (c == m_destStationCol) {
//                 destStation = value;
//             }
//             else if (c == m_actDprtTimeCol) {
//                 actDprtTime = value.substr(1, value.length() - 2);
//             }
//             else if (c == m_actArrvTimeCol) {
//                 actArrvTime = value.substr(1, value.length() - 2);
//             }
//             // cout << r << " " << c << " " << value << endl;
//             c++;
//         }

//         if (this->compareTimeStringLT(actDprtTime, actArrvTime) == -1) {
//             cout << r << " " << actDprtTime << " " << actArrvTime << endl;
//         }

//         r++;
//     }


//     // struct tm tm = {};
//     // if (strptime("23/08/2016 08:22:00", "%d/%m/%Y %H:%M:%S", &tm) == 0) {
//     //     cout << "Time format not correct." << endl;
//     // }

//     // // time_t t1 = mktime(&tm1);
//     // // time_t t2 = mktime(&tm2);

//     // printf("year: %d; month: %d; day: %d;\n",
//     //         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
//     // printf("hour: %d; minute: %d; second: %d\n",
//     //         tm.tm_hour, tm.tm_min, tm.tm_sec);
//     // printf("week day: %d; year day: %d\n", tm.tm_wday, tm.tm_yday);
// }

// bool PrePuncProcessor::interpolateMissingData(string fileName) {
//     if (fileName.length() == 0) {
//         cout << "File name is empty.";
//         return false;
//     }

//     ifstream infile;
//     infile.open(fileName.c_str());
//     if (!infile.is_open()) {
//         cout << "File " << fileName << " couldn't be opened.";
//         return false;
//     }

//     string value;
//     string line;
    
//     int r = 0;
//     while (getline(infile, line)) {
//         int c = 0;
//         stringstream ss(line);
//         string plannedStopNode = "";
//         string actStopNode = "";
//         string destStation = "";
//         string plannedStopStation = "";
//         string actStopStation = "";
        
//         while (getline(ss, value, ',')) {
//             // switch (c) {
//             //     case m_plannedStopNodeCol:
//             //         plannedStopNode = value;
//             //         break;
//             //     case m_actStopNodeCol:
//             //         actStopNode = value;
//             //         break;
//             //     case m_plannedStopStationCol:
//             //         plannedStopStation = value;
//             //         break;
//             //     case m_actStopStationCol:
//             //         actStopStation = value;
//             //         break;                    
//             //     case m_destStationCol:
//             //         destStation = value;
//             //         break;
//             //     default:
//             // }
//             if (c == m_plannedStopNodeCol) {
//                 plannedStopNode = value;
//             }
//             else if (c == m_actStopNodeCol) {
//                 actStopNode = value;
//             }
//             else if (c == m_plannedStopStationCol) {
//                 plannedStopStation = value;
//             }
//             else if (c == m_actStopStationCol) {
//                 actStopStation = value;
//             }
//             else if (c == m_destStationCol) {
//                 destStation = value;
//             }
//             // cout << r << " " << c << " " << value << endl;
//             c++;
//         }

//         // planned and act match check
//         // if (actStopStation.compare(plannedStopStation) != 0 && actStopStation.compare("\"Missing\"") != 0) {
//         //     cout << plannedStopNode << " " << actStopNode << " " << plannedStopStation << " " << actStopStation << endl;
//         //     // cout << line << endl;
//         // }

//         // 


//         r++;
//         // if (r++ > 3) {
//         //     break;
//         // }
//     }
    
//     return true;
// }
