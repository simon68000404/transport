#ifndef PERSTOPDATAANALYSER_H
#define PERSTOPDATAANALYSER_H

#include "../PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"

#include <string>

class PerStopDataAnalyser: public PerPersonTripDataAnalyser {
public:
protected:
	void updatePerStationCount(std::string strThisStopStation, unsigned int nBoardingCount, unsigned int nUnboardingCount);
	unsigned int m_iThisStationCol;
	unsigned int m_iBoardingCountCol;
	unsigned int m_iUnboardingCountCol;

	char m_cDivider;
};

class RoamPerStopResultAnalyser: public PerStopDataAnalyser {
public:
	RoamPerStopResultAnalyser();
	void calculatePerStationCount();
	void calculatePerLineCount();

protected:

private:
	// std::string m_strExceptionNotAbleToFindPath;
};

class CvmPerStopResultAnalyser: public PerStopDataAnalyser {
public:
	CvmPerStopResultAnalyser();
	void calculatePerStationCount();
	void calculatePerLineCount();

protected:

private:
	// std::string m_strExceptionNotAbleToFindPath;
};

#endif