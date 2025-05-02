//
// Created by nicol on 4/21/2024.
//

#ifndef PROJECT3_TILE_H
#define PROJECT3_TILE_H
#include <SFML/Graphics.hpp>
#include "Button.h"

using namespace sf;
using namespace std;
class Tile : public Button{
    private:
        bool isBomb = false;

    public:
        int col;
        int row;
        bool isClicked = false;
        vector<Tile*> adjacent_tiles;
        int mine_num;
        bool isFlagged = false;

        Tile(int width, int height, int x, int y, Texture* texture, int col, int row) : Button(width, height, x, y, texture){
            this->col = col;
            this->row = row;
        }

        bool SetBomb(){
            if(isBomb) return false;
            else{
                this->isBomb = true;
                return true;
            }
        }

        bool hasBomb() const{
            return isBomb;
        }

        void setFlag(map<string, sf::Texture*>& texturemap){
            if(!isClickable) return;
            if(isClicked) return;
            isFlagged = !isFlagged;

            if(isFlagged){
                tertiarySpriteNull = false;
                setTertiarySprite(texturemap["FLAG"]);
            }
            else{
                tertiarySpriteNull = true;
                setTertiarySprite(texturemap["HIDDEN"]);
            }

            cout << "tertiarySpriteNull: " << tertiarySpriteNull << endl;
        }

        void CalculateMineNumber(){
            int counter = 0;
            for(auto& i : adjacent_tiles){
                if(i->hasBomb()) counter++;
            }

            this->mine_num = counter;
        }

        void AutomaticallySetSecondarySprite(map<string, sf::Texture*>& texturemap){
            if(mine_num == 1) setSecondarySprite(texturemap.at("NUM1"));
            if(mine_num == 2) setSecondarySprite(texturemap.at("NUM2"));
            if(mine_num == 3) setSecondarySprite(texturemap.at("NUM3"));
            if(mine_num == 4) setSecondarySprite(texturemap.at("NUM4"));
            if(mine_num == 5) setSecondarySprite(texturemap.at("NUM5"));
            if(mine_num == 6) setSecondarySprite(texturemap.at("NUM6"));
            if(mine_num == 7) setSecondarySprite(texturemap.at("NUM7"));
            if(mine_num == 8) setSecondarySprite(texturemap.at("NUM8"));
            if(mine_num == 0) setSecondarySprite(texturemap.at("REVEALED"));
            if(hasBomb()) setSecondarySprite(texturemap.at("MINE"));
            secondarySpriteNull = false;
        }
        void Clicked(map<string, sf::Texture*>& texturemap){
            if(!isClickable) return;
            if (mine_num != 0 || isClicked || hasBomb()) {
                isClicked = true;
                setPrimarySprite(texturemap.at("REVEALED"));
                AutomaticallySetSecondarySprite(texturemap);
                tertiarySpriteNull = true;
                if(isFlagged) setFlag(texturemap);

                return;
            }

            tertiarySpriteNull = true;
            if(isFlagged) setFlag(texturemap);
            isClicked = true;
            setPrimarySprite(texturemap.at("REVEALED"));

            for (auto& adjacent_tile : adjacent_tiles) {
                adjacent_tile->Clicked(texturemap);
            }
        }



};
#endif //PROJECT3_TILE_H
