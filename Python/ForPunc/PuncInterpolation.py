# -*- coding: utf-8 -*-

import csv
import os
import sys
import copy
from datetime import datetime


print 'Number of arguments:', len(sys.argv), 'arguments.'

def get_filepaths(directory):
    """
    This function will generate the file names in a directory 
    tree by walking the tree either top-down or bottom-up. For each 
    directory in the tree rooted at directory top (including top itself), 
    it yields a 3-tuple (dirpath, dirnames, filenames).
    """
    file_paths = []  # List which will store all of the full filepaths.

    # Walk the tree.
    for root, directories, files in os.walk(directory):
        for filename in files:
            if filename.endswith("_UTS_FIXED.csv"):
                # Join the two strings in order to form the full filepath.
                filepath = os.path.join(root, filename)
                file_paths.append(filepath)  # Add it to the list.

    return file_paths  # Self-explanatory.

full_file_paths = get_filepaths(sys.argv[1]);
# print(full_file_paths)

def putError(tripStops, columnDict, i):
    tripStops[i][columnDict["Actual Stop Station"]] = "TimeConflict"
    # tripStops[i][columnDict["Actual Station Dprt Time"]] = ""
    # tripStops[i][columnDict["Actual Station Arrv Time"]] = ""

def handleTimeExceptions(tripStops, columnDict):
    for j in range(len(tripStops)):
        # copy time if one of dprt or arrv is blank
        if tripStops[j][columnDict["Actual Station Dprt Time"]] == "" and tripStops[j][columnDict["Actual Station Arrv Time"]] != "":
            tripStops[j][columnDict["Actual Station Dprt Time"]] = tripStops[j][columnDict["Actual Station Arrv Time"]]
        if tripStops[j][columnDict["Actual Station Dprt Time"]] != "" and tripStops[j][columnDict["Actual Station Arrv Time"]] == "":
            tripStops[j][columnDict["Actual Station Arrv Time"]] = tripStops[j][columnDict["Actual Station Dprt Time"]]

    delList = []
    for j in range(len(tripStops)):
        # print j, tripStops[j]
        # clean dprt/arrv conflicts in one row
        if True == isNormalRow(tripStops, columnDict, j):
            if False == laterThanDateTime(tripStops[j][columnDict["Actual Station Dprt Time"]], tripStops[j][columnDict["Actual Station Arrv Time"]]):
                if tripStops[j][columnDict["Planned Stop Station"]] == tripStops[j][columnDict["Actual Stop Station"]]:
                    putError(tripStops, columnDict, j)
                else:
                    delList.append(j)
    for k in range(len(delList)):
        del tripStops[delList[k]]

    delList = []
    # print 0, tripStops[0]
    j = 1
    while j < len(tripStops):
        # print j, tripStops[j]
        # clean dprt/arrv conflicts within 2 consecutive rows
        if False == isNormalRow(tripStops, columnDict, j) or False == isNormalRow(tripStops, columnDict, j - 1):
            # print "------------First"
            j += 1
        elif False == laterThanDateTime(tripStops[j][columnDict["Actual Station Arrv Time"]], tripStops[j - 1][columnDict["Actual Station Dprt Time"]]):
            # print "------------Second"
            if tripStops[j - 1][columnDict["Planned Stop Station"]] == tripStops[j - 1][columnDict["Actual Stop Station"]]:
                putError(tripStops, columnDict, j - 1)
            else:
                delList.append(j - 1)
            if tripStops[j][columnDict["Planned Stop Station"]] == tripStops[j][columnDict["Actual Stop Station"]]:
                putError(tripStops, columnDict, j)
            else:
                delList.append(j)
            j += 2
        else:
            # print "------------Third"
            j += 1
    for k in range(len(delList)):
        del tripStops[delList[k]]

def isNotMissingStop(tempList, columnDict, i):
    return tempList[i][columnDict["Actual Stop Station"]] != "Missing"

def isNormalRow(tripStops, columnDict, i):
    return tripStops[i][columnDict["Actual Stop Station"]] != "Missing" and tripStops[i][columnDict["Actual Stop Station"]] != "TimeConflict"

def areAllStopsMissing(tripStops, columnDict):
    bAllMissing = True
    for row in tripStops:
        if row[columnDict["Actual Stop Station"]] != "Missing" and row[columnDict["Actual Stop Station"]] != "TimeConflict":
            bAllMissing = False
            break
    return bAllMissing

