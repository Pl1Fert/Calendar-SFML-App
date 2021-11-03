#include "TextField.hpp"
#include "Calendar.hpp"

TextField::TextField(std::string type, std::string label, int xPos, int yPos)
{
    m_font.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Data/arial.ttf");

    m_type = type;
    
    m_xPos = xPos;
    m_yPos = yPos;
    
    m_label.setString(label);
    m_label.setPosition(m_xPos, m_yPos);
    m_label.setFont(m_font);
    m_label.setCharacterSize(18);
    m_label.setFillColor(sf::Color::Black);
    
    m_borderRect.setPosition(m_xPos + m_label.getGlobalBounds().width, m_yPos);
    m_borderRect.setSize(sf::Vector2f(330, 20));
    m_borderRect.setOutlineThickness(2);
    m_borderRect.setOutlineColor(sf::Color(000, 000, 000));
    
    m_value = "";
    m_valueText.setString("");
    m_valueText.setPosition(m_xPos + m_label.getGlobalBounds().width + 4, m_yPos + 1);
    m_valueText.setFont(m_font);
    m_valueText.setCharacterSize(16);
    m_valueText.setFillColor(sf::Color::Black);
}

TextField::~TextField()
{
}

sf::RectangleShape* TextField::getRect()
{
    return &m_borderRect;
}

std::string TextField::getValue()
{
    return m_value;
}

void TextField::handleInput(sf::Event* event){
    if(m_type == "text")
    {
        if(event->type == sf::Event::TextEntered && (event->text.unicode >= 32 && event->text.unicode <= 126))
        {
            if(m_valueText.getGlobalBounds().width <= 334 - 16)
            {
                m_value += static_cast<char>(event->text.unicode);
                m_valueText.setString(m_value);
            }
        }
    }
    else if(m_type == "number")
    {
        if(event->type == sf::Event::TextEntered && (event->text.unicode >= 48 && event->text.unicode <= 57))
        {
            if(m_valueText.getGlobalBounds().width <= 334 - 16)
            {
                m_value += static_cast<char>(event->text.unicode);
                m_valueText.setString(m_value);
            }
        }
    }
    
    if(event->type == sf::Event::KeyPressed)
    {
        if(event->key.code == sf::Keyboard::BackSpace)
        {
            m_value = m_value.substr(0, m_value.length() - 1);
            m_valueText.setString(m_value);
        }
    }
}

void TextField::setValue(std::string value)
{
    m_value = value;
    m_valueText.setString(value);
}

void TextField::setBorder(std::string type)
{
    if(type == "normal")
    {
        m_borderRect.setOutlineColor(sf::Color(000, 000, 000));
    }
    else if(type == "targeted")
    {
        m_borderRect.setOutlineColor(sf::Color(38, 188, 00));
    }
}

void TextField::draw(sf::RenderWindow* window)
{
    window->draw(m_label);
    window->draw(m_borderRect);
    window->draw(m_valueText);
}
