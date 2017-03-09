#ifndef MISSINGPUNCINTERPOLATION_H
#define MISSINGPUNCINTERPOLATION_H 

#include <string>
class MissingPuncInterpolation {
public:
    static bool handle(std::string fileName);
    
private:
    int m_plannedDprtTimePos;
    int m_plannedArrvTimePos;
    int m_actDprtTimePos;
    int m_actArrvTimePos;
    int m_plannedStopPos;
    int m_actStopPos;
    int m_tripNamePos;
};

#endif