def applyPlannedToActual(tripStops, columnDict):
    for row in tripStops:
        if row[columnDict["Actual Stop Station"]] == "Missing":
            row[columnDict["Actual Stop Node"]] = row[columnDict["Planned Stop Node"]]
        # elif row[columnDict["Actual Stop Station"]] == "TimeConflict":
            # do nothing
        row[columnDict["Actual Stop Station"]] = row[columnDict["Planned Stop Station"]]
        row[columnDict["Actual Station Arrv Time"]] = row[columnDict["Planned Station Arrv Time"]]
        row[columnDict["Actual Station Dprt Time"]] = row[columnDict["Planned Station Dprt Time"]]

def getMissingRowsSinceOrig(tripStops, columnDict):
    if tripStops[0][columnDict["Actual Stop Station"]] == "Missing" or tripStops[0][columnDict["Actual Stop Station"]] == "TimeConflict":
        for i in range(0, len(tripStops)):
            if tripStops[i][columnDict["Actual Stop Station"]] != "Missing" and tripStops[i][columnDict["Actual Stop Station"]] != "TimeConflict":
                return i
    return 0

def substractDateTime(strDateTime1, strDateTime2):
    return datetime.strptime(strDateTime1, '%d/%m/%Y %H:%M:%S') - datetime.strptime(strDateTime2, '%d/%m/%Y %H:%M:%S')

def substractTimeDelta(strDateTime, timeDelta):
    return datetime.strptime(strDateTime, '%d/%m/%Y %H:%M:%S') - timeDelta

def addDateTime(strDateTime1, strDateTime2):
    return datetime.strptime(strDateTime1, '%d/%m/%Y %H:%M:%S') + datetime.strptime(strDateTime2, '%d/%m/%Y %H:%M:%S')

def addTimeDelta(strDateTime, timeDelta):
    return datetime.strptime(strDateTime, '%d/%m/%Y %H:%M:%S') + timeDelta

def dateTimeToString(dateTime):
    return dateTime.strftime("%d/%m/%Y %H:%M:%S")

def interpolateHeadMissingRows(tripStops, columnDict, ifirstNotMissingRow):
    i = ifirstNotMissingRow
    while i > 0:
        # print i, len(tripStops)
        if i == 1:
            ADDiff = substractDateTime(tripStops[1][columnDict["Planned Station Arrv Time"]], tripStops[0][columnDict["Planned Station Dprt Time"]])
            tripStops[0][columnDict["Actual Station Dprt Time"]] = dateTimeToString(substractTimeDelta(tripStops[1][columnDict["Actual Station Arrv Time"]], ADDiff))
        else:
            ADDiff = substractDateTime(tripStops[i][columnDict["Planned Station Arrv Time"]], tripStops[i - 1][columnDict["Planned Station Dprt Time"]])
            DADiff = substractDateTime(tripStops[i - 1][columnDict["Planned Station Dprt Time"]], tripStops[i - 1][columnDict["Planned Station Arrv Time"]])
            tripStops[i - 1][columnDict["Actual Station Dprt Time"]] = dateTimeToString(substractTimeDelta(tripStops[i][columnDict["Actual Station Arrv Time"]], ADDiff))
            tripStops[i - 1][columnDict["Actual Station Arrv Time"]] = dateTimeToString(substractTimeDelta(tripStops[i - 1][columnDict["Actual Station Dprt Time"]], DADiff))
        tripStops[i - 1][columnDict["Actual Stop Station"]] = tripStops[i - 1][columnDict["Planned Stop Station"]]
        tripStops[i - 1][columnDict["Actual Stop Node"]] = tripStops[i - 1][columnDict["Planned Stop Node"]]
        i -= 1

