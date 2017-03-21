#ifndef TRANSPORTAPPLICATION_H
#define TRANSPORTAPPLICATION_H

#include <string>
#include <vector>

class TransportApplication {
public:
	// static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::vector<std::string> strOnOutputCSVNames, std::vector<std::string> strOffOutputCSVNames, std::string strMergedOnOutputCSVName, std::string strMergedOffOutputCSVName);
	static void compareOpalAndRoamPerStationPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOnOutputCSVName, std::string strOffOutputCSVName);
	static void compareOpalAndRoamPerLinePerDay(std::string strOpalInputCSVNames[], std::string strRoamlInputCSVNames[], std::string strOnOutputCSVNames[], std::string strOffOutputCSVNames[], std::string strMergedOutputCSVName);
	static void compareOpalAndRoamPerODPerDay(std::vector<std::string> strOpalInputCSVNames, std::vector<std::string> strRoamInputCSVNames, std::string strOutputCSVName);
};

#endif