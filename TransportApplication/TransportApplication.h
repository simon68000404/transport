#ifndef TRANSPORTAPPLICATION_H
#define TRANSPORTAPPLICATION_H

#include <string>
#include <vector>
#include <map>

class TransportApplication {
public:
	// static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strOnOutputCSVNames, std::vector<std::string> strOffOutputCSVNames, std::string strMergedOnOutputCSVName, std::string strMergedOffOutputCSVName);
	static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void compareOpalAndRoamPerLinePerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVName, std::map<std::string, std::vector<std::string> > mapLineStations);
	static void compareOpalAndRoamPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVName);

	static void compareRoamAndCvmPerStationFromPerStopData(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);


	static void comparerRoamPPAndRoamPSPerStationPerDay(std::vector<std::string> strRoamPersonInputCSVNames, std::vector<std::string> strRoamStopInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);

	static void generateOpalExceptions(std::vector<std::string> strOpalInputCSVNames, std::string strOutputCSVBaseName);
	static void generateRoamExceptions(std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVBaseName);
	static void generatePuncExceptions(std::vector<std::string> strPuncInputCSVNames, std::string strOutputCSVBaseName);

	static void generateAllLines(std::string strPuncFileName, std::string strOutputCSVNameBaseName);

	static std::map<std::string, std::vector<std::string> > importAllStationLines(std::string strAllLinesFileName);
};

#endif