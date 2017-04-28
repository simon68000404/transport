#ifndef PERSTOPDATAANALYSER_H
#define PERSTOPDATAANALYSER_H

#include "../PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"

#include <string>
#include <map>

class TrainTripStop;

class PerStopDataAnalyser: public PerPersonTripDataAnalyser {
public:
	std::map<std::string, std::vector<unsigned int> > getTripNameRows();
	void extractTripNameRows();
protected:
	void updatePerStationCount(std::string strThisStopStation, unsigned int nBoardingCount, unsigned int nUnboardingCount);
	void updateTripNameRows(std::string strTripName, unsigned int nRow);

	unsigned int m_iThisStationCol;
	unsigned int m_iBoardingCountCol;
	unsigned int m_iUnboardingCountCol;

	unsigned int m_iServiceDateCol;
	unsigned int m_iTripNameCol;

	char m_cDivider;

	std::map<std::string, std::vector<unsigned int> > m_mapTripNameRows;

	std::map<TrainTripStop, std::vector<unsigned int> > m_mapTripStopRows;
};

class RoamPerStopResultAnalyser: public PerStopDataAnalyser {
public:
	RoamPerStopResultAnalyser();
	void calculatePerStationCount();
	void calculatePerLineCount();

	void extractTripNameRows();

protected:

private:
	// std::string m_strExceptionNotAbleToFindPath;
};

class CvmPerStopResultAnalyser: public PerStopDataAnalyser {
public:
	CvmPerStopResultAnalyser();
	void calculatePerStationCount();
	void calculatePerLineCount();

	void extractTripNameRows();

protected:

private:
	// std::string m_strExceptionNotAbleToFindPath;
};

#endif