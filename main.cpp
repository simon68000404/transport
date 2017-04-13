#include "PrePuncProcessor/PrePuncProcessor.h"
#include "PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"
#include "TransportApplication/TransportApplication.h"

#include <iostream>
#include <string>
using namespace std;

#define _201608 1
#define _201609 0

#define OPAL_VS_ROAM_PER_STATION_PER_MONTH 0
#define OPAL_VS_ROAM_PER_STATION_PER_DAY 0

#define OPAL_VS_ROAM_PER_OD_PER_MONTH 0
#define OPAL_VS_ROAM_PER_OD_PER_DAY 0

#define OPAL_VS_ROAM_PER_LINE_PER_MONTH 0
#define OPAL_VS_ROAM_PER_LINE_PER_DAY 0

#define CVM_VS_ROAM_PER_STATION_PER_MONTH 0

#define EXTRACT_LINE_STATIONS 0

#define OPAL_EXCEPTION_PER_MONTH 0
#define OPAL_EXCEPTION_PER_DAY 0

#define ROAM_EXCEPTION_PER_MONTH 0
#define ROAM_EXCEPTION_PER_DAY 0

#define PUNC_EXCEPTION_PER_MONTH 0

#define ROAM_PERSON_VS_STOP_PER_STATION 0

#define COMPLETENESS_CHECK 1