def interpolateTailMissingRows(tripStops, columnDict, iLastNotMissingRow):
    i = iLastNotMissingRow
    while i < len(tripStops) - 1:
        print i, len(tripStops)
        if i == len(tripStops) - 2:
            ADDiff = substractDateTime(tripStops[i + 1][columnDict["Planned Station Arrv Time"]], tripStops[i][columnDict["Planned Station Dprt Time"]])
            tripStops[i + 1][columnDict["Actual Station Arrv Time"]] = dateTimeToString(addTimeDelta(tripStops[i][columnDict["Actual Station Dprt Time"]], ADDiff))
        else:
            ADDiff = substractDateTime(tripStops[i + 1][columnDict["Planned Station Arrv Time"]], tripStops[i][columnDict["Planned Station Dprt Time"]])
            DADiff = substractDateTime(tripStops[i + 1][columnDict["Planned Station Dprt Time"]], tripStops[i + 1][columnDict["Planned Station Arrv Time"]])
            tripStops[i + 1][columnDict["Actual Station Arrv Time"]] = dateTimeToString(addTimeDelta(tripStops[i][columnDict["Actual Station Dprt Time"]], ADDiff))
            tripStops[i + 1][columnDict["Actual Station Dprt Time"]] = dateTimeToString(addTimeDelta(tripStops[i + 1][columnDict["Actual Station Arrv Time"]], DADiff))
        tripStops[i + 1][columnDict["Actual Stop Station"]] = tripStops[i + 1][columnDict["Planned Stop Station"]]
        tripStops[i + 1][columnDict["Actual Stop Node"]] = tripStops[i + 1][columnDict["Planned Stop Node"]]
        i += 1

def getMissingRowsTillDest(tripStops, columnDict):
    if tripStops[len(tripStops) - 1][columnDict["Actual Stop Station"]] == "Missing" or tripStops[len(tripStops) - 1][columnDict["Actual Stop Station"]] == "TimeConflict":
        for i in range(len(tripStops) - 1, -1, -1):
            if tripStops[i][columnDict["Actual Stop Station"]] != "Missing" and tripStops[i][columnDict["Actual Stop Station"]] != "TimeConflict":
                return len(tripStops) - 1 - i
    return 0

def interpolateNormalMissingRows(tripStops, columnDict, preNotMissingIndex, nextNotMissingIndex):
    prePlannedDprtNotMissingTime = tempList[preNotMissingIndex][columnDict["Planned Station Dprt Time"]]
    nextPlannedDprtNotMissingTime = tempList[nextNotMissingIndex][columnDict["Planned Station Dprt Time"]]
    preActDprtNotMissingTime = tempList[preNotMissingIndex][columnDict["Actual Station Dprt Time"]]
    nextActDprtNotMissingTime = tempList[nextNotMissingIndex][columnDict["Actual Station Dprt Time"]]

    prePlannedArrvNotMissingTime = tempList[preNotMissingIndex][columnDict["Planned Station Arrv Time"]]
    nextPlannedArrvNotMissingTime = tempList[nextNotMissingIndex][columnDict["Planned Station Arrv Time"]]
    preActArrvNotMissingTime = tempList[preNotMissingIndex][columnDict["Actual Station Arrv Time"]]
    nextActArrvNotMissingTime = tempList[nextNotMissingIndex][columnDict["Actual Station Arrv Time"]]

    plannedDprtTimeDiff = float(nextPlannedDprtNotMissingTime - prePlannedDprtNotMissingTime)
    plannedArrvTimeDiff = float(nextPlannedArrvNotMissingTime - prePlannedArrvNotMissingTime)
    actDprtTimeDiff = float(nextActDprtNotMissingTime - preActDprtNotMissingTime)
    actArrvTimeDiff = float(nextActArrvNotMissingTime - preActArrvNotMissingTime)    
    
    for k in range(preNotMissingIndex + 1, nextNotMissingIndex):
        dprtRatio = float(tempList[k][columnDict["Planned Station Dprt Time"]]) / plannedDprtTimeDiff
        arrvRatio = float(tempList[k][columnDict["Planned Station Arrv Time"]]) / plannedArrvTimeDiff
        tempList[k][columnDict["Actual Station Dprt Time"]] = actDprtTimeDiff * (1 + dprtRatio)
        tempList[k][columnDict["Actual Station Arrv Time"]] = actArrvTimeDiff * (1 + arrvRatio)

def laterThanDateTime(strTime1, strTime2):
    time1 = datetime.strptime(strTime1, '%d/%m/%Y %H:%M:%S')
    time2 = datetime.strptime(strTime2, '%d/%m/%Y %H:%M:%S')
    return time1 >= time2

