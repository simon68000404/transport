#include "PrePuncProcessor/PrePuncProcessor.h"
#include "PerPersonTripDataAnalyser/PerPersonTripDataAnalyser.h"
#include "TransportApplication/TransportApplication.h"

#include <iostream>
#include <string>
using namespace std;

#define _201608 0
#define _201609 1

#define OPAL_VS_ROAM_PER_STATION_PER_MONTH 0
#define OPAL_VS_ROAM_PER_STATION_PER_DAY 0

#define OPAL_VS_ROAM_PER_OD_PER_MONTH 0
#define OPAL_VS_ROAM_PER_OD_PER_DAY 0

#define OPAL_VS_ROAM_PER_LINE_PER_MONTH 0
#define OPAL_VS_ROAM_PER_LINE_PER_DAY 0

#define OPAL_VS_CVM_PER_STATION_PER_MONTH 0
#define OPAL_VS_CVM_PER_STATION_PER_DAY 0

#define OPAL_VS_CVM_PER_OD_PER_MONTH 0
#define OPAL_VS_CVM_PER_OD_PER_DAY 0

#define OPAL_VS_CVM_PER_LINE_PER_MONTH 0
#define OPAL_VS_CVM_PER_LINE_PER_DAY 0

#define CVM_VS_ROAM_PER_STATION_PER_MONTH 0

#define CVM_VS_ROAM_INTERCHANGES_PER_MONTH 0
#define CVM_VS_ROAM_INTERCHANGES_PER_DAY 0

#define EXTRACT_LINE_STATIONS 0

#define OPAL_EXCEPTION_PER_MONTH 0
#define OPAL_EXCEPTION_PER_DAY 0

#define ROAM_EXCEPTION_PER_MONTH 0
#define ROAM_EXCEPTION_PER_DAY 0

#define PUNC_EXCEPTION_PER_MONTH 0

#define ROAM_PERSON_VS_STOP_PER_STATION 0

#define CVM_PERSON_VS_STOP_PER_STATION 0

#define COMPLETENESS_CHECK 0
#define COMPLETENESS_CHECK2_PER_MONTH 1
#define COMPLETENESS_CHECK2_PER_DAY 0

