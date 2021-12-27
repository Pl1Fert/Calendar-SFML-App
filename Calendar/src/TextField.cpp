#include "TextField.hpp"

TextField::TextField(std::string type, std::string label, int xPos, int yPos)
{
    m_font.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Data/arial.ttf");

    m_type = type;
    
    m_xPos = xPos;
    m_yPos = yPos;
    
    m_label.setString(label);
    m_label.setPosition(m_xPos, m_yPos);
    m_label.setFont(m_font);
    m_label.setCharacterSize(24);
    m_label.setFillColor(sf::Color::Black);
    
    m_borderRect.setPosition(m_xPos + m_label.getGlobalBounds().width, m_yPos);
    m_borderRect.setSize(sf::Vector2f(550, 40));
    m_borderRect.setOutlineThickness(2);
    m_borderRect.setOutlineColor(sf::Color(000, 000, 000));
    
    m_value = "";
    m_valueText.setString("");
    m_valueText.setPosition(m_xPos + m_label.getGlobalBounds().width + 4, m_yPos + 1);
    m_valueText.setFont(m_font);
    m_valueText.setCharacterSize(24);
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

std::string TextField::ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

std::wstring TextField::s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

void TextField::handleInput(sf::Event* event)
{
    if(m_type == "text")
    {
        if(event->type == sf::Event::TextEntered && (event->text.unicode >= 32 && event->text.unicode <= 1103))
        {
            if(m_valueText.getGlobalBounds().width <= 334 - 16)
            {
                m_string += static_cast<wchar_t>(event->text.unicode);
                m_valueText.setString(m_string);
                //m_value = m_string;
                m_value = ws2s(m_string);
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
            m_string = m_string.substr(0, m_string.length() - 1);
            m_valueText.setString(m_string);
            m_value = ws2s(m_string);
        }
    }
}

void TextField::setValue(std::string value)
{
    m_value = value;
    m_string = s2ws(m_value);
    m_valueText.setString(m_string);
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
