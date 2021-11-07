#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Event
{
private:
    std::string m_title;
    int m_month;
    int m_day;
    int m_year;
    sf::Text m_eventText;
    sf::Font m_font;
public:
    Event(std::string, int, int, int);
    ~Event();
    
    std::string getTitle();
    int getMonth();
    int getDay();
    int getYear();
    sf::Text* getEventText();
    void setTitle(std::string);
    
    void draw(sf::RenderWindow*, int);
};
