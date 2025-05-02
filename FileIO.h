//
// Created by nicol on 4/29/2024.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
#include <iomanip>

#ifndef PROJECT3_FILEIO_H
#define PROJECT3_FILEIO_H

using namespace std;
class FileIO{

public:
    struct User{
        string name;
        string time_string;
        int time_int;
    };

    FileIO(){

    }

    vector<User> user_list;

    void ReadLeaderBoard() {
        user_list.clear();
        ifstream leadership_file("files/leaderboard.txt");

        User user;
        while (leadership_file >> user.time_string >> user.name) {
            user.time_string = user.time_string.erase(user.time_string.size() - 1);
            user_list.push_back(user);
        }

        cout << "user_list.size(): " << user_list.size() << endl;
        for (int i = 0; i <  user_list.size(); ++i){
            string chars =  user_list[i].time_string;
            int min_val = stoi(chars.substr(0, 2));
            int sec_val = stoi(chars.substr(3, 2));
            user_list[i].time_int = min_val * 60 + sec_val;
        }
    }

    bool isSorted(){
        for(int i = 0; i < user_list.size() - 1; i++){
            if(user_list[i].time_int > user_list[i+1].time_int) return false;
        }

        return true;
    }

    void SortLeaderboard(){
        while(!isSorted()){
            for(int i = 0; i < user_list.size() - 1; i++){
                if(user_list[i].time_int > user_list[i+1].time_int){
                    User temp = user_list[i];
                    user_list[i] = user_list[i+1];
                    user_list[i + 1] = temp;
                }
            }
        }

        user_list.pop_back();
    }

    void AddPlayer(User& user){
        user_list.push_back(user);
        SortLeaderboard();
    }

    vector<int> GetColRowMines(){

        vector<int> col_row_mines;
        ifstream file("files/config.cfg");
        string str;

        while (getline(file, str))
        {
            col_row_mines.push_back(stoi(str));
        }

        return col_row_mines;
    }

    string getFontPath(){
        return "files/font.ttf";
    }

    map<string, Texture*> LoadTextureMap(){
        Texture* smiley = new Texture();
        Texture* debug = new Texture();;
        Texture* play = new Texture();;
        Texture* pause = new Texture();;
        Texture* leaderboard = new Texture();;
        Texture* tile_hidden = new Texture();;
        Texture* digits = new Texture();;
        Texture* mine = new Texture();
        Texture* revealed = new Texture();
        Texture* num1 = new Texture();
        Texture* num2 = new Texture();
        Texture* num3 = new Texture();
        Texture* num4 = new Texture();
        Texture* num5 = new Texture();
        Texture* num6 = new Texture();
        Texture* num7 = new Texture();
        Texture* num8 = new Texture();
        Texture* flag = new Texture();
        Texture* face_lose = new Texture();
        Texture* face_win = new Texture();

        smiley->loadFromFile("files/images/face_happy.png");
        debug->loadFromFile("files/images/debug.png");
        play->loadFromFile("files/images/play.png");
        pause->loadFromFile("files/images/pause.png");
        leaderboard->loadFromFile("files/images/leaderboard.png");
        tile_hidden->loadFromFile("files/images/tile_hidden.png");
        digits->loadFromFile("files/images/digits.png");
        mine->loadFromFile("files/images/mine.png");
        revealed->loadFromFile("files/images/tile_revealed.png");

        num1->loadFromFile("files/images/number_1.png");
        num2->loadFromFile("files/images/number_2.png");
        num3->loadFromFile("files/images/number_3.png");
        num4->loadFromFile("files/images/number_4.png");
        num5->loadFromFile("files/images/number_5.png");
        num6->loadFromFile("files/images/number_6.png");
        num7->loadFromFile("files/images/number_7.png");
        num8->loadFromFile("files/images/number_8.png");
        flag->loadFromFile("files/images/flag.png");
        face_lose->loadFromFile("files/images/face_lose.png");
        face_win->loadFromFile("files/images/face_win.png");

        map<string, Texture*> map;
        map["SMILEY"] = smiley;
        map["DEBUG"] = debug;
        map["PLAY"] = play;
        map["PAUSE"] = pause;
        map["LEADERBOARD"] = leaderboard;
        map["HIDDEN"] = tile_hidden;
        map["DIGITS"] = digits;
        map["MINE"] = mine;
        map["REVEALED"] = revealed;
        map["NUM1"] = num1;
        map["NUM2"] = num2;
        map["NUM3"] = num3;
        map["NUM4"] = num4;
        map["NUM5"] = num5;
        map["NUM6"] = num6;
        map["NUM7"] = num7;
        map["NUM8"] = num8;
        map["FLAG"] = flag;
        map["LOSE"] = face_lose;
        map["WIN"] = face_win;
        return map;
    }

    void WriteLeaderboard(){
        ofstream outfile("files/leaderboard.txt");

        for (const User& user : user_list) {
            int minutes = user.time_int / 60;
            int seconds = user.time_int % 60;
            outfile << setw(2) << setfill('0') << minutes << ":"
                    << setw(2) << setfill('0') << seconds << ", "
                    << user.name << endl;
        }

        outfile.close();
    }


};

#endif //PROJECT3_FILEIO_H
