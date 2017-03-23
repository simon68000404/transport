#ifndef PREPUNCPROCESSOR_H
#define PREPUNCPROCESSOR_H 

#include <string>
#include <set>

class Line {
public:
    std::string strType;
    std::string strName;
    std::set<std::string> strStationNames;
};

class PrePuncProcessor {
public:
    PrePuncProcessor(int tripNameCol, 
    	int plannedStopNodeCol, 
    	int actStopNodeCol,
    	int plannedStopStationCol,
    	int actStopStationCol,
    	int destStationCol, 
    	int plannedDprtTimeCol, 
    	int plannedArrvTimeCol, 
    	int actDprtTimeCol, 
    	int actArrvTimeCol);
    bool isActDprtLaterThanArrv(std::string fileName);
    bool doesActStationMatchPlanned();
    bool correctUnknownActStation();
    bool interpolateMissingData(std::string fileName);
    
private:
    int compareTimeStringLT(std::string t1, std::string t2);
    int m_plannedDprtTimeCol;
    int m_plannedArrvTimeCol;
    int m_actDprtTimeCol;
    int m_actArrvTimeCol;

    int m_plannedStopNodeCol;
    int m_actStopNodeCol;

    int m_plannedStopStationCol;
    int m_actStopStationCol;

    int m_tripNameCol;
    int m_destStationCol;
};

#endif
