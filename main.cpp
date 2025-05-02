#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "TextField.h"
#include "Button.h"
#include "Tile.h"
#include <map>
#include <chrono>
#include "Timer.h"
#include "UIRenderer.h"
#include "Counter.h"
#include "FileIO.h"
#include <string>

using namespace sf;
using namespace std;

string CreateWelcomeWindow(float height, float width, Color welcome_background, Font& font);
bool CreateGameWindow(float height, float width, Color welcome_background, Font& font, vector<int> col_row_mines, map<string, Texture*>& texture_map, map<string, Sprite>& sprite_map, string name, FileIO& file);

Text CreateText(Font& font, const string& value, int size, Text::Style style, Color color, int height, int width);
vector<Tile> GenerateTiles(int rows, int columns, int window_width, int window_height, map<string, Texture*>& texture_map);
void GenerateBombs(vector<Tile>& tiles, int bomb_amount);
map<string, Sprite> InitializeGUIObjects(map<string, Texture*>& texture_map, vector<int> col_row_mines);
void HandleEvents(Event& event, RenderWindow& game_window, vector<Tile>& tiles, map<string, Sprite>& sprite_map, map<string, Texture*>& texture_map, Button& debug_button, Button& smiley, Button& play_button, Button& leaderboard_button, bool& isDebug, bool& isGameOver, bool& isPaused, vector<int> col_row_mines, Font& font, string name, FileIO& file, Timer& timer, bool& restartRequested);
void ShowMines(bool show, map<string, sf::Texture*>& texture_map, vector<Tile>& tiles);
void AssignNeighbors(vector<Tile>& tiles, int rows, int columns);
void GameLost(map<string, Texture*>& texture_map, map<string, Sprite>& sprite_map, Button& smiley);
void GameWon(vector<Tile> tiles, map<string, Texture*>& texture_map, Button& smiley, string name, Timer& timer, FileIO& file);
bool hasGameWon(vector<Tile>& tiles);
void GameOver(bool& isGameOver, vector<Tile>& tiles, bool& isPaused, map<string, Texture*>& texture_map, bool& isDebug, Button& pause_button);
void GamePaused(vector<Tile>& tiles, map<string, Texture*>& texture_map, Button& play_button, bool& isPaused);
void CreateLeaderboardWindow(vector<int> col_row_mines, Font& font, string name, FileIO& file, Timer& timer);
string FormatAllPlayers(const std::vector<FileIO::User>& user_list, const std::string& name);
string FormatPlayer(const FileIO::User& player, int index, const std::string& current_name);
void RegisterLeaderboardOpportunity(string name, Timer& timer, FileIO& file);

int main() {

    FileIO file;

    const Color welcome_background(sf::Color::Blue);
    const Color game_background(sf::Color::White);
    const vector<int> col_row_mines = file.GetColRowMines();
    cout << col_row_mines[0] << endl;
    cout << col_row_mines[1] << endl;
    cout << col_row_mines[2] << endl;
    const int main_height = (col_row_mines[1] * 32) + 100;
    const int main_width = col_row_mines[0] * 32;

    map<string, Texture*> texture_map = file.LoadTextureMap();
    map<string, Sprite> sprite_map = InitializeGUIObjects(texture_map, col_row_mines);

    Font font;
    font.loadFromFile(file.getFontPath());
    string name;
    bool restarting = true;
    name = CreateWelcomeWindow(main_height, main_width, welcome_background, font);

    while(restarting){
        restarting = CreateGameWindow(main_height, main_width, game_background, font, col_row_mines, texture_map, sprite_map, name, file);
    }

    return 0;
}

