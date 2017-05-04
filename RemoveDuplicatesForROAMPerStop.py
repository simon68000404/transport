# -*- coding: utf-8 -*-

import csv
import os
import sys
import copy

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
            if filename.endswith(".csv"):
                # Join the two strings in order to form the full filepath.
                filepath = os.path.join(root, filename)
                file_paths.append(filepath)  # Add it to the list.

    return file_paths  # Self-explanatory.

full_file_paths = get_filepaths(sys.argv[1]);
# print(full_file_paths)

for csvfile in full_file_paths:
    print csvfile
    
    distinctSet = set()
    # originalSet = []
    originalList = []
    resultSet = []
    groupSet = []

    with open(csvfile, 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='"')
        originalList = list(reader)

    print "whole list: ", len(originalList)

    for row in originalList:
        row2 = copy.deepcopy(row)
        # print(row2)
        del row2[29] #change the 1 to a different column id
        del row2[28]
        del row2[27]
        del row2[26]
        del row2[23]
        # print row
        row2 = tuple(row2)
        if row2 not in distinctSet:
            distinctSet.add(row2)
            # originalSet.append(row)

    i = 0
    for distinctRow in distinctSet:
        i = i+1
        # if i == 100: break
        # print "current row: ", i
        groupSet = []
        for row in originalList:
            # print row[:23]
            # print distinctRow[:23]
            if row[:23] == list(distinctRow[:23]):
                # print "yes"
                groupSet.append(row)
        boardingCnt = 0
        unboardingCnt = 0
        for row in groupSet:
            # print row
            unboardingCnt += int(row[27])
            boardingCnt += int(row[28])

        # print len(groupSet)
        # if len(groupSet) == 0:
        #     print distinctRow
        tempRow = groupSet[0]
        tempRow[27] = unboardingCnt
        tempRow[28] = boardingCnt

        resultSet.append(tempRow)

    csvfileout = csvfile.split(".");
    # print distinctSet
    with open(csvfileout[0] + "_distinct.csv", 'wb') as csvfileoutName:
        spamwriter = csv.writer(csvfileoutName, quotechar = '"')
        for i in resultSet:
            spamwriter.writerow(i)
            # print i