int main(int argc, char* argv[]) {
    string strHardDriveBaseName = "/media/nlp/Elements/";
	string puncFile = strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv";
    string puncFile201609 = strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160930.csv";
    string puncFileDistinct201609 = strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160930_distinct.csv";

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

    string opalFileNames201609[] = {
        "JS_ALL_V.20160901_20160916.csvp",
        "JS_ALL_V.20160902_20160917.csvp",
        "JS_ALL_V.20160903_20160918.csvp",
        "JS_ALL_V.20160904_20160919.csvp",
        "JS_ALL_V.20160905_20160920.csvp",
        "JS_ALL_V.20160906_20160921.csvp",
        "JS_ALL_V.20160907_20160922.csvp",
        "JS_ALL_V.20160908_20160923.csvp",
        "JS_ALL_V.20160909_20160924.csvp",
        "JS_ALL_V.20160910_20160925.csvp",
        "JS_ALL_V.20160911_20160926.csvp",
        "JS_ALL_V.20160912_20160927.csvp",
        "JS_ALL_V.20160913_20160928.csvp",
        "JS_ALL_V.20160914_20160929.csvp",
        "JS_ALL_V.20160915_20160930.csvp",
        "JS_ALL_V.20160916_20161001.csvp",
        "JS_ALL_V.20160917_20161002.csvp",
        "JS_ALL_V.20160918_20161003.csvp",
        "JS_ALL_V.20160919_20161004.csvp",
        "JS_ALL_V.20160920_20161005.csvp",
        "JS_ALL_V.20160921_20161006.csvp",
        "JS_ALL_V.20160922_20161007.csvp",
        "JS_ALL_V.20160923_20161008.csvp",
        "JS_ALL_V.20160924_20161009.csvp",
        "JS_ALL_V.20160925_20161010.csvp",
        "JS_ALL_V.20160926_20161011.csvp",
        "JS_ALL_V.20160927_20161012.csvp",
        "JS_ALL_V.20160928_20161013.csvp",
        "JS_ALL_V.20160929_20161014.csvp",
        "JS_ALL_V.20160930_20161015.csvp",
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

    string cvmFileNames201609[] = {
        "20160901_distinct.csv",
        "20160902_distinct.csv",
        "20160903_distinct.csv",
        "20160904_distinct.csv",
        "20160905_distinct.csv",
        "20160906_distinct.csv",
        "20160907_distinct.csv",
        "20160908_distinct.csv",
        "20160909_distinct.csv",
        "20160910_distinct.csv",
        "20160911_distinct.csv",
        "20160912_distinct.csv",
        "20160913_distinct.csv",
        "20160914_distinct.csv",
        "20160915_distinct.csv",
        "20160916_distinct.csv",
        "20160917_distinct.csv",
        "20160918_distinct.csv",
        "20160919_distinct.csv",
        "20160920_distinct.csv",
        "20160921_distinct.csv",
        "20160922_distinct.csv",
        "20160923_distinct.csv",
        "20160924_distinct.csv",
        "20160925_distinct.csv",
        "20160926_distinct.csv",
        "20160927_distinct.csv",
        "20160928_distinct.csv",
        "20160929_distinct.csv",
        "20160930_distinct.csv",
    };

    vector<string> strOpalNames;
    vector<string> strOpalNames201609;
    vector<string> strRoamNames;
    vector<string> strRoamNames201609;
    vector<string> strCvmNames201609;

    vector<string> strRoamOpalDayOnOutputNames;
    vector<string> strRoamOpalDayOffOutputNames;
    vector<string> strRoamOpalDayOnOutputNames201609;
    vector<string> strRoamOpalDayOffOutputNames201609;
    vector<string> strCvmOpalDayOnOutputNames201609;
    vector<string> strCvmOpalDayOffOutputNames201609;
    
    vector<string> strRoamOpalDayODOutputNames;
    
    vector<string> strRoamOpalDayLineOutputNames;
    vector<string> strRoamOpalDayLineOutputNames201609;
    vector<string> strCvmOpalDayLineOutputNames201609;
    
    vector<string> strRoamOpalDayODOutputNames201609;
    vector<string> strCvmOpalDayODOutputNames201609;

    vector<string> strOpalExceptionBaseNames;
    vector<string> strRoamExceptionBaseNames;

    vector<string> strRoamPerStopNames;
    vector<string> strRoamPerStopNames201609;
    vector<string> strCvmPerStopNames201609;
    vector<string> strRoamPerStopDayOnOutputNames;
    vector<string> strRoamPerStopDayOffOutputNames;

    vector<string> strCompletenessCheckOutput201609;

    vector<string> strRoamCvmInterchangesDayOutputNames201609;


    string strMergedOnOutputName = "./Results/201608/Per Station/Summary/opal_roam_per_station_08_on.csv";
    string strMergedOffOutputName = "./Results/201608/Per Station/Summary/opal_roam_per_station_08_off.csv";
    string strMergedODOutputName = "./Results/201608/Per OD Pair/Summary/opal_roam_per_od_08.csv";
    string strMergedLineOutputName = "./Results/201608/Per Line/Summary/opal_roam_per_line_08.csv";

    string strMergedRoamOpalOnOutputName201609 = "./Results/201609/Per Station/Summary/opal_roam_per_station_09_on.csv";
    string strMergedRoamOpalOffOutputName201609 = "./Results/201609/Per Station/Summary/opal_roam_per_station_09_off.csv";
    string strMergedRoamOpalODOutputName201609 = "./Results/201609/Per OD Pair/Summary/opal_roam_per_od_09.csv";
    string strMergedRoamOpalLineOutputName201609 = "./Results/201609/Per Line/Summary/opal_roam_per_line_09.csv";

    string strMergedCvmOpalOnOutputName201609 = "./Results/201609/Per Station/Summary/opal_cvm_per_station_09_on.csv";
    string strMergedCvmOpalOffOutputName201609 = "./Results/201609/Per Station/Summary/opal_cvm_per_station_09_off.csv";
    string strMergedCvmOpalODOutputName201609 = "./Results/201609/Per OD Pair/Summary/opal_cvm_per_od_09.csv";
    string strMergedCvmOpalLineOutputName201609 = "./Results/201609/Per Line/Summary/opal_cvm_per_line_09.csv";

    string strMergedRoamCvmInterchangesOutputName201609 = "./Results/201609/Interchanges/Summary/roam_cvm_just_interchanges_09.csv";

    string strMergedCompletenessCheckOutput201609 = "./Results/201609/Completeness Check/Summary/roam_cvm_completeness_check_09.csv";

    // self check 
    string strMergedRoamPerStopOnOutputName = "./Results/roam_pp_vs_ps_per_station_08_on.csv";
    string strMergedRoamPerStopOffOutputName = "./Results/roam_pp_vs_ps_per_station_08_off.csv";

    string strMergedCvmPerStopOnOutputName201609 = "./Results/cvm_pp_vs_ps_per_station_09_on.csv";
    string strMergedCvmPerStopOffOutputName201609 = "./Results/cvm_pp_vs_ps_per_station_09_off.csv";

    string strMergedRoamCvmPerStopOnOutputName201609 = "./Results/roam_ps_vs_cvm_ps_per_station_09_on.csv";
    string strMergedRoamCvmPerStopOffOutputName201609 = "./Results/roam_ps_vs_cvm_ps_per_station_09_off.csv";

    string strMergedOpalExceptionBaseName = "./Results/201608/Exceptions/Opal/Summary/exception_opal_08_";
    string strMergedRoamExceptionBaseName = "./Results/201608/Exceptions/Roam/Summary/exception_roam_08_";
    string strPuncExceptionBaseName = "./Results/201608/Exceptions/Punctuality/exception_punctuality_08_";

    for (int i = 0; i < 31; i++) {
        string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);
        strOpalNames.push_back(strHardDriveBaseName + "Transport/Opal/201608/" + opalFileNames[i]);
        strRoamNames.push_back(strHardDriveBaseName + "Transport/Roam/Person/" + roamFileNames[i]);

        strRoamOpalDayOnOutputNames.push_back("./Results/201608/Per Station/Daily Reports/opal_roam_per_station_08" + strDate + "_on.csv");
        strRoamOpalDayOffOutputNames.push_back("./Results/201608/Per Station/Daily Reports/opal_roam_per_station_08" + strDate + "_off.csv");

        strRoamOpalDayODOutputNames.push_back("./Results/201608/Per OD Pair/Daily Reports/opal_roam_per_od_08" + strDate + ".csv");

        strRoamOpalDayLineOutputNames.push_back("./Results/201608/Per Line/Daily Reports/opal_roam_per_line_08" + strDate + ".csv");

        strOpalExceptionBaseNames.push_back("./Results/201608/Exceptions/Opal/Daily Reports/exception_opal_08" + strDate + "_");
        strRoamExceptionBaseNames.push_back("./Results/201608/Exceptions/Roam/Daily Reports/exception_roam_08" + strDate + "_");

        strRoamPerStopNames.push_back(strHardDriveBaseName + "Transport/Roam/Stop/" + roamPerStopFileNames[i]);
        strRoamPerStopDayOnOutputNames.push_back("./Results/roam_pp_vs_ps_per_station_08" + strDate + "_on.csv");
        strRoamPerStopDayOffOutputNames.push_back("./Results/roam_pp_vs_ps_per_station_08" + strDate + "_off.csv");
    }

    // strCvmNames201609.push_back("/media/nlp/Elements/Transport/CVM/Person/boardings_awtt_20160901_and_20160902.csv");

    for (int i = 0; i < 30; i++) {
        string strDate = i < 9 ? "0" + to_string(i + 1) : to_string(i + 1);
        strOpalNames201609.push_back(strHardDriveBaseName + "Transport/Opal/201609/" + opalFileNames201609[i]);
        strRoamNames201609.push_back(strHardDriveBaseName + "Transport/Roam/Person/" + roamFileNames201609[i]);
        strCvmNames201609.push_back(strHardDriveBaseName + "Transport/CVM/Person/" + cvmFileNames201609[i]);

        strRoamOpalDayOnOutputNames201609.push_back("./Results/201609/Per Station/Daily Reports/opal_roam_per_station_09" + strDate + "_on.csv");
        strRoamOpalDayOffOutputNames201609.push_back("./Results/201609/Per Station/Daily Reports/opal_roam_per_station_09" + strDate + "_off.csv");
        strCvmOpalDayOnOutputNames201609.push_back("./Results/201609/Per Station/Daily Reports/opal_cvm_per_station_09" + strDate + "_on.csv");
        strCvmOpalDayOffOutputNames201609.push_back("./Results/201609/Per Station/Daily Reports/opal_cvm_per_station_09" + strDate + "_off.csv");

        strRoamOpalDayODOutputNames201609.push_back("./Results/201609/Per OD Pair/Daily Reports/opal_roam_per_od_09" + strDate + ".csv");
        strCvmOpalDayODOutputNames201609.push_back("./Results/201609/Per OD Pair/Daily Reports/opal_cvm_per_od_09" + strDate + ".csv");

        strRoamOpalDayLineOutputNames201609.push_back("./Results/201609/Per Line/Daily Reports/opal_roam_per_line_09" + strDate + ".csv");
        strCvmOpalDayLineOutputNames201609.push_back("./Results/201609/Per Line/Daily Reports/opal_cvm_per_line_09" + strDate + ".csv");
        
        strCompletenessCheckOutput201609.push_back("./Results/201609/Completeness Check/Daily Reports/roam_cvm_completeness_check_09" + strDate + ".csv");

        strRoamCvmInterchangesDayOutputNames201609.push_back("./Results/201609/Interchanges/Daily Reports/roam_cvm_just_interchanges_09" + strDate + ".csv");
        
        strRoamPerStopNames201609.push_back(strHardDriveBaseName + "Transport/Roam/Stop/" + roamPerStopFileNames201609[i]);

        strCvmPerStopNames201609.push_back(strHardDriveBaseName + "Transport/CVM/Stop/201609" + strDate + "_d__services_calcs.csv");
    }

    // strCvmPerStopNames201609.push_back(strHardDriveBaseName + "Transport/CVM/Stop/d__services_calcs.csv");

    // All stations -> lines
    map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLinesOneToOne("Stations_Lines_311_20160706.csv", 2, 4);


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
    for (int i = 0; i < 1; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerStationPerDay(strInputNames1, strInputNames2, strRoamOpalDayOnOutputNames[i], strRoamOpalDayOffOutputNames[i]);
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
    for (int i = 0; i < 1; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerODPerDay(strInputNames1, strInputNames2, strRoamOpalDayODOutputNames[i]);
    }
#endif

#if EXTRACT_LINE_STATIONS
    TransportApplication::generateAllLines(strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv", "all_lines.csv");
#endif

#if OPAL_VS_ROAM_PER_LINE_PER_MONTH
    // map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    // Per line - whole aug into one csv
    TransportApplication::compareOpalAndRoamPerLinePerDay(
        strOpalNames, 
        strRoamNames, 
        strMergedLineOutputName, mapStationLines);
#endif
#if OPAL_VS_ROAM_PER_LINE_PER_DAY
    // Per line - each day of aug into one csv

    // map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    for (int i = 0; i < 1; i++) {
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames[i]);

        TransportApplication::compareOpalAndRoamPerLinePerDay(strInputNames1, strInputNames2, strRoamOpalDayLineOutputNames[i], mapStationLines);
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
    for (int i = 0; i < 31; i++) {
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
    for (int i = 0; i < 31; i++) {
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

#if OPAL_VS_ROAM_PER_STATION_PER_MONTH
    // Per station - whole sep into one csv
    cout << "September Opal vs Roam per station - month" << endl;
    TransportApplication::compareOpalAndRoamPerStationPerDay(
        strOpalNames201609, 
        strRoamNames201609, 
        strMergedRoamOpalOnOutputName201609, 
        strMergedRoamOpalOffOutputName201609);
#endif
#if OPAL_VS_ROAM_PER_STATION_PER_DAY
    // Per station - each day of sep into one csv
    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs Roam per station - day " << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames201609[i]);

        TransportApplication::compareOpalAndRoamPerStationPerDay(strInputNames1, strInputNames2, strRoamOpalDayOnOutputNames201609[i], strRoamOpalDayOffOutputNames201609[i]);
    }
#endif

#if OPAL_VS_ROAM_PER_OD_PER_MONTH
    // Per od - whole aug into one csv
    cout << "September Opal vs Roam per od - month" << endl;
    TransportApplication::compareOpalAndRoamPerODPerDay(
        strOpalNames201609, 
        strRoamNames201609, 
        strMergedRoamOpalODOutputName201609);
#endif
#if OPAL_VS_ROAM_PER_OD_PER_DAY
    // Per od - each day of aug into one csv
    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs Roam per od - day" << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames201609[i]);

        TransportApplication::compareOpalAndRoamPerODPerDay(strInputNames1, strInputNames2, strRoamOpalDayODOutputNames201609[i]);
    }
#endif

// #if EXTRACT_LINE_STATIONS
//     TransportApplication::generateAllLines(strHardDriveBaseName + "Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv", "all_lines.csv");
// #endif

#if OPAL_VS_ROAM_PER_LINE_PER_MONTH
    // map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    // Per line - whole aug into one csv
    cout << "September Opal vs Roam per line - month" << endl;
    TransportApplication::compareOpalAndRoamPerLinePerDay(
        strOpalNames201609, 
        strRoamNames201609, 
        strMergedRoamOpalLineOutputName201609, mapStationLines);
#endif
#if OPAL_VS_ROAM_PER_LINE_PER_DAY
    // Per line - each day of aug into one csv

    // map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs Roam per line - day" << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strRoamNames201609[i]);

        TransportApplication::compareOpalAndRoamPerLinePerDay(strInputNames1, strInputNames2, strRoamOpalDayLineOutputNames201609[i], mapStationLines);
    }
#endif

#if OPAL_VS_CVM_PER_STATION_PER_MONTH
    // Per station - whole sep into one csv
    cout << "September Opal vs CVM per station - month" << endl;
    TransportApplication::compareOpalAndCvmPerStationPerDay(
        strOpalNames201609, 
        strCvmNames201609, 
        strMergedCvmOpalOnOutputName201609, 
        strMergedCvmOpalOffOutputName201609);
#endif
#if OPAL_VS_CVM_PER_STATION_PER_DAY
    // Per station - each day of sep into one csv
    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs CVM per station - day " << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strCvmNames201609[i]);

        TransportApplication::compareOpalAndCvmPerStationPerDay(strInputNames1, strInputNames2, strCvmOpalDayOnOutputNames201609[i], strCvmOpalDayOffOutputNames201609[i]);
    }
