import csv
import sys

# f = open("/media/nlp/Maxtor/Transport/Opal/JS_ALL_V.20160801_20160816.csvp", 'rt')
# try:
#     reader = csv.reader(f)
#     count = 0
#     stationNameList = {}
#     stationNameListD = {}
#     for row in list(reader)[1:]:
#     	st = row[0].split('|')
#     	if len(st)>60 and st[10].lower()=='train' :
#     		if st[60] not in stationNameListD:
#     			stationNameListD[st[60]] = 0
#     		if st[54] not in stationNameList:
#     			stationNameList[st[54]] = 0
#     		stationNameList[st[54]] += 1		
#     		stationNameListD[st[60]] += 1		
#     		if  st[60]=='Sydenham Station':
#     		#print st

#         	#print st[54],st[60],count
#         		count+=1
#     print count
#     print len(stationNameList)
#     count = 2
#     for i in sorted(stationNameListD.iterkeys()):
#     #for i in stationNameList:
#     	print count,i,stationNameListD[i]
#     	count+=1
# finally:
#     f.close()

f = open("/media/nlp/Maxtor/Transport/Roam/Person/FINAL_MERGE_TABLE-01-08-2016_matched.csv", 'rt')
try:
	reader = csv.reader(f)
	count = 0
	stationNameList = {}
	stationNameListD = {}
	Rcount=0
	# print len(list(reader)[1:])
	for row in list(reader)[1:]:
		if row[4] == '2 or more interchanges required':
			Rcount+=1

			# st = row[0].split('|')
			#if len(st)>60 and st[10].lower()=='train' :
			startStation= row[2] #12
			endStation=row[1]

			# 	if  st[60]=='Sydenham Station':
			# if 'NA' == row[3]:
			# #one train
			# # print row[12],row[15]

			# 	endStation=row[15]
			# else:
			# #two trains

			# # print row[12],row[22]
			# 	endStation=row[22]
			# # if startStation == 'Hexham':
			# 	print startStation,row[12],Rcount,row
			#print st[54],st[60],count
			# count+=1
			if startStation not in stationNameList:
				stationNameList[startStation] = 0
			if endStation not in stationNameListD:
				stationNameListD[endStation] = 0
			stationNameList[startStation] += 1		
			stationNameListD[endStation] += 1
		# if 	startStation == "Aberdeen":
		# print startStation,endStation
	count = 2
	totalCount =0 
	for i in sorted(stationNameListD.iterkeys()):
	#for i in stationNameList:
		print count,i,stationNameListD[i]
		totalCount+=	stationNameListD[i]
		count+=1
	print "Total",totalCount,"RowCount:",Rcount
    # print count
    # print len(stationNameList)
    # count = 2
    # for i in sorted(stationNameListD.iterkeys()):
    # #for i in stationNameList:
    # 	print count,i,stationNameListD[i]
    # 	count+=1
finally:
    f.close()