string CreateWelcomeWindow(float height, float width, Color welcome_background, Font& font){

    RenderWindow welcome_window(VideoMode(width, height), "Minesweeper", Style::Close);

    Text title = CreateText(font, "WELCOME TO MINESWEEPER!", 24, Text::Style::Bold, Color::White,width/2, height/2 - 150);
    title.setStyle(sf::Text::Underlined);
    Text name_title = CreateText(font, "Enter your name:", 20, Text::Style::Bold, Color::White, width/2, height/2 - 75);
    TextField name_field(font, "|", 18, Text::Style::Bold, Color::Yellow, width/2, height/2 - 45);


    while(welcome_window.isOpen()) {
        Event event;
        while(welcome_window.pollEvent(event)) {
            if(event.type == Event::Closed) {
                welcome_window.close();
            }

            if(event.type == Event::TextEntered){
                if(std::isalpha(event.key)) {  //if is not backspace
                    name_field.append(event.key);
                }
            }

            if(event.type == Event::KeyPressed){
                if(event.key == sf::Keyboard::BackSpace){
                    name_field.backspace();
                }
            }

            if(event.type == Event::KeyPressed){
                if(event.key == sf::Keyboard::Enter){
                    if(name_field.getSize() == 0) continue;
                    welcome_window.close();
                    string name =  name_field.getText().getString().toAnsiString();
                    name.erase(name.size() - 1);
                    return name;
                }
            }
        }

        welcome_window.clear(welcome_background);
        welcome_window.draw(title);
        welcome_window.draw(name_title);
        welcome_window.draw(name_field.getText());
        welcome_window.display();
    }
}
bool CreateGameWindow(float height, float width, Color welcome_background, Font& font, vector<int> col_row_mines, map<string, Texture*>& texture_map,  map<string, Sprite>& sprite_map, string name, FileIO& file){
    RenderWindow game_window(VideoMode(width, height), "Minesweeper", Style::Close);
    auto start_time = chrono::steady_clock::now();

    Timer game_timer;
    Counter counter;

    file.ReadLeaderBoard();

    Button smiley_button(50, 50, (col_row_mines[0]/2 * 32) - 32, 32 * (col_row_mines[1] + .5), texture_map["SMILEY"]);
    sprite_map["SMILEY"] = smiley_button.getPrimarySprite();

    Button debug_button(50, 50, (col_row_mines[0] * 32) - 304, 32 * (col_row_mines[1] + .5), texture_map["DEBUG"]);
    sprite_map["DEBUG"] = debug_button.getPrimarySprite();

    Button play_button(50, 50, (col_row_mines[0] * 32) - 240, 32 * (col_row_mines[1] + .5), texture_map["PLAY"]);
    sprite_map["PLAY"] = play_button.getPrimarySprite();

    Button leaderboard_button(50, 50, (col_row_mines[0] * 32) - 176, 32 * (col_row_mines[1] + .5), texture_map["LEADERBOARD"]);
    sprite_map["LEADERBOARD"] = leaderboard_button.getPrimarySprite();

    vector<Tile> tiles = GenerateTiles(col_row_mines[1], col_row_mines[0], width, height, texture_map);
    GenerateBombs(tiles,  col_row_mines[2]);
    counter.SetCount(tiles);
    AssignNeighbors(tiles, col_row_mines[1], col_row_mines[0]);
    for(auto& i : tiles){
        i.CalculateMineNumber();
    }

    for(auto& i : tiles){
        i.setSecondarySprite(texture_map["HIDDEN"]);
        i.setSecondarySprite(texture_map["HIDDEN"]);
        i.secondarySpriteNull = true;
        i.tertiarySpriteNull = true;
    }

    bool isDebug = false;
    bool isGameOver = false;
    bool isPaused = false;
    bool restartRequested = false;


    while(game_window.isOpen()) {
        if(restartRequested) return true;
        sprite_map["SMILEY"] = smiley_button.getPrimarySprite();
        sprite_map["PLAY"] = play_button.getPrimarySprite();

        if(counter.getCount(tiles) >= 0) counter.showCounter = false;
        else {
            counter.showCounter = true;
            sprite_map["COUNTER_SIGN"].setTexture(*texture_map.at("DIGITS"));
            sprite_map["COUNTER_SIGN"].setTextureRect(sf::IntRect(210, 0, 21, 32));
        }


        Event event;
        while(game_window.pollEvent(event)) {
            HandleEvents(event, game_window, tiles, sprite_map, texture_map, debug_button, smiley_button, play_button, leaderboard_button, isDebug, isGameOver, isPaused, col_row_mines, font, name, file, game_timer, restartRequested);
            counter.SetCount(tiles);
        }

        if(game_timer.hasElapsed(1)){
            UIRenderer::TimerRendering(sprite_map, game_timer);
            game_timer.SetPause(isPaused);
        }



        //DISPLAY GUI OBJECTS
        game_window.clear(welcome_background);
        UIRenderer::TimerRendering(sprite_map, game_timer);
        UIRenderer::TileRendering(game_window, tiles, texture_map, isDebug);
        UIRenderer::GameCounterRenering(game_window, sprite_map, counter);
        UIRenderer::DigitAndButtonSprites(game_window, sprite_map, texture_map, counter);

        game_window.display();
    }

    //NEED TO DELETE TEXTURE POINTERS IN MAP!!!
}
void CreateLeaderboardWindow(vector<int> col_row_mines, Font& font, string name, FileIO& file, Timer& timer){
    float leaderboard_height = col_row_mines[1] * 16 + 50;
    float leaderboard_width = col_row_mines[0] * 16;

    float leaderboard_title_x = (leaderboard_width / 2);
    float leaderboard_title_y = ((leaderboard_height / 2) - 120);

    float leaderboard_players_x = (leaderboard_width / 2);
    float leaderboard_players_y = ((leaderboard_height / 2) + 20);

    sf::RenderWindow leaderboard_window(sf::VideoMode(leaderboard_width, leaderboard_height), "Leaderboard", sf::Style::Close);

    Text leaderboard_title = CreateText(font, "LEADERBOARD", 20, Text::Style::Bold, Color::White, leaderboard_title_x, leaderboard_title_y);
    leaderboard_title.setStyle(Text::Underlined);

    string players_string = FormatAllPlayers(file.user_list, name);

    Text players = CreateText(font, players_string, 18, Text::Style::Bold, Color::White, leaderboard_players_x, leaderboard_players_y);

    while (leaderboard_window.isOpen()) {
        sf::Event lb_event;

        if(timer.hasElapsed(1)){
            timer.SetPause(true);
        }

        while (leaderboard_window.pollEvent(lb_event)) {
            if (lb_event.type == sf::Event::Closed) {
                leaderboard_window.close();
            }
        }
        leaderboard_window.clear(sf::Color::Blue);
        leaderboard_window.draw(leaderboard_title);
        leaderboard_window.draw(players);
        leaderboard_window.display();
    }
}