#endif

#if OPAL_VS_CVM_PER_OD_PER_MONTH
    // Per od - whole aug into one csv
    cout << "September Opal vs Cvm per od - month" << endl;
    TransportApplication::compareOpalAndCvmPerODPerDay(
        strOpalNames201609, 
        strCvmNames201609, 
        strMergedCvmOpalODOutputName201609);
#endif
#if OPAL_VS_CVM_PER_OD_PER_DAY
    // Per od - each day of aug into one csv
    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs CVM per od - day" << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strCvmNames201609[i]);

        TransportApplication::compareOpalAndCvmPerODPerDay(strInputNames1, strInputNames2, strCvmOpalDayODOutputNames201609[i]);
    }
#endif

#if OPAL_VS_CVM_PER_LINE_PER_MONTH
    // map<string, vector<string> > mapStationLines = TransportApplication::importAllStationLines("all_lines.csv");
    // Per line - whole aug into one csv
    cout << "September Opal vs CVM per line - month" << endl;
    TransportApplication::compareOpalAndCvmPerLinePerDay(
        strOpalNames201609, 
        strCvmNames201609, 
        strMergedCvmOpalLineOutputName201609, mapStationLines);
#endif
#if OPAL_VS_CVM_PER_LINE_PER_DAY
    // Per line - each day of aug into one csv

    for (int i = 0; i < 30; i++) {
        cout << "September Opal vs CVM per line - day" << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strOpalNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strCvmNames201609[i]);

        TransportApplication::compareOpalAndCvmPerLinePerDay(strInputNames1, strInputNames2, strCvmOpalDayLineOutputNames201609[i], mapStationLines);
    }