int main(int argc, char* argv[]) {
    string strHardDriveBaseName = "/media/vision/Maxtor/";
	string puncFile = strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv";
    string puncFile201609 = strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160930.csv";

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

    string roamPerStopFileNames[] = {
        "Final OUTPUT TPA-01-08-2016_matched.csv",
        "Final OUTPUT TPA-02-08-2016_matched.csv",
        "Final OUTPUT TPA-03-08-2016_matched.csv",
        "Final OUTPUT TPA-04-08-2016_matched.csv",
        "Final OUTPUT TPA-05-08-2016_matched.csv",
        "Final OUTPUT TPA-06-08-2016_matched.csv",
        "Final OUTPUT TPA-07-08-2016_matched.csv",
        "Final OUTPUT TPA-08-08-2016_matched.csv",
        "Final OUTPUT TPA-09-08-2016_matched.csv",
        "Final OUTPUT TPA-10-08-2016_matched.csv",
        "Final OUTPUT TPA-11-08-2016_matched.csv",
        "Final OUTPUT TPA-12-08-2016_matched.csv",
        "Final OUTPUT TPA-13-08-2016_matched.csv",
        "Final OUTPUT TPA-14-08-2016_matched.csv",
        "Final OUTPUT TPA-15-08-2016_matched.csv",
        "Final OUTPUT TPA-16-08-2016_matched.csv",
        "Final OUTPUT TPA-17-08-2016_matched.csv",
        "Final OUTPUT TPA-18-08-2016_matched.csv",
        "Final OUTPUT TPA-19-08-2016_matched.csv",
        "Final OUTPUT TPA-20-08-2016_matched.csv",
        "Final OUTPUT TPA-21-08-2016_matched.csv",
        "Final OUTPUT TPA-22-08-2016_matched.csv",
        "Final OUTPUT TPA-23-08-2016_matched.csv",
        "Final OUTPUT TPA-24-08-2016_matched.csv",
        "Final OUTPUT TPA-25-08-2016_matched.csv",
        "Final OUTPUT TPA-26-08-2016_matched.csv",
        "Final OUTPUT TPA-27-08-2016_matched.csv",
        "Final OUTPUT TPA-28-08-2016_matched.csv",
        "Final OUTPUT TPA-29-08-2016_matched.csv",
        "Final OUTPUT TPA-30-08-2016_matched.csv",
        "Final OUTPUT TPA-31-08-2016_matched.csv",
    };

    string roamFileNames201609[] = {
        "FINAL_MERGE_TABLE-01-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-02-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-03-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-04-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-05-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-06-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-07-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-08-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-09-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-10-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-11-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-12-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-13-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-14-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-15-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-16-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-17-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-18-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-19-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-20-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-21-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-22-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-23-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-24-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-25-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-26-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-27-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-28-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-29-09-2016_matched.csv",
        "FINAL_MERGE_TABLE-30-09-2016_matched.csv",
    };

    string roamPerStopFileNames201609[] = {
        "Final OUTPUT TPA-01-09-2016_matched.csv",
        "Final OUTPUT TPA-02-09-2016_matched.csv",
        "Final OUTPUT TPA-03-09-2016_matched.csv",
        "Final OUTPUT TPA-04-09-2016_matched.csv",
        "Final OUTPUT TPA-05-09-2016_matched.csv",
        "Final OUTPUT TPA-06-09-2016_matched.csv",
        "Final OUTPUT TPA-07-09-2016_matched.csv",
        "Final OUTPUT TPA-08-09-2016_matched.csv",
        "Final OUTPUT TPA-09-09-2016_matched.csv",
        "Final OUTPUT TPA-10-09-2016_matched.csv",
        "Final OUTPUT TPA-11-09-2016_matched.csv",
        "Final OUTPUT TPA-12-09-2016_matched.csv",
        "Final OUTPUT TPA-13-09-2016_matched.csv",
        "Final OUTPUT TPA-14-09-2016_matched.csv",
        "Final OUTPUT TPA-15-09-2016_matched.csv",
        "Final OUTPUT TPA-16-09-2016_matched.csv",
        "Final OUTPUT TPA-17-09-2016_matched.csv",
        "Final OUTPUT TPA-18-09-2016_matched.csv",
        "Final OUTPUT TPA-19-09-2016_matched.csv",
        "Final OUTPUT TPA-20-09-2016_matched.csv",
        "Final OUTPUT TPA-21-09-2016_matched.csv",
        "Final OUTPUT TPA-22-09-2016_matched.csv",
        "Final OUTPUT TPA-23-09-2016_matched.csv",
        "Final OUTPUT TPA-24-09-2016_matched.csv",
        "Final OUTPUT TPA-25-09-2016_matched.csv",
        "Final OUTPUT TPA-26-09-2016_matched.csv",
        "Final OUTPUT TPA-27-09-2016_matched.csv",
        "Final OUTPUT TPA-28-09-2016_matched.csv",
        "Final OUTPUT TPA-29-09-2016_matched.csv",
        "Final OUTPUT TPA-30-09-2016_matched.csv",
    };

    vector<string> strOpalNames;
    vector<string> strRoamNames;
    vector<string> strRoamNames201609;
    vector<string> strDayOnOutputNames;
    vector<string> strDayOffOutputNames;
    vector<string> strDayODOutputNames;
    vector<string> strDayLineOutputNames;
    vector<string> strOpalExceptionBaseNames;
    vector<string> strRoamExceptionBaseNames;

    vector<string> strRoamPerStopNames;
    vector<string> strRoamPerStopNames201609;
    vector<string> strCvmPerStopNames201609;
    vector<string> strRoamPerStopDayOnOutputNames;
    vector<string> strRoamPerStopDayOffOutputNames;

    string strMergedOnOutputName = "./Results/201608/Per Station/Summary/opal_roam_per_station_08_on.csv";
    string strMergedOffOutputName = "./Results/201608/Per Station/Summary/opal_roam_per_station_08_off.csv";
    string strMergedODOutputName = "./Results/201608/Per OD Pair/Summary/opal_roam_per_od_08.csv";
    string strMergedLineOutputName = "./Results/opal_roam_per_line_08.csv";

    string strMergedRoamPerStopOnOutputName = "./Results/roam_pp_vs_ps_per_station_08_on.csv";
    string strMergedRoamPerStopOffOutputName = "./Results/roam_pp_vs_ps_per_station_08_off.csv";

    string strMergedRoamCvmPerStopOnOutputName201609 = "./Results/roam_ps_vs_cvm_ps_per_station_09_on.csv";
    string strMergedRoamCvmPerStopOffOutputName201609 = "./Results/roam_ps_vs_cvm_ps_per_station_09_off.csv";

    string strMergedOpalExceptionBaseName = "./Results/201608/Exceptions/Opal/Summary/exception_opal_08_";
    string strMergedRoamExceptionBaseName = "./Results/201608/Exceptions/Roam/Summary/exception_roam_08_";
    string strPuncExceptionBaseName = "./Results/201608/Exceptions/Punctuality/exception_punctuality_08_";

    for (int i = 0; i < 31; i++) {
        string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);
        strOpalNames.push_back(strHardDriveBaseName + "Transport/Opal/" + opalFileNames[i]);
        strRoamNames.push_back(strHardDriveBaseName + "Transport/Roam/Person/" + roamFileNames[i]);

        strDayOnOutputNames.push_back("./Results/201608/Per Station/Daily Reports/opal_roam_per_station_08" + strDate + "_on.csv");
        strDayOffOutputNames.push_back("./Results/201608/Per Station/Daily Reports/opal_roam_per_station_08" + strDate + "_off.csv");

        strDayODOutputNames.push_back("./Results/201608/Per OD Pair/Daily Reports/opal_roam_per_od_08" + strDate + ".csv");

        strDayLineOutputNames.push_back("./Results/opal_roam_per_line_08" + strDate + ".csv");

        strOpalExceptionBaseNames.push_back("./Results/201608/Exceptions/Opal/Daily Reports/exception_opal_08" + strDate + "_");
        strRoamExceptionBaseNames.push_back("./Results/201608/Exceptions/Roam/Daily Reports/exception_roam_08" + strDate + "_");

        strRoamPerStopNames.push_back(strHardDriveBaseName + "Transport/Roam/Stop/" + roamPerStopFileNames[i]);
        strRoamPerStopDayOnOutputNames.push_back("./Results/roam_pp_vs_ps_per_station_08" + strDate + "_on.csv");
        strRoamPerStopDayOffOutputNames.push_back("./Results/roam_pp_vs_ps_per_station_08" + strDate + "_off.csv");
    }

    for (int i = 0; i < 30; i++) {
        strRoamNames201609.push_back(strHardDriveBaseName + "Transport/Roam/Person/" + roamFileNames201609[i]);
        strRoamPerStopNames201609.push_back(strHardDriveBaseName + "Transport/Roam/Stop/" + roamPerStopFileNames201609[i]);
    }

    strCvmPerStopNames201609.push_back(strHardDriveBaseName + "Transport/CVM/Stop/d__services_calcs.csv");