void HandleEvents(Event& event, RenderWindow& game_window, vector<Tile>& tiles, map<string, Sprite>& sprite_map, map<string, Texture*>& texture_map, Button& debug_button, Button& smiley, Button& play_button, Button& leaderboard_button, bool& isDebug, bool& isGameOver, bool& isPaused, vector<int> col_row_mines, Font& font, string name, FileIO& file, Timer& timer, bool& restartRequested){
    if(event.type == Event::Closed) {
        game_window.close();

        for (auto const &texture: texture_map) {
                delete texture.second;
        }

        if(!restartRequested) exit(0);
    }

    if(event.type == sf::Event::MouseButtonPressed){

       if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
           for(auto& i : tiles){
               if(i.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                   i.setFlag(texture_map);
               }
           }

           if(hasGameWon(tiles)){
               GameWon(tiles, texture_map, smiley, name, timer, file);
               GameOver(isGameOver, tiles, isPaused, texture_map, isDebug, play_button);
           }
        }
       else {
           for (auto &i: tiles) {
               if (i.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                   if(isPaused) return;
                   if(!isPaused) i.setPrimarySprite(texture_map["REVEALED"]);

                   i.Clicked(texture_map);
                   if(i.hasBomb()){
                       GameLost(texture_map, sprite_map, smiley);
                       GameOver(isGameOver, tiles, isPaused, texture_map, isDebug, play_button);
                   }
                   if(hasGameWon(tiles)){
                       GameWon(tiles, texture_map, smiley, name, timer, file);
                       GameOver(isGameOver, tiles, isPaused, texture_map, isDebug, play_button);
                   }
               }
           }

           if (debug_button.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

               if(debug_button.isClickable && !isPaused){
                   isDebug = !isDebug;

                   if (isDebug) {
                       ShowMines(isDebug, texture_map, tiles);
                   }
               }


           }

           if (play_button.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
               if(play_button.isClickable){
                   GamePaused(tiles, texture_map, play_button, isPaused);
                   isDebug = false;
               }
           }
//Button& debug_button, Button& smiley, Button& play_button, Button& leaderboard_button
           if (leaderboard_button.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
               debug_button.isClickable = !debug_button.isClickable;
               smiley.isClickable = !smiley.isClickable;
               play_button.isClickable = !play_button.isClickable;


               CreateLeaderboardWindow(col_row_mines, font, name, file, timer);

               debug_button.isClickable = !debug_button.isClickable;
               smiley.isClickable = !smiley.isClickable;
               play_button.isClickable = !play_button.isClickable;
           }

           if (smiley.getPrimarySprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                if(smiley.isClickable) restartRequested = true;
           }


       }
    }


}
void GameLost(map<string, Texture*>& texture_map, map<string, Sprite>& sprite_map, Button& smiley){
    smiley.setPrimarySprite(texture_map["LOSE"]);
}
void GameWon(vector<Tile> tiles, map<string, Texture*>& texture_map, Button& smiley, string name, Timer& timer, FileIO& file){
    RegisterLeaderboardOpportunity(name, timer, file);
    smiley.setPrimarySprite(texture_map["WIN"]);
}
void GameOver(bool& isGameOver, vector<Tile>& tiles, bool& isPaused, map<string, Texture*>& texture_map, bool& isDebug, Button& pause_button){
    isGameOver = true;
    isPaused = true;
    isDebug = true;
    ShowMines(true, texture_map, tiles);
    pause_button.isClickable = false;
    for(auto& i : tiles){
        i.isClickable = false;
    }

}
void GamePaused(vector<Tile>& tiles, map<string, Texture*>& texture_map, Button& play_button, bool& isPaused){
    isPaused = !isPaused;
    if(isPaused) {
        play_button.setPrimarySprite(texture_map["PAUSE"]);
        for(auto& i : tiles){
            i.setPrimarySprite(texture_map["REVEALED"]);
            i.setSecondarySprite(texture_map["HIDDEN"]);
            i.setTertiarySprite(texture_map["HIDDEN"]);
            i.secondarySpriteNull = true;
            i.tertiarySpriteNull = true;
            i.isClickable = false;
        }

    }
    else{
        play_button.setPrimarySprite(texture_map["PLAY"]);
        for(auto& i : tiles){
            i.isClickable = true;
            i.AutomaticallySetSecondarySprite(texture_map);
            i.secondarySpriteNull = false;
            if(i.isClicked) i.setPrimarySprite(texture_map["REVEALED"]);
            else i.setPrimarySprite(texture_map["HIDDEN"]);
            if(i.isFlagged){
                i.setTertiarySprite(texture_map["FLAG"]);
                i.tertiarySpriteNull = true;
            }
        }
    }
}

