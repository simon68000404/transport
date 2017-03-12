#include "PrePuncProcessor/PrePuncProcessor.h"
#include "PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	string puncFile = "/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv";
	string opalFile = "/media/nlp/Maxtor/Transport/Opal/JS_ALL_V.20160802_20160817.csvp";
	PrePuncProcessor test(3, 14, 18, 15, 19, 8, 16, 17, 20, 21);
    // bool succeeded = test.handle("/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv");
    // cout << succeeded << endl;
    test.isActDprtLaterThanArrv("/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv");
    // OpalTripAnalyser opal;
    // opal.setFile(opalFile);
    // opal.calculatePerStationCount();
}
