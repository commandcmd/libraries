#ifndef BackgroundTimer_HPP
#define BackgroundTimer_HPP

#include <chrono>

class Timer {
    public:
        void set();
        unsigned long timePassed();
        
    private:
        std::chrono::time_point <std::chrono::system_clock> started_time;
};

void Timer::set(){
    started_time = std::chrono::system_clock::now();
}

unsigned long Timer::timePassed(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - started_time).count();
}

#endif