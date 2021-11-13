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
    sf::Text m_editNote;
    sf::Text m_selectedDay;
    
    sf::Texture m_leftSelectorImage;
    sf::Texture m_rightSelectorImage;
    sf::Texture m_dividerImage;
    sf::Texture m_deleteNoteButtonImage;
    sf::Texture m_addNoteButtonImage;
    sf::Texture m_editNoteButtonImage;
    sf::Texture m_secondEditNoteButtonImage;
    
    sf::Sprite m_leftSelector;
    sf::Sprite m_rightSelector;
    sf::Sprite m_divider;
    sf::Sprite m_deleteNoteButton;
    sf::Sprite m_addNoteButton;
    sf::Sprite m_editNoteButton;
    sf::Sprite m_secondEditNoteButton;
    
    TextField* m_targetTextField;
    TextField* m_addNoteField;
    TextField* m_editNoteField;
    
    Calendar();
    ~Calendar();
    
    void centerText(sf::Text*, int, int, int);
    void updateDayColors();
    bool spriteIncludesPoint(sf::Sprite*, int, int);
    bool rectIncludesPoint(sf::RectangleShape*, int, int);
    
    std::string addSpaces(std::string);
    std::string removeSpaces(std::string);
    
    int getActiveYear();
    int getActiveMonth();
    
    void handleInput(sf::Event*);
    void constructMonth();
    void drawMonth();
    void nextMonth();
    void prevMonth();
    void setNextMonthDays(int);
    int setLastMonthDays(int,int,int);
    void prepBeforeMonthChange();
    
    void deleteEvent(Event*);
    void loadEvents();
    void sortEvents();
    void addEvents();
    void saveEvents();

    sf::Font* getFont();
    std::string getMonthString(int);
    std::string setMonthString(std::string, std::string, std::string);
    int getDaysInMonth(int, int);
    Day* getClickedDay(int, int);
public:
    static Calendar* getInstance();
    void run();
};