map<string, Sprite> InitializeGUIObjects(map<string, Texture*>& texture_map, vector<int> col_row_mines){
    map<string, Sprite> gui_map;

    float time_min_x = ((col_row_mines[0] * 32) - 97);
    float time_min_y = ((col_row_mines[1] + 0.5) * 32 + 16);

    float time_sec_x = ((col_row_mines[0] * 32) - 54);
    float time_sec_y = ((col_row_mines[1] + 0.5) * 32 + 16);

    sf::Sprite timer_sprite_one_sec;
    timer_sprite_one_sec.setTexture(*texture_map.at("DIGITS"));
    timer_sprite_one_sec.setTextureRect(sf::IntRect(0, 0, 21, 32));
    timer_sprite_one_sec.setPosition(time_sec_x + 21, time_sec_y);
    gui_map["TIME_SEC_ONES"] = timer_sprite_one_sec;

    sf::Sprite timer_sprite_ten_sec;
    timer_sprite_ten_sec.setTexture(*texture_map.at("DIGITS"));
    timer_sprite_ten_sec.setTextureRect(sf::IntRect(0, 0, 21, 32));
    timer_sprite_ten_sec.setPosition(time_sec_x, time_sec_y);
    gui_map["TIME_SEC_TENS"] = timer_sprite_ten_sec;

    sf::Sprite timer_sprite_one_min;
    timer_sprite_one_min.setTexture(*texture_map.at("DIGITS"));
    timer_sprite_one_min.setTextureRect(sf::IntRect(0, 0, 21, 32));
    timer_sprite_one_min.setPosition(time_min_x + 21, time_min_y);
    gui_map["TIME_MIN_ONES"] = timer_sprite_one_min;

    sf::Sprite timer_sprite_ten_min;
    timer_sprite_ten_min.setTexture(*texture_map.at("DIGITS"));
    timer_sprite_ten_min.setTextureRect(sf::IntRect(0, 0, 21, 32));
    timer_sprite_ten_min.setPosition(time_min_x, time_min_y);
    gui_map["TIME_MIN_TENS"] = timer_sprite_ten_min;

    float counter_x = 33;
    float counter_y = ((col_row_mines[1] + 0.5) * 32 + 16);

    sf::Sprite counter_hundreds;
    counter_hundreds.setTexture(*texture_map.at("DIGITS"));
    counter_hundreds.setTextureRect(sf::IntRect(0, 0, 21, 32));
    counter_hundreds.setPosition(counter_x, counter_y);
    gui_map["COUNTER_HUNDREDS"] = counter_hundreds;

    sf::Sprite counter_tenths;
    counter_tenths.setTexture(*texture_map.at("DIGITS"));
    counter_tenths.setTextureRect(sf::IntRect(0, 0, 21, 32));
    counter_tenths.setPosition(counter_x + 21, counter_y);
    gui_map["COUNTER_TENS"] = counter_tenths;

    sf::Sprite counter_ones;
    counter_ones.setTexture(*texture_map.at("DIGITS"));
    counter_ones.setTextureRect(sf::IntRect(0, 0, 21, 32));
    counter_ones.setPosition(counter_x + 42, counter_y);
    gui_map["COUNTER_ONES"] = counter_ones;

    sf::Sprite counter_positive_negative;
    counter_positive_negative.setTexture(*texture_map.at("DIGITS"));
    counter_positive_negative.setTextureRect(sf::IntRect(210, 0, 21, 32));
    counter_positive_negative.setPosition(12, 32 * (col_row_mines[1] + .5) + 16);
    gui_map["COUNTER_SIGN"] = counter_positive_negative;

    return gui_map;
}
vector<Tile> GenerateTiles(int rows, int columns, int window_width, int window_height, map<string, Texture*>& texture_map){
    vector<Tile> tiles;
    int width = (window_width) / columns;
    int height = (window_height - 100) / rows;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++) {
            Tile tile(width, height, j * width, i * height, texture_map["HIDDEN"], j, i); // Corrected the order of width and height
            tiles.push_back(tile);
        }
    }

    return tiles;
}

