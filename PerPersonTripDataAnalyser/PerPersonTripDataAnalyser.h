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
	PerPersonTripDataAnalyser();
	bool setFiles(std::vector<std::string> strFileNames);
	void setStationLines(std::map<std::string, std::vector<std::string> > mapStationLines);
	void calculatePerStationCount();
	void calculatePerODCount();
	void calculatePerLineCount();
	void calculateExceptions();
	std::map<std::string, std::vector<int> > getOnPerStationCount();
	std::map<std::string, std::vector<int> > getOffPerStationCount();
	std::map<std::string, std::vector<int> > getPerODCount();
	std::map<std::string, std::vector<int> > getPerLineCount();
	int getTotalRowCount();
	// std::map<std::string, std::vector<int> > getExceptions();
protected:
	void updatePerStationCount(std::string onStopName, std::string offStopName, int row);
	void updatePerODCount(std::string onStopName, std::string offStopName, int row);
	void updatePerLineCount(std::string strOnStopName, std::string strOffStopName, int nRow);
protected:
	std::map<std::string, std::vector<int> > m_mapOnCountPerStation;
	std::map<std::string, std::vector<int> > m_mapOffCountPerStation;
	std::map<std::string, std::vector<int> > m_mapCountPerOD;
	std::map<std::string, std::vector<int> > m_mapCountPerLine;

	std::vector<std::string> m_strInfileNames;

	std::map<std::string, std::vector<std::string> > m_mapStationLines;

	int m_nTotalRows;
	
	std::vector<std::string> m_vecStationsToSkip;

	// std::map<std::string, std::vector<int> > m_mapExceptions;
};

class OpalTripAnalyser: public PerPersonTripDataAnalyser {
public:
	OpalTripAnalyser();
	void calculatePerStationCount();
	void calculatePerODCount();
	void calculatePerLineCount();
	void calculateExceptions();

	std::vector<int> getUnknownOnCount();
	std::vector<int> getUnknownOffCount();
	std::vector<int> getSameOnOffCount();
	// std::map<std::string, std::vector<int> > getExceptions();
private:
	void updateUnknownOnCount(int nRow);
	void updateUnknownOffCount(int nRow);
	void updateSameOnOffCount(int nRow);

	bool shouldSkipOpalRecord(std::string onStopName, std::string offStopName);

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
	void calculatePerStationCountWithTransfers();
	void calculatePerODCount();
	void calculatePerLineCount();
	void calculateExceptions();

	std::vector<int> getMT2TripsCount();
	// std::map<std::string, std::vector<int> > getExceptions();
private:
	void updateMT2TripsCount(int nRow);

	int m_iOriginStopCol;
	int m_iDestStopCol;
	int m_iTransferStationCol;
	int m_iFirstTripLineCol;

	std::vector<int> m_vecMT2TripsCount;

	// std::string m_strExceptionNotAbleToFindPath;
};

#endif