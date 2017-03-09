#include "MissingPuncInterpolation.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool MissingPuncInterpolation::handle(string fileName) {
    if (fileName.length() == 0) {
        cout << "File name is empty.";
        return false;
    }

    ifstream infile;
    infile.open(fileName.c_str());
    if (!infile.is_open()) {
        cout << "File " << fileName << " couldn't be opened.";
        return false;
    }

    string value;
    string line;
    
    int r = 0;
    while (getline(infile, line)) {
        int c = 0;
        stringstream ss(line);
        while (getline(ss, value, ',')) {
            
            cout << r << " " << c << " " << value << endl;
            c++;
        }

        if (r++ > 3) {
            break;
        }
    }
    
    return true;
}
