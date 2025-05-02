//
// Created by nicol on 4/28/2024.
//
#include <SFML/Graphics.hpp>
#include <chrono>

#ifndef PROJECT3_TIMER_H
#define PROJECT3_TIMER_H

using namespace sf;
using namespace std;
class Timer{

    private:
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::chrono::time_point<std::chrono::steady_clock> last_check;
        int pause_amount = 0;

    public:
        Timer(){
            start_time = std::chrono::steady_clock::now();
            last_check = std::chrono::steady_clock::now();
        }

        int getTotalTime() const {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
            return elapsed_seconds.count() - pause_amount;
        }

        bool hasIncrementedBy(int increment){
            if(getTotalTime() % increment == 0) return true;

            return false;
        }

        bool hasElapsed(int seconds){
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_check);
            if(elapsed_seconds.count() >= seconds){
                last_check = std::chrono::steady_clock::now();
                return true;
            };

            return false;
        }

        void SetPause(bool isPaused){
            if(isPaused) pause_amount++;
        }
        int TimerSpritePositionOnes(bool isMinutes){

            int total = getTotalTime();
            if(isMinutes) total /= 60;

            int position = (total % 10) * 21;

            if(position > 189) position = 0;

            return position;
        }
    int TimerSpritePositionTen(bool isMinutes) {
        int total = getTotalTime();
        if (isMinutes) {
            total /= 60;
        }

        int position = ((total / 10) % 10) * 21;

        if (!isMinutes) {
            if (position > 105) {
                position = ((position - 105) % 105) - 21; // Adjust the position
            }

            if(position > 189) position = 0;
        }

        return position;
    }






};
#endif //PROJECT3_TIMER_H
