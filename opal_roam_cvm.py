# -*- coding: utf-8 -*-

import csv
import datetime

base_file_path = "/home/nlp/dev/Sammi/Transport/Results/201609/Per Station/Daily Reports/"
out_base_file_path = "/home/nlp/dev/Sammi/Transport/Results/201609/Per Station/For R/"

full_file_paths_roam_on = []
full_file_paths_roam_off = []
full_file_paths_cvm_on = []
full_file_paths_cvm_off = []

out_file_paths_on = out_base_file_path + "opal_roam_cvm_per_station_09_on.csv"
out_file_paths_off = out_base_file_path + "opal_roam_cvm_per_station_09_off.csv"

for i in range(1, 31):
    str_date = str(i) if i > 9 else "0" + str(i) 
    full_file_paths_roam_on.append(base_file_path + "opal_roam_per_station_09" + str_date + "_on.csv")
    full_file_paths_roam_off.append(base_file_path + "opal_roam_per_station_09" + str_date + "_off.csv")
    full_file_paths_cvm_on.append(base_file_path + "opal_cvm_per_station_09" + str_date + "_on.csv")
    full_file_paths_cvm_off.append(base_file_path + "opal_cvm_per_station_09" + str_date + "_off.csv")


on_list1 = []
on_list2 = []
on_result_list = [["Station", "Service Date", "Is Weekend", "Opal Count", "ROAM Count", "CVM Count"]]

for i in range(1, 31):
    with open(full_file_paths_roam_on[i - 1], 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='|')
        on_list1 = list(reader)
    with open(full_file_paths_cvm_on[i - 1], 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='|')
        on_list2 = list(reader)
    j = 0
    for row1 in on_list1:
        row2 = on_list2[j]
        str_date = str(i) if i > 9 else "0" + str(i) 
        station = ""
        service_date = ""
        is_weekend = False
        opal_cnt = 0
        roam_cnt = 0
        cvm_cnt = 0
        if row1[0] != "Total" and row1[0] != "Average" and row1[0] != "Station":
            station = row1[0]
            service_date = "2016-09-" + str_date
            is_weekend = datetime.datetime.strptime(service_date, '%Y-%m-%d').weekday() > 4
            opal_cnt = row1[1]
            roam_cnt = row1[2]
            cvm_cnt = row2[2]
            on_result_list.append([station, service_date, is_weekend, opal_cnt, roam_cnt, cvm_cnt])
        j += 1
    i += 1

with open(out_file_paths_on, 'wb') as csvfileoutName:
    spamwriter = csv.writer(csvfileoutName, quotechar = "'")
    for i in on_result_list:
        spamwriter.writerow(i)

off_list1 = []
off_list2 = []
off_result_list = [["Station", "Service Date", "Is Weekend", "Opal Count", "ROAM Count", "CVM Count"]]

for i in range(1, 31):
    with open(full_file_paths_roam_off[i - 1], 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='|')
        off_list1 = list(reader)
    with open(full_file_paths_cvm_off[i - 1], 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='|')
        off_list2 = list(reader)
    j = 0
    for row1 in off_list1:
        row2 = off_list2[j]
        str_date = str(i) if i > 9 else "0" + str(i) 
        station = ""
        service_date = ""
        is_weekend = False
        opal_cnt = 0
        roam_cnt = 0
        cvm_cnt = 0
        if row1[0] != "Total" and row1[0] != "Average" and row1[0] != "Station":
            station = row1[0]
            service_date = "2016-09-" + str_date
            is_weekend = datetime.datetime.strptime(service_date, '%Y-%m-%d').weekday() > 4
            opal_cnt = row1[1]
            roam_cnt = row1[2]
            cvm_cnt = row2[2]
            off_result_list.append([station, service_date, is_weekend, opal_cnt, roam_cnt, cvm_cnt])
        j += 1
    i += 1

with open(out_file_paths_off, 'wb') as csvfileoutName:
    spamwriter = csv.writer(csvfileoutName, quotechar = "'")
    for i in off_result_list:
        spamwriter.writerow(i)