bool hasGameWon(vector<Tile>& tiles){
    for(const auto& i : tiles){
        if(i.hasBomb() && !i.isFlagged)
            return false; // A bomb tile is not flagged

        if(!i.hasBomb() && !i.isClicked)
            return false; // A non-bomb tile is not clicked
    }

    return true; // All tiles meet the winning conditions
}
void ShowMines(bool show, map<string, sf::Texture*>& texture_map, vector<Tile>& tiles){
    for(auto& j : tiles){
        if(show) {
            if (j.hasBomb()){
                j.setSecondarySprite(texture_map.at("MINE"));
                j.secondarySpriteNull = false;
            }
        }
        if(!show){
            if(j.hasBomb() && !j.isClicked) j.secondarySpriteNull = true;
        }

    }
}
void GenerateBombs(vector<Tile>& tiles, int bomb_amount){
    int counter = 0;
    while(counter < bomb_amount){
        for(auto& i : tiles){
            if(counter == bomb_amount) break;
            if(rand() % 100 + 1 < 3)
                if(i.SetBomb()) counter++;
        }
    }

    int bomb_count = 0;
    for(auto& i : tiles){
        if(i.hasBomb()) bomb_count++;

    }

}
void AssignNeighbors(vector<Tile>& tiles, int rows, int columns) {
    for (auto& j : tiles) {
        // RIGHT
        if (j.col != columns - 1) {
            int right_index = j.row * columns + (j.col + 1);
            j.adjacent_tiles.push_back(&tiles[right_index]);
        }

        // LEFT
        if (j.col != 0) {
            int left_index = j.row * columns + (j.col - 1);
            j.adjacent_tiles.push_back(&tiles[left_index]);
        }

        // DOWN
        if (j.row != rows - 1) {
            int down_index = (j.row + 1) * columns + j.col;
            j.adjacent_tiles.push_back(&tiles[down_index]);
        }

        // UP
        if (j.row != 0) {
            int up_index = (j.row - 1) * columns + j.col;
            j.adjacent_tiles.push_back(&tiles[up_index]);
        }

        // UP RIGHT
        if (j.row != 0 && j.col != columns - 1) {
            int up_right_index = (j.row - 1) * columns + (j.col + 1);
            j.adjacent_tiles.push_back(&tiles[up_right_index]);
        }

        // UP LEFT
        if (j.row != 0 && j.col != 0) {
            int up_left_index = (j.row - 1) * columns + (j.col - 1);
            j.adjacent_tiles.push_back(&tiles[up_left_index]);
        }

        // DOWN RIGHT
        if (j.row != rows - 1 && j.col != columns - 1) {
            int down_right_index = (j.row + 1) * columns + (j.col + 1);
            j.adjacent_tiles.push_back(&tiles[down_right_index]);
        }

        // DOWN LEFT
        if (j.row != rows - 1 && j.col != 0) {
            int down_left_index = (j.row + 1) * columns + (j.col - 1);
            j.adjacent_tiles.push_back(&tiles[down_left_index]);
        }
    }
}
Text CreateText(Font& font, const string& value, int size, Text::Style style, Color color, int x, int y){
    Text text(value, font);
    text.setCharacterSize(size);
    text.setStyle(style);
    text.setFillColor(color);
    text.setPosition(x, y);

    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);


    return text;
}


