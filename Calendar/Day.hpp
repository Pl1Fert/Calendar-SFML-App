#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Event.hpp"

class Day
{
private:
    sf::RectangleShape m_dayRect;
    sf::Text m_numText;
    int m_xPos, m_yPos;
    int m_number;
    std::vector<sf::Text*> m_dayContent;
    std::string m_type;
    bool m_isSelected;
    
    std::vector<Event*> m_events;
public:
    Day(int, int, sf::Font*);
    ~Day();
    
    int getX();
    int getY();
    int getNumber();
    std::string getType();
    
    std::vector<Event*>* getEvents();
    Event* getClickedEvent(int, int);
    
    void setNumber(int);
    void setType(std::string);
    void setSelected(bool);
    void addEvent(Event*);
    bool deleteEvent(Event*);
    void refreshContent();
    void clearContent();
    void updateEventTextColor(Event*);
    void draw(sf::RenderWindow*);
    void drawEvents(sf::RenderWindow*);
    void sortEvents();
};

