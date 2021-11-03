#include "Event.hpp"
#include "Calendar.hpp"

Event::Event(std::string title, int month, int day, int year)
{
    m_type = "unique";
    m_title = title;
    m_month = month;
    m_day = day;
    m_year = year;
    
    m_eventText.setString("- " + m_title);
    m_eventText.setFillColor(sf::Color::Black);
    m_eventText.setCharacterSize(24);
}

Event::Event(std::string title, int month, int day)
{
    m_type = "persistent";
    m_title = title;
    m_month = month;
    m_day = day;
    m_year = -1;
    
    m_eventText.setString("- " + m_title);
    m_eventText.setFillColor(sf::Color::Blue);
    m_eventText.setCharacterSize(16);
}

Event::~Event()
{
}

std::string Event::getType()
{
    return m_type;
}

std::string Event::getTitle()
{
    return m_title;
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
    m_eventText.setPosition(420, 470 + (20 * iterator));
    m_eventText.setFont(*Calendar::getInstance()->getFont());
    window->draw(m_eventText);
}