#if _201608
#if OPAL_VS_ROAM_PER_STATION_PER_MONTH
    // Per station - whole aug into one csv
    TransportApplication::compareOpalAndRoamPerStationPerDay(
        strOpalNames, 
        strRoamNames, 
        strMergedOnOutputName, 
        strMergedOffOutputName);
#endif
#if OPAL_VS_ROAM_PER_STATION_PER_DAY
    // Per station - each day of aug into one csv
    for (int i = 0; i < 31; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerStationPerDay(strInputNames1, strInputNames2, strDayOnOutputNames[i], strDayOffOutputNames[i]);
    }
#endif

#if OPAL_VS_ROAM_PER_OD_PER_MONTH
    // Per od - whole aug into one csv
    TransportApplication::compareOpalAndRoamPerODPerDay(
        strOpalNames, 
        strRoamNames, 
        strMergedODOutputName);
#endif
#if OPAL_VS_ROAM_PER_OD_PER_DAY
    // Per od - each day of aug into one csv
    for (int i = 0; i < 31; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerODPerDay(strInputNames1, strInputNames2, strDayODOutputNames[i]);
    }
#endif

#if EXTRACT_LINE_STATIONS
    TransportApplication::generateAllLines(strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv", "all_lines.csv");
#endif

#if OPAL_VS_ROAM_PER_LINE_PER_MONTH
    map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    // Per line - whole aug into one csv
    TransportApplication::compareOpalAndRoamPerLinePerDay(
        strOpalNames, 
        strRoamNames, 
        strMergedLineOutputName, mapStationLines);
#endif
#if OPAL_VS_ROAM_PER_LINE_PER_DAY
    // Per line - each day of aug into one csv

    map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");

    for (int i = 0; i < 31; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerLinePerDay(strInputNames1, strInputNames2, strDayLineOutputNames[i], mapStationLines);
    }
#endif

#if OPAL_EXCEPTION_PER_MONTH
    cout << "Opal exceptions per month..." << endl;
    TransportApplication::generateOpalExceptions(
        strOpalNames, 
        strMergedOpalExceptionBaseName);
#endif
#if OPAL_EXCEPTION_PER_DAY
    cout << "Opal exceptions per day..." << endl;
    for (int i = 0; i < 2; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);

        TransportApplication::generateOpalExceptions(strInputNames1, strOpalExceptionBaseNames[i]);
    }

