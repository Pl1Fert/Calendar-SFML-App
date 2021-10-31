#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Day.hpp"
#include "Event.hpp"
#include "TextField.hpp"
#include "Checkbox.hpp"
#include "ResourcePath.hpp"

class Calendar
{
private:
    static Calendar* m_instance;
    sf::RenderWindow m_window;
    sf::Event m_event;
    sf::Font m_font;
    
    time_t m_currentTime;
    struct tm* m_currentDate;
    
    vector<Day*> m_month;
    Day* m_target;
    Event* m_targetEvent;
    int m_activeMonth;
    int m_activeYear;
    
    vector<Event*> m_persistentEvents;//убрать нахуй виды эвентов и оставить 1
    vector<Event*> m_uniqueEvents;
    
    sf::Text m_monthLabel;
    sf::Text m_daysLabel;
    sf::Text m_addNote;
    sf::Text m_selectedDay;
    
    sf::Texture m_leftSelectorImage;
    sf::Texture m_rightSelectorImage;
    sf::Texture m_dividerImage;
    sf::Texture m_deleteButtonImage;
    sf::Texture m_addNoteButtonImage;
    
    sf::Sprite m_leftSelector;
    sf::Sprite m_rightSelector;
    sf::Sprite m_divider;
    sf::Sprite m_deleteButton;
    sf::Sprite m_addNoteButton;
    
    TextField* m_targetTextField;
    TextField* m_titleField;
    
    Checkbox* m_typeBox;
    
    Calendar();
    ~Calendar();
    
    string convertIntToString(int);
    void centerText(sf::Text*, int, int, int);
    void updateDayColors();
    bool spriteIncludesPoint(sf::Sprite*, int, int);
    bool rectIncludesPoint(sf::RectangleShape*, int, int);
    
    string addSpaces(string);
    string removeSpaces(string);
    
    int getActiveYear();
    int getActiveMonth();
    void loadPersistentEvents();
    void loadUniqueEvents();
    void deleteEvent(Event*);
    void constructMonth();
    void drawMonth();
    void handleInput(sf::Event*);
    
    void nextMonth();
    void prevMonth();
    void save();
    
public:
    static Calendar* getInstance();
    
    sf::Font* getFont();
    string getMonthString(int);
    int getDaysInMonth(int, int);
    Day* getClickedDay(int, int);
    
    
    void run();
    
    
};