#endif

#if TEST
        cout << "aaaaaaaaaaa" << endl;

#endif

#if CVM_VS_ROAM_INTERCHANGES_PER_MONTH
    // Per station - whole sep into one csv
    cout << "September Roam vs CVM just interchanges - month" << endl;
    TransportApplication::compareRoamAndCvmInterchangesPerDay(
        strRoamNames201609, 
        strCvmNames201609, 
        strMergedRoamCvmInterchangesOutputName201609);
#endif
#if CVM_VS_ROAM_INTERCHANGES_PER_DAY
    // Per station - each day of sep into one csv
    for (int i = 0; i < 30; i++) {
        cout << "September Roam vs CVM just interchanges - day " << i << endl;
        vector<string> strInputNames1;
        strInputNames1.push_back(strRoamNames201609[i]);
        vector<string> strInputNames2;
        strInputNames2.push_back(strCvmNames201609[i]);

        TransportApplication::compareRoamAndCvmInterchangesPerDay(strInputNames1, strInputNames2, strRoamCvmInterchangesDayOutputNames201609[i]);
    }
#endif

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

#if COMPLETENESS_CHECK2_PER_MONTH
    vector<string> strPuncInputCSVNames;
    strPuncInputCSVNames.push_back(puncFileDistinct201609);
    map<std::string, std::vector<std::string> > mapPerStopInputCSVNames;
    mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Roam", strRoamPerStopNames201609));
    mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Cvm", strCvmPerStopNames201609));
    TransportApplication::checkCompleteness2(strPuncInputCSVNames, mapPerStopInputCSVNames, 
        strMergedCompletenessCheckOutput201609);