def swapActualDprtArrv(tripStops, columnDict, i):
    temp = tripStops[i][columnDict["Actual Station Arrv Time"]]
    tripStops[i][columnDict["Actual Station Arrv Time"]] = tripStops[i][columnDict["Actual Station Dprt Time"]]
    tripStops[i][columnDict["Actual Station Dprt Time"]] = temp

for csvfile in full_file_paths:
    print csvfile
    newSet = set()
    originalSet = []
    columnDict = {}
    tripDict = {}
    with open(csvfile, 'rb') as f:
        reader = csv.reader(f, delimiter=',')
        tempList = list(reader)
        listLength = len(tempList)
        for colIndex in range(len(tempList[0])):
            columnDict[tempList[0][colIndex]] = colIndex
        # print tempList[100][columnDict["Actual Stop Station"]]

        i = 1
        while i < listLength:
            # if tempList[i][columnDict["Trip Name"]] == tempList[i - 1][columnDict["Trip Name"]]:
            tup = tuple([tempList[i][columnDict["Service Date"]], tempList[i][columnDict["Trip Name"]]])
            if tup not in tripDict:
                tripDict[tup] = []
            tripDict[tup].append(tempList[i])
            i += 1

        # swap reversed dprt and arrv
        # i = 0
        # for tup in tripDict:
        #     tripStops = tripDict[tup]
        #     for j in range(len(tripStops)):
        #         if tripStops[j][columnDict["Actual Station Dprt Time"]] != "" and tripStops[j][columnDict["Actual Station Arrv Time"]] != "":
        #             if False == laterThanDateTime(tripStops[j][columnDict["Actual Station Dprt Time"]], tripStops[j][columnDict["Actual Station Arrv Time"]]):
        #                 swapActualDprtArrv(tripStops, columnDict, j)
        #     i += 1

        i = 0
        for tup in tripDict:
            tripStops = tripDict[tup]
            handleTimeExceptions(tripStops, columnDict)
        # interpolation
        i = 0
        for tup in tripDict:
            tripStops = tripDict[tup]
            iOrig = 0
            iDest = len(tripStops)
            if areAllStopsMissing(tripStops, columnDict):
                applyPlannedToActual(tripStops, columnDict)
                # print tup
            else:
                nMissingSinceOrig = getMissingRowsSinceOrig(tripStops, columnDict)
                nMissingTillDest = getMissingRowsTillDest(tripStops, columnDict)

                if nMissingSinceOrig > 0:
                    print "HEAD", nMissingSinceOrig, tup
                    interpolateHeadMissingRows(tripStops, columnDict, nMissingSinceOrig)

                if nMissingTillDest > 0:
                    print "TAIL", nMissingTillDest, tup
                    interpolateTailMissingRows(tripStops, columnDict, len(tripStops) - 1 - nMissingTillDest)

                # interpolateNormalMissingRows(tripStops)


            # bAllMissing = False
            # i = 1
            # while i < len(tripStops):
            #     if False == laterThanDateTime(tripStops[i][columnDict["Planned Station Arrv Time"]], tripStops[i - 1][columnDict["Planned Station Dprt Time"]]):
            #         print "Planned Station arrv time earlier than previous dprt", tripStops[i]
            #     i += 1
            # i = 0
            # while i < len(tripStops):
            #     if False == laterThanDateTime(tripStops[i][columnDict["Planned Station Dprt Time"]], tripStops[i][columnDict["Planned Station Arrv Time"]]):
            #         print "Planned station dprt time earlier than arrv", tripStops[i]
            #     i += 1

            # for row in tripStops:
            #     if row[columnDict["Actual Stop Station"]] != "Missing":
            #         bAllMissing = True
            #         break
            # if bAllMissing == False:
            #     print tup

        # currTripName = ""
        # preTripName = ""
        # i = 1
        # while i < listLength:
        #     if isNotMissingStop(tempList, columnDict, i):
        #         if tempList[i][columnDict["Actual Station Dprt Time"]] == "" and tempList[i][columnDict["Actual Station Arrv Time"]] != "":
        #             tempList[i][columnDict["Actual Station Dprt Time"]] = tempList[i][columnDict["Actual Station Arrv Time"]]
        #             # print "Actual Dprt blank row: ", i, " ", tempList[i]
        #         elif tempList[i][columnDict["Actual Station Arrv Time"]] == "" and tempList[i][columnDict["Actual Station Dprt Time"]] != "":
        #             tempList[i][columnDict["Actual Station Arrv Time"]] = tempList[i][columnDict["Actual Station Dprt Time"]]
        #             # print "Actual Arrv blank row: ", i, " ", tempList[i]
        #         # elif tempList[i][columnDict["Actual Station Dprt Time"]] == "" and tempList[i][columnDict["Actual Station Arrv Time"]] == "":
        #         #     print "both blank row: ", i, " ", tempList[i] # printed, no results
        #         preNotMissingIndex = i
        #         i += 1
        #     else:
        #         firstMissingIndex = i

        #         # tempMissinglist.append(i)
        #         j = i
        #         while tempList[j][columnDict["Actual Stop Station"]] == "Missing":
        #             # tempMissinglist.append(i)
        #             j += 1
        #         lastMissingIndex = j - 1
        #         nextNotMissingIndex = j

        #         # if firstMissingIndex is first in the trip:
        #         #     print error
        #         # if lastMissingIndex is last in the trip:
        #         #     print error
        #         if firstMissingIndex - 1 > 0 and tempList[firstMissingIndex][columnDict["Trip Name"]] != tempList[firstMissingIndex - 1][columnDict["Trip Name"]]:
        #             print "error: first stop is missing, row:", firstMissingIndex
        #         if lastMissingIndex + 1 < listLength and tempList[lastMissingIndex][columnDict["Trip Name"]] != tempList[lastMissingIndex + 1][columnDict["Trip Name"]]:
        #             print "error: last stop is missing, row:", lastMissingIndex
                
        #         # prePlannedDprtNotMissingTime = tempList[preNotMissingIndex][columnDict["Planned Station Dprt Time"]]
        #         # nextPlannedDprtNotMissingTime = tempList[nextNotMissingIndex][columnDict["Planned Station Dprt Time"]]
        #         # preActDprtNotMissingTime = tempList[preNotMissingIndex][columnDict["Actual Station Dprt Time"]]
        #         # nextActDprtNotMissingTime = tempList[nextNotMissingIndex][columnDict["Actual Station Dprt Time"]]

        #         # prePlannedArrvNotMissingTime = tempList[preNotMissingIndex][columnDict["Planned Station Arrv Time"]]
        #         # nextPlannedArrvNotMissingTime = tempList[nextNotMissingIndex][columnDict["Planned Station Arrv Time"]]
        #         # preActArrvNotMissingTime = tempList[preNotMissingIndex][columnDict["Actual Station Arrv Time"]]
        #         # nextActArrvNotMissingTime = tempList[nextNotMissingIndex][columnDict["Actual Station Arrv Time"]]

        #         # plannedDprtTimeDiff = float(nextPlannedDprtNotMissingTime - prePlannedDprtNotMissingTime)
        #         # plannedArrvTimeDiff = float(nextPlannedArrvNotMissingTime - prePlannedArrvNotMissingTime)
        #         # actDprtTimeDiff = float(nextActDprtNotMissingTime - preActDprtNotMissingTime)
        #         # actArrvTimeDiff = float(nextActArrvNotMissingTime - preActArrvNotMissingTime)    
                
        #         # for k in range(firstMissingIndex, lastMissingIndex)
        #         #     dprtRatio = float(tempList[k][columnDict["Planned Station Dprt Time"]]) / plannedDprtTimeDiff
        #         #     arrvRatio = float(tempList[k][columnDict["Planned Station Arrv Time"]]) / plannedArrvTimeDiff
        #         #     tempList[k][columnDict["Actual Station Dprt Time"]] = actDprtTimeDiff * (1 + dprtRatio)
        #         #     tempList[k][columnDict["Actual Station Arrv Time"]] = actArrvTimeDiff * (1 + arrvRatio)

        #         i = nextNotMissingIndex

        csvfileout = csvfile.split(".")
        # print newSet
        with open(csvfileout[0] + "_interpolated.csv", 'wb') as csvfileoutName:
            spamwriter = csv.writer(csvfileoutName, quotechar = "'")
            for tup in tripDict:
                tripStops = tripDict[tup]
                for i in tripStops:
                    spamwriter.writerow(i)
                # print i