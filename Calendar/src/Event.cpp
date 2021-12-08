#include "Event.hpp"

Event::Event(std::string title, int month, int day, int year)
{
    m_title = title;
    m_month = month;
    m_day = day;
    m_year = year;
    m_font.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Data/arial.ttf");
    
    m_eventText.setString("- " + m_title);
    m_eventText.setFillColor(sf::Color::Black);
    m_eventText.setCharacterSize(24);
}

Event::~Event()
{
}

std::string Event::getTitle()
{
    return m_title;
}

void Event::setTitle(std::string title)
{
    m_title = title;
}

int Event::getMonth()
{
    return m_month;
}

int Event::getDay()
{
    return m_day;
}

int Event::getYear()
{
    return m_year;
}

sf::Text* Event::getEventText()
{
    return &m_eventText;
}

void Event::draw(sf::RenderWindow* window, int iterator)
{
    m_eventText.setPosition(830, 790 + (25 * iterator));
    m_eventText.setFont(m_font);
    window->draw(m_eventText);
}

