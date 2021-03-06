#ifndef TRANSPORTAPPLICATION_H
#define TRANSPORTAPPLICATION_H

#include <string>
#include <vector>
#include <map>

class TransportApplication {
public:
	// static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strOnOutputCSVNames, std::vector<std::string> strOffOutputCSVNames, std::string strMergedOnOutputCSVName, std::string strMergedOffOutputCSVName);
	static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void compareOpalAndRoamPerLinePerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVName, std::map<std::string, std::vector<std::string> > mapStationLines);
	static void compareOpalAndRoamPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVName);

	static void compareOpalAndCvmPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void compareOpalAndCvmPerLinePerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName, std::map<std::string, std::vector<std::string> > mapStationLines);
	static void compareOpalAndCvmPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName);

	static void compareRoamAndCvmPerStationFromPerStopData(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void compareRoamAndCvmInterchangesPerDay(std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strCvmInputCSVNames, std::string strOutputCSVName);

	static void checkCompleteness(std::vector<std::string> strPuncInputCSVNames, std::map<std::string, std::vector<std::string> > mapPerStopInputCSVNames, std::string strOutputCSVName);
	static void checkCompleteness2(std::vector<std::string> strPuncInputCSVNames, std::map<std::string, std::vector<std::string> > mapPerStopInputCSVNames, std::string strOutputCSVName);

	static void comparerRoamPPAndRoamPSPerStationPerDay(std::vector<std::string> strRoamPersonInputCSVNames, std::vector<std::string> strRoamStopInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void comparerCvmPPAndCvmPSPerStationPerDay(std::vector<std::string> strCvmPersonInputCSVNames, std::vector<std::string> strCvmStopInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);

	static void generateOpalExceptions(std::vector<std::string> strOpalInputCSVNames, std::string strOutputCSVBaseName);
	static void generateRoamExceptions(std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVBaseName);
	static void generatePuncExceptions(std::vector<std::string> strPuncInputCSVNames, std::string strOutputCSVBaseName);

	static void generateAllLines(std::string strPuncFileName, std::string strOutputCSVNameBaseName);

	static std::map<std::string, std::vector<std::string> > importAllStationLines(std::string strAllLinesFileName);
	static std::map<std::string, std::vector<std::string> > importAllStationLinesOneToOne(std::string strAllLinesFileName, unsigned int iStationCol, unsigned int iLineCol);
};

#endif