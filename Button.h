//
// Created by nicol on 4/21/2024.
//

#ifndef PROJECT3_BUTTON_H
#define PROJECT3_BUTTON_H

#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Button{
    private:
        bool isClicked = false;
        int width;
        int height;
        int x;
        int y;
        Sprite primary_sprite;
        Sprite secondary_sprite;
        Sprite tertiary_sprite;

    public:
        bool isClickable = true;
        bool secondarySpriteNull = true;
        bool tertiarySpriteNull = true;

        Button(int width, int height, int x, int y, Texture* texture){
            this->width = width;
            this->height = height;
            this->x = x;
            this->y = y;
            this->primary_sprite.setTexture(*texture);
            this->primary_sprite.setPosition(x, y);
            this->secondary_sprite.setPosition(x, y);
            this->tertiary_sprite.setPosition(x, y);
        }

        Sprite& getPrimarySprite(){
            return primary_sprite;
        }

        Sprite& getSecondarySprite(){
            return secondary_sprite;
        }

        Sprite& getTertiarySprite(){
            return tertiary_sprite;
        }

        void setPrimarySprite(Texture* texture){
            if(!isClickable) return;
            this->primary_sprite.setTexture(*texture);
        }

        void setSecondarySprite(Texture* texture){
            if(!isClickable) return;
            this->secondary_sprite.setTexture(*texture);
        }

        void setTertiarySprite(Texture* texture){
            if(!isClickable) return;
            this->tertiary_sprite.setTexture(*texture);
        }



};
#endif //PROJECT3_BUTTON_H
