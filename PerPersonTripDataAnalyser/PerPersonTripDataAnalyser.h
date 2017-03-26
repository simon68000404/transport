#ifndef PERPERSONTRIPDATAANALYSER_H
#define PERPERSONTRIPDATAANALYSER_H

#include <fstream>
#include <map>
#include <vector>
class PersonTrip {
public:
	enum TRIP_TYPE {
		TRIP_TYPE_TRAIN = 0,
		TRIP_TYPE_BUS,
		TRIP_TYPE_FERRY
	};

	std::string cardFK;
	TRIP_TYPE tripType;
	time_t OnTime;
	time_t OffTime;
	std::string OnStopName;
	std::string OffStopName;
	int onLocFK;
	int offLocFK;
};

class PerPersonTripDataAnalyser {
public:
	// PerPersonTripDataAnalyser();
	bool setFiles(std::vector<std::string> strFileNames);
	void calculatePerStationCount();
	void calculatePerODCount();
	void calculateExceptions();
	std::map<std::string, std::vector<int> > getOnPerStationCount();
	std::map<std::string, std::vector<int> > getOffPerStationCount();
	std::map<std::string, std::vector<int> > getPerODCount();
	int getTotalRowCount();
	// std::map<std::string, std::vector<int> > getExceptions();
protected:
	void updatePerStationCount(std::string onStopName, std::string offStopName, int row);
	void updatePerODCount(std::string onStopName, std::string offStopName, int row);
protected:
	std::map<std::string, std::vector<int> > m_mapOnCountPerStation;
	std::map<std::string, std::vector<int> > m_mapOffCountPerStation;
	std::map<std::string, std::vector<int> > m_mapCountPerOD;

	std::vector<std::string> m_strInfileNames;

	int m_nTotalRows;

	// std::map<std::string, std::vector<int> > m_mapExceptions;
};

class OpalTripAnalyser: public PerPersonTripDataAnalyser {
public:
	OpalTripAnalyser();
	void calculatePerStationCount();
	void calculatePerODCount();
	void calculateExceptions();

	std::vector<int> getUnknownOnCount();
	std::vector<int> getUnknownOffCount();
	std::vector<int> getSameOnOffCount();
	// std::map<std::string, std::vector<int> > getExceptions();
private:
	void updateUnknownOnCount(int row);
	void updateUnknownOffCount(int row);
	void updateSameOnOffCount(int row);

	int m_iOnDateCol;
	int m_iOnTimeCol;
	int m_iOffDateCol;
	int m_iOffTimeCol;
	int m_iOnStopNameCol;
	int m_iOffStopNameCol;
	int m_iTripTypeCol;

	std::vector<int> m_vecUnknownTapOnCount;
	std::vector<int> m_vecUnknownTapOffCount;

	std::vector<int> m_vecSameOnOffCount;

	// std::string m_strExceptionNoTapOn;
	// std::string m_strExceptionNoTapOff;
};

class RoamResultAnalyser: public PerPersonTripDataAnalyser {
public:
	RoamResultAnalyser();
	void calculatePerStationCount();
	void calculatePerODCount();
	void calculateExceptions();
	// std::map<std::string, std::vector<int> > getExceptions();
private:
	int m_iOriginStopCol;
	int m_iDestStopCol;
	int m_iFirstTripLineCol;

	// std::string m_strExceptionNotAbleToFindPath;
};

#endif