#endif

#if ROAM_EXCEPTION_PER_MONTH
    cout << "Roam exceptions per month..." << endl;
    TransportApplication::generateRoamExceptions(
        strRoamNames, 
        strMergedRoamExceptionBaseName);
#endif
#if ROAM_EXCEPTION_PER_DAY
    cout << "Roam exceptions per day..." << endl;
    for (int i = 0; i < 31; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strRoamNames[i]);

        TransportApplication::generateRoamExceptions(strInputNames1, strRoamExceptionBaseNames[i]);
    }

#endif

#if PUNC_EXCEPTION_PER_MONTH
    cout << "Punc exceptions per month..." << endl;
    vector<string> vecPuncFiles;
    vecPuncFiles.push_back(puncFile);
    TransportApplication::generatePuncExceptions(
        vecPuncFiles, 
        strPuncExceptionBaseName);
#endif

#if ROAM_PERSON_VS_STOP_PER_STATION
    // Per station - whole aug into one csv
    // TransportApplication::comparerRoamPPAndRoamPSPerStationPerDay(
    //     strRoamPerStopNames, 
    //     strRoamNames, 
    //     strMergedRoamPerStopOnOutputName, 
    //     strMergedRoamPerStopOffOutputName);

    // Per station - each day of aug into one csv
    for (int i = 0; i < 1; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strRoamNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamPerStopNames[i]);

        TransportApplication::comparerRoamPPAndRoamPSPerStationPerDay(strInputNames1, strInputNames2, strRoamPerStopDayOnOutputNames[i], strRoamPerStopDayOffOutputNames[i]);
    }
#endif

#if COMPLETENESS_CHECK
    vector<string> strPuncInputCSVNames;
    strPuncInputCSVNames.push_back(puncFile);
    map<std::string, std::vector<std::string> > mapPerStopInputCSVNames;
    mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Roam", strRoamPerStopNames));
    TransportApplication::checkCompleteness(strPuncInputCSVNames, mapPerStopInputCSVNames, 
        "./Results/201608/Completeness Check/completeness_check.csv");
#endif
#endif

#if _201609
#if CVM_VS_ROAM_PER_STATION_PER_MONTH
    TransportApplication::compareRoamAndCvmPerStationFromPerStopData(
        strRoamPerStopNames201609,  
        strCvmPerStopNames201609,
        strMergedRoamCvmPerStopOnOutputName201609, 
        strMergedRoamCvmPerStopOffOutputName201609);
#endif

#if COMPLETENESS_CHECK
    vector<string> strPuncInputCSVNames;
    strPuncInputCSVNames.push_back(puncFile201609);
    map<std::string, std::vector<std::string> > mapPerStopInputCSVNames;
    mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Roam", strRoamPerStopNames201609));
    mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Cvm", strCvmPerStopNames201609));
    TransportApplication::checkCompleteness(strPuncInputCSVNames, mapPerStopInputCSVNames, 
        "./Results/201609/Completeness Check/completeness_check.csv");
#endif
#endif
}
