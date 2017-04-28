#ifndef PREPUNCPROCESSOR_H
#define PREPUNCPROCESSOR_H 

#include <string>
#include <set>
#include <vector>

class Line {
public:
    std::string strType;
    std::string strName;
    std::set<std::string> strStationNames;
};

class TrainTripStop {
public:
    std::string strServiceDate; // "2016-09-01"
    std::string strTripID; // "204S"
    std::string strStationName; // "Central"
    std::string strDateTime; // 2016-09-01 or Missing
};

// class TrainTrip {
// public:
//     std::vector<TrainStop> vecTrainStops; 
// };

class PrePuncProcessor {
public:
    // PrePuncProcessor(int tripNameCol, 
    // 	int plannedStopNodeCol, 
    // 	int actStopNodeCol,
    // 	int plannedStopStationCol,
    // 	int actStopStationCol,
    // 	int destStationCol, 
    // 	int plannedDprtTimeCol, 
    // 	int plannedArrvTimeCol, 
    // 	int actDprtTimeCol, 
    // 	int actArrvTimeCol);
    PrePuncProcessor();

    void setFiles(std::vector<std::string> strFileNames);

    void extractTripNames();
    std::vector<std::string> getTripNames();

    void extractTripStops();
    std::vector<TrainTripStop> & getTripStops();

    bool isActDprtLaterThanArrv(std::string fileName);
    bool doesActStationMatchPlanned();
    bool correctUnknownActStation();
    bool interpolateMissingData(std::string fileName);

    std::vector<std::pair<int, std::string> > getExceptionUnknownStations();
    std::vector<std::pair<int, std::string> > getExceptionMissing();
    std::vector<std::pair<int, std::string> > getExceptionDprtLTArrv();

    void calculateExceptions();

    unsigned int getTotalRowCount();
    
private:
    void updateExceptionUnknownStations(int nRow, std::string strThatRow);
    void updateExceptionMissing(int nRow, std::string strThatRow);
    void updateExceptionDprtLTArrv(int nRow, std::string strThatRow);

    void updateTripNames(std::string strTripName);
    void updateTripStops(std::string strServiceDate, std::string strTripName, std::string strStationName, std::string strArrivalTime);

    int compareTimeStringLT(std::string t1, std::string t2);
    int m_iPlannedDprtTimeCol;
    int m_iPlannedArrvTimeCol;
    int m_iActDprtTimeCol;
    int m_iActArrvTimeCol;

    int m_iPlannedStopNodeCol;
    int m_iActStopNodeCol;

    int m_iPlannedStopStationCol;
    int m_iActStopStationCol;

    int m_iServiceDateCol;
    int m_iTripNameCol;
    int m_iDestStationCol;
    int m_iServiceTypeCol;
    int m_iServiceLineCol;

    std::vector<std::string> m_strInfileNames;

    std::vector<std::pair<int, std::string> > m_vecExceptionUnknownStation;
    std::vector<std::pair<int, std::string> > m_vecExceptionMissing;
    std::vector<std::pair<int, std::string> > m_vecExceptionDprtLTArrv;

    std::set<std::string> m_setTripNames;

    unsigned int m_nTotalRows;

    std::vector<TrainTripStop> m_vecAllTripStops;
};

#endif
