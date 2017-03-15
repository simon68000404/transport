#include "PrePuncProcessor/PrePuncProcessor.h"
#include "PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
	string puncFile = "/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv";
	string opalFile = "/media/nlp/Maxtor/Transport/Opal/JS_ALL_V.20160801_20160816.csvp";
	string roamFile = "/media/nlp/Maxtor/Transport/Roam/Person/FINAL_MERGE_TABLE-01-08-2016_matched.csv";

    string opalFileNames[] = {
        "JS_ALL_V.20160801_20160816.csvp",
        "JS_ALL_V.20160802_20160817.csvp",
        "JS_ALL_V.20160803_20160818.csvp",
        "JS_ALL_V.20160804_20160819.csvp",
        "JS_ALL_V.20160805_20160820.csvp",
        "JS_ALL_V.20160806_20160821.csvp",
        "JS_ALL_V.20160807_20160822.csvp",
        "JS_ALL_V.20160808_20160823.csvp",
        "JS_ALL_V.20160809_20160824.csvp",
        "JS_ALL_V.20160810_20160825.csvp",
        "JS_ALL_V.20160811_20160826.csvp",
        "JS_ALL_V.20160812_20160827.csvp",
        "JS_ALL_V.20160813_20160828.csvp",
        "JS_ALL_V.20160814_20160829.csvp",
        "JS_ALL_V.20160815_20160830.csvp",
        "JS_ALL_V.20160816_20160831.csvp",
        "JS_ALL_V.20160817_20160901.csvp",
        "JS_ALL_V.20160818_20160902.csvp",
        "JS_ALL_V.20160819_20160903.csvp",
        "JS_ALL_V.20160820_20160904.csvp",
        "JS_ALL_V.20160821_20160916.csvp",
        "JS_ALL_V.20160822_20160906.csvp",
        "JS_ALL_V.20160823_20160916.csvp",
        "JS_ALL_V.20160824_20160908.csvp",
        "JS_ALL_V.20160825_20160909.csvp",
        "JS_ALL_V.20160826_20160916.csvp",
        "JS_ALL_V.20160827_20160911.csvp",
        "JS_ALL_V.20160828_20160912.csvp",
        "JS_ALL_V.20160829_20160916.csvp",
        "JS_ALL_V.20160830_20160914.csvp",
        "JS_ALL_V.20160831_20160915.csvp",
    };

    string roamFileNames[] = {
        "FINAL_MERGE_TABLE-01-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-02-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-03-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-04-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-05-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-06-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-07-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-08-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-09-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-10-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-11-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-12-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-13-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-14-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-15-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-16-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-17-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-18-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-19-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-20-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-21-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-22-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-23-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-24-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-25-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-26-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-27-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-28-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-29-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-30-08-2016_matched.csv",
        "FINAL_MERGE_TABLE-31-08-2016_matched.csv",
    };
	// PrePuncProcessor test(3, 14, 18, 15, 19, 8, 16, 17, 20, 21);
    // bool succeeded = test.handle("/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv");
    // cout << succeeded << endl;
    // test.isActDprtLaterThanArrv("/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv");

    for (int i = 0; i < 31; i++) {
        opalFile = "/media/nlp/Maxtor/Transport/Opal/" + opalFileNames[i];
        roamFile = "/media/nlp/Maxtor/Transport/Roam/Person/" + roamFileNames[i];
        string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);

        OpalTripAnalyser opal;
        opal.setFile(opalFile);
        opal.calculatePerStationCount();
        map<string, vector<int> > mapOnOpalPerStationCount = opal.getOnPerStationCount();
        map<string, vector<int> > mapOffOpalPerStationCount = opal.getOffPerStationCount();

        RoamResultAnalyser roam;
        roam.setFile(roamFile);
        roam.calculatePerStationCount();
        map<string, vector<int> > mapOnRoamPerStationCount = roam.getOnPerStationCount();
        map<string, vector<int> > mapOffRoamPerStationCount = roam.getOffPerStationCount();

        float fSumInaccuracy = 0.0f;
        ofstream outfile;
        outfile.open("opal_roam_perstation_08" + strDate + "_on.csv");
        // output on as csv
        outfile << "Opal" << ", Count, " << "Roam" << ", Count," << endl;

        for (map<string, vector<int> >::iterator it = mapOnOpalPerStationCount.begin(); it != mapOnOpalPerStationCount.end(); ++it) {
            string strStationName = it->first;
            vector<int> vecOpal = it->second;

            map<string, vector<int> >::iterator itRoam = mapOnRoamPerStationCount.find(strStationName);
            if (itRoam != mapOnRoamPerStationCount.end()) {
                vector<int> vecRoam = itRoam->second;
                float accuracy = float(vecRoam.size())/float(vecOpal.size());
                float inaccuracy = 1 - accuracy;
                fSumInaccuracy += inaccuracy;
                outfile << strStationName << ", " << vecOpal.size() << ", " << itRoam->first << ", " << vecRoam.size() << ", " << accuracy << ", " << inaccuracy <<endl;
            }
            else {
                outfile << strStationName << ", " << vecOpal.size() << ", " << strStationName << ", " << 0 << ", " << 0 << ", " << 1 << endl;
            }
        }

        outfile << ",,,,," << fSumInaccuracy/mapOnOpalPerStationCount.size() << endl;

        outfile.close();


        fSumInaccuracy = 0.0f;
        outfile.open("opal_roam_perstation_08" + strDate + "_off.csv");
        // output off as csv
        outfile << "Opal" << ", Count, " << "Roam" << ", Count," << endl;

        for (map<string, vector<int> >::iterator it = mapOffOpalPerStationCount.begin(); it != mapOffOpalPerStationCount.end(); ++it) {
            string strStationName = it->first;
            vector<int> vecOpal = it->second;

            map<string, vector<int> >::iterator itRoam = mapOffRoamPerStationCount.find(strStationName);
            if (itRoam != mapOffRoamPerStationCount.end()) {
                vector<int> vecRoam = itRoam->second;
                float accuracy = float(vecRoam.size())/float(vecOpal.size());
                float inaccuracy = 1 - accuracy;
                fSumInaccuracy += inaccuracy;
                outfile << strStationName << ", " << vecOpal.size() << ", " << itRoam->first << ", " << vecRoam.size() << ", " << accuracy << ", " << inaccuracy <<endl;
            }
            else {
                outfile << strStationName << ", " << vecOpal.size() << ", " << strStationName << ", " << 0 << ", " << 0 << ", " << 1 << endl;
            }
        }

        outfile << ",,,,," << fSumInaccuracy/mapOffOpalPerStationCount.size() << endl;

        outfile.close();

    }
}
