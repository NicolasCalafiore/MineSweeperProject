//
// Created by nicol on 4/21/2024.
//

#ifndef PROJECT3_TEXTFIELD_H
#define PROJECT3_TEXTFIELD_H

#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class TextField{
    private:
        Text text_field;

        void CenterText(Text& text){
            FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f,
                           textRect.top + textRect.height/2.0f);
        }

    public:
        TextField(Font& font, const string& value, int size, Text::Style style, Color color, int width, int height){
            Text text(value, font);
            text.setCharacterSize(size);
            text.setStyle(style);
            text.setFillColor(color);
            text.setPosition(width, height);


            this->text_field = text;
        }

        int getSize(){
            return text_field.getString().toAnsiString().size() - 1; // -1 to account for cursor
        }

        void append(char character){
            std::string current_string = text_field.getString().toAnsiString();
            if(current_string.size() == 11) return;

            current_string.erase(current_string.size() - 1); // Erasing Cursor

            current_string = current_string + character;

            if (!current_string.empty()) {
                current_string[0] = toupper((current_string[0]));
                for (int i = 1; i < current_string.size(); ++i) {
                    current_string[i] = tolower(current_string[i]);
                }
            }

            text_field.setString(current_string + "|");
            CenterText(text_field);
        }

        void backspace(){
            string current_string = (std::string) text_field.getString();

            if (current_string.size() > 1) {
                current_string.erase(current_string.size() - 2);
                text_field.setString(current_string  + "|");
            }
            else{
                text_field.setString("|");
            }
            CenterText(text_field);
        }

        Text getText(){
            return text_field;
        }

};

#endif //PROJECT3_TEXTFIELD_H
