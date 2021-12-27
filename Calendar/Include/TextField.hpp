#pragma once

#include <iostream>
#include <string>
#include <codecvt>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class TextField
{
private:
    std::string m_value;
    std::string m_type;
    int m_xPos, m_yPos;
    sf::Text m_label;
    sf::Text m_valueText;
    sf::RectangleShape m_borderRect;
    sf::Font m_font;
    std::wstring m_string;
public:
    TextField(std::string, std::string, int, int);
    ~TextField();
    
    sf::RectangleShape* getRect();
    std::string getValue();
    
    void handleInput(sf::Event*);
    void setValue(std::string);
    void setBorder(std::string);
    void draw(sf::RenderWindow*);
    std::string ws2s(const std::wstring& wstr);
    std::wstring s2ws(const std::string& str);
};
