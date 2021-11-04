#include "Day.hpp"

Day::Day(int xPos, int yPos, sf::Font* font)
{
    m_xPos = xPos;
    m_yPos = yPos;
    
    m_dayRect.setPosition(xPos, yPos);
    m_dayRect.setSize(sf::Vector2f(220, 97));
    m_dayRect.setOutlineThickness(2);
    m_dayRect.setOutlineColor(sf::Color(000, 000, 000));
    
    m_numText.setString("");
    m_numText.setFont(*font);
    m_numText.setPosition(m_xPos + 3, m_yPos + 1);
    m_numText.setFillColor(sf::Color::Black);
    m_numText.setCharacterSize(24);
    m_numText.setStyle(sf::Text::Bold);
    
    for(int i = 0; i < 4; i++)
    {
        m_dayContent.push_back(new sf::Text());
        m_dayContent[i]->setString("");
        m_dayContent[i]->setFont(*font);
        m_dayContent[i]->setFillColor(sf::Color::Black);
        m_dayContent[i]->setCharacterSize(20);
        m_dayContent[i]->setPosition(m_xPos + 20, m_yPos + 1 + (21 * i));
    }
    
    m_type = "default";
}

Day::~Day()
{
}

int Day::getNumber()
{
    return m_number;
}

std::string Day::getType()
{
    return m_type;
}

int Day::getX()
{
    return m_xPos;
}

int Day::getY()
{
    return m_yPos;
}

std::vector<Event*>* Day::getEvents()
{
    return &m_events;
}

Event* Day::getClickedEvent(int clickX, int clickY)
{
    Event* result = NULL;
    
    for(int e = 0; e < m_events.size(); e++)
    {
        if(clickX >= m_events[e]->getEventText()->getPosition().x && clickX < m_events[e]->getEventText()->getPosition().x
           + m_events[e]->getEventText()->getGlobalBounds().width && clickY >= m_events[e]->getEventText()->getPosition().y
           && clickY < m_events[e]->getEventText()->getPosition().y + m_events[e]->getEventText()->getGlobalBounds().height + 5)
        {
            result = m_events[e];
        }
    }
    
    return result;
}

void Day::setNumber(int number)
{
    m_number = number;
    
    std::string numString;
    numString = std::to_string(m_number);
    
    m_numText.setString(numString);
}

void Day::setType(std::string type)
{
    m_type = type;
    
    if(m_type == "default")
    {
        m_dayRect.setFillColor(sf::Color(255, 255, 255));
    }
    else if(m_type == "current")
    {
        m_dayRect.setFillColor(sf::Color(98, 220, 123));
    }
    else if(m_type == "inactive")
    {
        m_dayRect.setFillColor(sf::Color(174, 174, 174));
    }
}

void Day::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
    
    if(m_isSelected == true)
    {
        m_dayRect.setOutlineColor(sf::Color(38, 188, 00));
    }
    else
    {
        m_dayRect.setOutlineColor(sf::Color(000, 000, 000));
    }
}

void Day::addEvent(Event* event)
{
    m_events.push_back(event);
    refreshContent();
}

bool Day::deleteEvent(Event* event)
{
    bool result = false;

    for(int e = 0; e < m_events.size(); e++)
    {
        if(m_events[e] == event)
        {
            m_events.erase(m_events.begin() + e);
            refreshContent();
            result = true;
        }
    }
    
    return result;
}

void Day::refreshContent()
{
    for(int i = 0; i < m_dayContent.size(); i++)
    {
        m_dayContent[i]->setString("");
    }
    
    int size = 0;
    if(m_events.size() <= 4)
    {
        size = m_events.size();
    }
    else if(m_events.size() > 4)
    {
        size = 4;
    }
    
    for(int e = 0; e < size; e++)
    {
        std::string content = "- ";
        std::string title = m_events[e]->getTitle();
        
        for(int c = 0; c < title.length(); c++)
        {
            if(m_dayContent[e]->getGlobalBounds().width <= 130)
            {
                content += title[c];
                m_dayContent[e]->setString(content);
            }
        }
        
        m_dayContent[e]->setFillColor(sf::Color::Black);
    }
}

void Day::clearContent()
{
    m_events.clear();
    
    for(int i = 0; i < m_dayContent.size(); i++)
    {
        m_dayContent[i]->setString("");
    }
}

void Day::updateEventTextColor(Event* targetEvent)
{
    //Set the text of all of this day's events to the default color.
    for(int e = 0; e < m_events.size(); e++)
    {
        m_events[e]->getEventText()->setFillColor(sf::Color::Black);
    }
    
    //Change the target event's text color.
    if(targetEvent != NULL)
    {
        targetEvent->getEventText()->setFillColor(sf::Color(56, 188, 0, 255));
    }
}

void Day::draw(sf::RenderWindow* window)
{
    window->draw(m_dayRect);
    window->draw(m_numText);
    
    for(int i = 0; i < m_dayContent.size(); i++)
    {
        window->draw(*m_dayContent[i]);
    }
}

void Day::drawEvents(sf::RenderWindow* window)
{
    for(int i = 0; i < m_events.size(); i++)
    {
        m_events[i]->draw(window, i);
    }
}