#endif
// #if COMPLETENESS_CHECK2_PER_DAY
//     vector<string> strPuncInputCSVNames;
//     strPuncInputCSVNames.push_back(puncFile201609);

//     for (int i = 0; i < 1; i++) {
//         cout << "September Completeness check - day " << i << endl;
//         vector<string> strInputNames1;
//         strInputNames1.push_back(strRoamPerStopNames201609[i]);
//         vector<string> strInputNames2;
//         strInputNames2.push_back(strCvmPerStopNames201609[i]);

//         map<std::string, std::vector<std::string> > mapPerStopInputCSVNames;
//         mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Roam", strInputNames1));
//         mapPerStopInputCSVNames.insert(pair<string, vector<string> >("Cvm", strInputNames2));

//         TransportApplication::checkCompleteness2(strPuncInputCSVNames[i], mapPerStopInputCSVNames, 
//             strCompletenessCheckOutput201609[i]);
//     }
// #endif

#if CVM_PERSON_VS_STOP_PER_STATION
    TransportApplication::comparerCvmPPAndCvmPSPerStationPerDay(
        strCvmNames201609, 
        strCvmPerStopNames201609, 
        strMergedCvmPerStopOnOutputName201609, 
        strMergedCvmPerStopOffOutputName201609);
#endif

#endif
}
