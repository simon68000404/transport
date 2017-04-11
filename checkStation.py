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

stationNameList = {}
stationNameListD = {}
Rcount=0
totalCount =0

month = '08'
mypath= "/media/nlp/Maxtor/Transport/Roam/Person/"
from os import listdir
from os.path import isfile, join
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
onlyfiles=sorted(onlyfiles)
for filePath in onlyfiles:
	if filePath[-4:]=='.csv' and filePath.split('-')[2]==month:
		newFilePath = "/media/nlp/Maxtor/Transport/Roam/Person/"+filePath
		print newFilePath
		f = open(newFilePath, 'rt')
		try:
			reader = csv.reader(f)
			count = 0

			for row in list(reader)[1:]:
				if row[4] == '2 or more interchanges required':
					Rcount+=1
				else:

					startStation= row[2] #12
					endStation=row[1]

					if startStation not in stationNameList:
						stationNameList[startStation] = 0
					if endStation not in stationNameListD:
						stationNameListD[endStation] = 0
					stationNameList[startStation] += 1		
					stationNameListD[endStation] += 1
				totalCount+=1

			count = 2
			

		finally:
		    f.close()

print "Tap off Data" 
f = open('../JasonVerification_deleteme/python_resultsTapOffAugust.csv', 'rt')
for i in sorted(stationNameListD.iterkeys()):
	f.write(i +','+str(stationNameListD[i])+'\n')
	print count,i,stationNameListD[i]
	count+=1
f.close()
print "Tap on Data" 
f = open('../JasonVerification_deleteme/python_resultTapOnAugust.csv', 'rt')
for i in sorted(stationNameList.iterkeys()):
	f.write(i +','+str(stationNameList[i])+'\n')
	print count,i,stationNameList[i]
	count+=1
f.close()
print "Total",totalCount,"RowCount:",Rcount