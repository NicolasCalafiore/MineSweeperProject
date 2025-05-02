//
// Created by nicol on 4/28/2024.
//
#include "Timer.h"
#include "Tile.h"

#ifndef PROJECT3_COUNTER_H
#define PROJECT3_COUNTER_H
class Counter{
    public:

        int count = 0;
        bool showCounter = false;


        Counter(){

        }

        void SetCount(vector<Tile> tiles){
            int flag_count = 0;
            int bomb_count = 0;
            for(auto& i : tiles){
                if(i.isFlagged) flag_count++;
                if(i.hasBomb()) bomb_count++;
            }

            this->count = bomb_count - flag_count;
        }

        int getCount(vector<Tile> tiles){
            return count;
        }

        int getCounterPosition(bool is_hundreds, bool is_tens, bool is_ones){

            int position;
            int count_filtered = this->count;
            if(count_filtered < 0) {
                count_filtered *= -1;
            }

            if(is_ones){
                position =  (count_filtered % 10) * 21;
            }
            if(is_tens){
                position = ((count_filtered/10) % 10) * 21;
            }
            if(is_hundreds){
                ((count_filtered/100) % 10) * 21;
            }

            if(position > 189) position = 0;

            return position;

        }






};
#endif //PROJECT3_COUNTER_H
