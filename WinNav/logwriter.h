#ifndef LOGWRITER_H
#define LOGWRITER_H
#include <iostream>
#include <fstream>

class LogWriter
{
public:
    LogWriter();
    LogWriter(bool reset);
    std::ofstream logFile;
    ~LogWriter();
};

#endif // LOGWRITER_H
