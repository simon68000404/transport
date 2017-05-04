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
    newSet = set()
    originalSet = []
    with open(csvfile, 'rb') as f:
        reader = csv.reader(f, delimiter=',', quotechar='|')
        tempList = list(reader)
        for row in tempList:
            # row2 = copy.deepcopy(row)
            # # print(row2)
            # del row2[26] #change the 1 to a different column id
            # del row2[19]
            # print row
            tup = (row[0], row[1], row[3], row[4], row[7], row[11], row[12], row[14], row[15], row[16], row[17], row[18], row[19], row[20], row[21])
            if tup not in newSet:
                newSet.add(tup)
                originalSet.append(row)

    csvfileout = csvfile.split(".");
    # print newSet
    with open(csvfileout[0] + "_distinct.csv", 'wb') as csvfileoutName:
        spamwriter = csv.writer(csvfileoutName, quotechar = "'")
        for i in originalSet:
            spamwriter.writerow(i)
            # print i