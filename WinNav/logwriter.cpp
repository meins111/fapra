#include "logwriter.h"

LogWriter::LogWriter()
{
    logFile.open("logfile.log", std::ios::ate | std::ios::out);
    if (!logFile.is_open()){
        printf("Failed to open log file!\n");
    }
}

LogWriter::LogWriter(bool reset)
{
    if (reset) {
        logFile.open("logfile.log", std::ios::out);
        if (!logFile.is_open()){
            printf("Failed to open log file!\n");
        }
        else {
            logFile<<"File new opened.\n";
        }
    }
    else {
        logFile.open("logfile.log", std::ios::app | std::ios::out);
        if (!logFile.is_open()){
            printf("Failed to open log file!\n");
        }
        else {
            logFile<<"==========================================\n";
            logFile<<"======= New log instance created =========\n";
            logFile<<"==========================================\n";
        }
    }
}
LogWriter::~LogWriter() {
    //Make sure everything is written to the file, and close the log
    logFile.flush();
    logFile.close();
}