void RegisterLeaderboardOpportunity(string name, Timer& timer, FileIO& file){
    cout << "Registering Leaderboard Opportunity" << endl;
    int time = timer.getTotalTime();
    FileIO::User user;
    user.name = name;
    user.time_string = to_string(time);
    user.time_int = time;


    string mins_one;
    string mins_ten;
    string sec_one;
    string sec_ten;

    int minutes = time / 60;
    int mins_one_digit = minutes % 10;
    int mins_ten_digit = (minutes / 10) % 10;
    int seconds = time % 60;
    int sec_one_digit = seconds % 10;
    int sec_ten_digit = (seconds / 10) % 10;

    mins_one = to_string(mins_one_digit);
    mins_ten = to_string(mins_ten_digit);
    sec_one = to_string(sec_one_digit);
    sec_ten = to_string(sec_ten_digit);

    user.time_string = mins_ten + mins_one + ":" + sec_ten + sec_one;

    file.AddPlayer(user);
    file.WriteLeaderboard();
}

string FormatPlayer(const FileIO::User& player, int index, const std::string& current_name) {
    std::string player_string = std::to_string(index) + ".\t" + player.time_string + '\t' + player.name;
    if (player.name == current_name) {
        player_string += "*";
    }
    return player_string;
}
string FormatAllPlayers(const std::vector<FileIO::User>& user_list, const std::string& name) {
    std::string all_players;
    for (size_t i = 0; i < user_list.size(); ++i) {
        all_players += FormatPlayer(user_list[i], i + 1, name) + "\n\n";
    }
    return all_players;
}


