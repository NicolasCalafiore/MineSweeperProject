  //
// Created by nicol on 4/29/2024.
//
#include "Counter.h"

#ifndef PROJECT3_UIRENDERER_H
#define PROJECT3_UIRENDERER_H
class UIRenderer{



public:
    UIRenderer(){

    }

    static void TimerRendering(map<string, Sprite>& sprite_map, Timer& game_timer){
        sprite_map["TIME_SEC_ONES"].setTextureRect(sf::IntRect(game_timer.TimerSpritePositionOnes(false), 0, 21, 32));
        sprite_map["TIME_MIN_ONES"].setTextureRect(sf::IntRect(game_timer.TimerSpritePositionOnes(true), 0, 21, 32));

        sprite_map["TIME_SEC_TENS"].setTextureRect(sf::IntRect(game_timer.TimerSpritePositionTen(false), 0, 21, 32));
        sprite_map["TIME_MIN_TENS"].setTextureRect(sf::IntRect(game_timer.TimerSpritePositionTen(true), 0, 21, 32));
    }

    static void DigitAndButtonSprites(RenderWindow& game_window, map<string, Sprite>& sprite_map, map<string, Texture*>& texture_map, Counter& counter){
        for (auto const &sprite: sprite_map) {
            if(&sprite.second != &sprite_map["COUNTER_SIGN"] || counter.showCounter){
                game_window.draw(sprite.second);
            }

        }
    }

    static void TileRendering(RenderWindow& game_window, vector<Tile>& tiles, map<string, Texture*>& texture_map, bool& isDebug){
        for(Tile& i : tiles){

            game_window.draw(i.getPrimarySprite());

            if(!i.tertiarySpriteNull){
                game_window.draw(i.getTertiarySprite());
            }

            if(i.getSecondarySprite().getTexture() == texture_map["MINE"] && isDebug){
                game_window.draw(i.getSecondarySprite());
            }

            if(!i.secondarySpriteNull && i.isClicked){
                game_window.draw(i.getSecondarySprite());
            }

        }
    }

    static void GameCounterRenering(RenderWindow& game_window, map<string, Sprite>& sprite_map, Counter& counter){
        sprite_map["COUNTER_ONES"].setTextureRect(sf::IntRect(counter.getCounterPosition(false, false, true), 0, 21, 32));
        sprite_map["COUNTER_TENS"].setTextureRect(sf::IntRect(counter.getCounterPosition(false, true, false), 0, 21, 32));
        sprite_map["COUNTER_HUNDREDS"].setTextureRect(sf::IntRect(counter.getCounterPosition(true, false, false), 0, 21, 32));
    }
};
#endif //PROJECT3_UIRENDERER_H
