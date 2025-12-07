// need to get start time with clock

// make function to save log information to a file and name it based on start date and time
// ex. KeoghsPort10_18_2025_0114.txt
// Where the session was first started on Oct 18th in 2025 at 1:14 am

/* LOG VISUAL
10 18 2023: 01:14 Program was started.
...
10 18 2023: 04:23 [04, 01] was moved to [10,02]
...
10 18 2023: 23:13 Program was shut down.
*/

// program runs in background so have a present function to add a detail by user if desired

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;
using namespace chrono;

class Log {
private:
    vector<string> logEntries;          // stores log entries
    tm startTime;                       // stores start time (will be used for writing FINAL log file name)

    tm getCurrentTime() {
        auto present = system_clock::now();                                 // gets curr time for computer to read
        time_t present_time_t = system_clock::to_time_t(present);           // makes into number which gets broken down

        tm timeinfo{};

        // convert int time to readable person time
        #ifdef _WIN32 // for windows
        //    localtime_s(&timeinfo, &present_time_t);                        
        #else // if linux or macOS
            timeinfo = *localtime(&present_time_t); 
        #endif 
        
        return timeinfo;
    }

    string formatTimestamp(const tm& timeinfo) {
        stringstream ss;
        ss << put_time(&timeinfo, "%m %d %Y: %H:%M"); // fixes timeinfo to follow format Month Day Year: Hour:Minute
        return ss.str();
    }

    string generateLogFileName() {
        stringstream ss;
        ss << "KeoghsPort_" << std::put_time(&startTime, "%m_%d_%Y_%H%M") << ".txt";
        return ss.str();
    }

public:
    Log() {
        startTime = getCurrentTime();
        addLogEntry("Program was started.");
    }


    void addLogEntry(const string& logEntry) {
        tm currentTime = getCurrentTime();
        string timestamp = formatTimestamp(currentTime);
        logEntries.push_back(timestamp + " " + logEntry);
    }

    void writeNote(const string& note) {
        addLogEntry(note);
    }

    void saveLog() {
        string fileName = generateLogFileName();
        ofstream outFS(fileName);
        
        // ERROR CHECK: in case file cannot be opened to write on
        if (!outFS.is_open()) {
            cout << "Error: Cannot open log file for writing: " << fileName << endl;
            return;
        }

        for (const string& entry : logEntries) {        // uses the logEntries vector to write each entry to the file
            outFS << entry << endl;
        }

        outFS.close();
    }

    ~Log() {
        addLogEntry("Program was shut down.");
        saveLog();
    }
};



#endif // LOG_H