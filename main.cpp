#include "MissingPuncInterpolation/MissingPuncInterpolation.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    bool succeeded = MissingPuncInterpolation::handle("/media/nlp/Maxtor/Transport/Punctuality/cvm_punctuality_station_data_extract_ver0_2_20160831.csv");
    cout << succeeded << endl;
}
