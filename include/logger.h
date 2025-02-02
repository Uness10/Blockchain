#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

struct Logger {
    string fname;

    Logger() : fname("logs/blockchain.log") {}

    void log(const string& msg) const {
        ofstream logFile(fname, ios::app); 
        string tm = ftimestamp(); 
        if (logFile) {
            logFile << "[" << tm << "] " << msg << endl;
        } else {
            cerr << "[ERROR] Failed to open log file: " << fname << endl;
        }
        cout  << "[" << tm << "] " << msg << endl;
    }


private:
    string ftimestamp() const {
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        tm local_tm{};
        localtime_s(&local_tm, &now_c); // Thread-safe localtime for Windows

        ostringstream oss;
        oss << put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};