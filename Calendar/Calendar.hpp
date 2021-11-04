#pragma once

#include <ctime>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Day.hpp"
#include "Event.hpp"
#include "TextField.hpp"

class Calendar
{
private:
    static Calendar* m_instance;
    sf::RenderWindow m_window;
    sf::Event m_event;
    sf::Font m_font;
    
    time_t m_currentTime;
    struct tm* m_currentDate;
    
    std::vector<Day*> m_month;
    Day* m_target;
    Event* m_targetEvent;
    int m_activeMonth;
    int m_activeYear;
    
    std::vector<Event*> m_allEvents;
    
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
    
    Calendar();
    ~Calendar();
    
    std::string convertIntToString(int);
    void centerText(sf::Text*, int, int, int);
    void updateDayColors();
    bool spriteIncludesPoint(sf::Sprite*, int, int);
    bool rectIncludesPoint(sf::RectangleShape*, int, int);
    
    std::string addSpaces(std::string);
    std::string removeSpaces(std::string);
    
    int getActiveYear();
    int getActiveMonth();
    void loadEvents();
    void deleteEvent(Event*);
    void constructMonth();
    void drawMonth();
    void handleInput(sf::Event*);
    void nextMonth();
    void prevMonth();
    void saveEvents();
    void prepBeforeMonthChange();
    std::string setMonthString(std::string, std::string, std::string);
    
public:
    static Calendar* getInstance();
    
    sf::Font* getFont();
    std::string getMonthString(int);
    int getDaysInMonth(int, int);
    Day* getClickedDay(int, int);
    
    void run();
};
