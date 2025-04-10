#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <iomanip>

using namespace std;

class Timer {
public:
    virtual void start() = 0;
    virtual ~Timer() {}
    
protected:
    string convertSecondsToHumanReadableTime(double seconds) {
        if (seconds < 0) {
            return "Invalid time!";
        }

        int hrs = seconds / 3600;
        seconds -= hrs * 3600;
        int mins = seconds / 60;
        seconds -= mins * 60;
        int secs = static_cast<int>(seconds);

        stringstream ss;
        if (hrs > 0) ss << hrs << " hours ";
        if (mins > 0 || hrs > 0) ss << mins << " minutes ";
        ss << secs << " seconds";
        return ss.str();
    }

    double calculateTotalSeconds(double hours, double minutes, double seconds) {
        if (hours < 0 || minutes < 0 || seconds < 0) {
            cout << "Please ensure all inputs are non-negative.\n";
            return 0;
        }
        return (hours * 3600) + (minutes * 60) + seconds;
    }
};

class CountdownTimer : public Timer {
public:
    CountdownTimer(double h, double m, double s) : hours(h), minutes(m), seconds(s) {}

    void start() override {
        double totalSeconds = calculateTotalSeconds(hours, minutes, seconds);
        cout << "\nStarting countdown: " << convertSecondsToHumanReadableTime(totalSeconds) << "\n";

        while (totalSeconds > 0) {
            cout << "\r" << convertSecondsToHumanReadableTime(totalSeconds) << flush;
            usleep(1000000);  // 1 second
            totalSeconds--;
        }

        cout << "\nTime's up!\n";
        handleReset();
    }

private:
    double hours, minutes, seconds;

    void handleReset() {
        char input;
        cout << "Would you like to RESET (R) or QUIT (q)? ";
        cin >> input;

        if (input == 'R' || input == 'r') {
            start();
        }
    }
};

class StopwatchTimer : public Timer {
public:
    void start() override {
        time_t startTime = time(NULL);
        cout << "Stopwatch started. Press Ctrl+C to stop.\n";

        while (true) {
            time_t elapsed = time(NULL) - startTime;
            cout << "\rElapsed Time: " << convertSecondsToHumanReadableTime(elapsed) << flush;
            usleep(1000000);
        }
    }
};

int main() {
    string timerType;
    cout << "Enter timer type (COUNTDOWN or STOPWATCH): ";
    cin >> timerType;

    Timer* timer = nullptr;

    if (timerType == "COUNTDOWN" || timerType == "countdown") {
        double h, m, s;
        cout << "Enter hours: ";
        cin >> h;
        cout << "Enter minutes: ";
        cin >> m;
        cout << "Enter seconds: ";
        cin >> s;

        timer = new CountdownTimer(h, m, s);
    } else if (timerType == "STOPWATCH" || timerType == "stopwatch") {
        timer = new StopwatchTimer();
    } else {
        cout << "Invalid timer type selected.\n";
        return 1;
    }

    timer->start();
    delete timer;

    return 0;
}
