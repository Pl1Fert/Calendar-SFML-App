#include "Calendar.hpp"

Calendar* Calendar::m_instance;

Calendar::Calendar()
{
    m_window.create(sf::VideoMode(1600, 1200, 32), "Calendar", sf::Style::Close);
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true);
    
    m_font.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Data/arial.ttf");
    
    m_currentTime = time(0);
    m_currentDate = localtime(&m_currentTime);
    
    m_target = NULL;
    //targetEvent_ = NULL;
    
    m_activeMonth = m_currentDate->tm_mon;
    m_activeYear = m_currentDate->tm_year + 1900;
    
    m_monthLabel.setString(getMonthString(m_currentDate->tm_mon) + " " + std::to_string(m_currentDate->tm_year + 1900));
    m_monthLabel.setFont(m_font);
    m_monthLabel.setFillColor(sf::Color::Black);
    m_monthLabel.setCharacterSize(32);
    m_monthLabel.setStyle(sf::Text::Bold);
    centerText(&m_monthLabel, 0, 1600, 10);
    
    m_daysLabel.setString("       Monday            Tuesday           Wednesday         Thursday             Friday             Saturday             Sunday     ");
    m_daysLabel.setPosition(0, 50);
    m_daysLabel.setFont(m_font);
    m_daysLabel.setFillColor(sf::Color::Black);
    m_daysLabel.setCharacterSize(28);
    m_daysLabel.setStyle(sf::Text::Bold);
    
    m_leftSelectorImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/SelectorLeft.png");
    m_rightSelectorImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/SelectorRight.png");
    m_leftSelector.setTexture(m_leftSelectorImage);
    m_leftSelector.setPosition(600, 12);
    m_rightSelector.setTexture(m_rightSelectorImage);
    m_rightSelector.setPosition(980, 11);
    
    m_dividerImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/Divider.png");
    m_divider.setTexture(m_dividerImage);
    m_divider.setPosition(680, 778);
    
    m_selectedDay.setString("");
    m_selectedDay.setFont(m_font);
    m_selectedDay.setFillColor(sf::Color::Black);
    m_selectedDay.setCharacterSize(24);
    m_selectedDay.setStyle(sf::Text::Bold);
    
    m_deleteNoteButtonImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/DeleteButton.png");
    m_deleteNoteButton.setTexture(m_deleteNoteButtonImage);
    
    m_editNoteButtonImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/EditButton.png");
    m_editNoteButton.setTexture(m_editNoteButtonImage);
    
    m_secondEditNoteButtonImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/EditButton.png");
    m_secondEditNoteButton.setTexture(m_secondEditNoteButtonImage);
    m_secondEditNoteButton.setPosition(250, 1040);
    
    m_addNoteButtonImage.loadFromFile("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Images/AddButton.png");
    m_addNoteButton.setTexture(m_addNoteButtonImage);
    m_addNoteButton.setPosition(250, 840);
    
    m_targetTextField = NULL;
    m_addNote.setString("Add a note: ");
    m_addNote.setFont(m_font);
    m_addNote.setPosition(11, 742);
    m_addNote.setFillColor(sf::Color::Black);
    m_addNote.setCharacterSize(24);
    m_addNote.setStyle(sf::Text::Bold);
    
    m_editNote.setString("Edit note: ");
    m_editNote.setFont(m_font);
    m_editNote.setPosition(11, 932);
    m_editNote.setFillColor(sf::Color::Black);
    m_editNote.setCharacterSize(24);
    m_editNote.setStyle(sf::Text::Bold);
    m_addNoteField = new TextField("text", "Note: ", 10, 775);
    m_editNoteField = new TextField("text", "Edit: ", 10, 975);
        
    try
    {
        loadEvents();
    }
    catch(std::runtime_error &error)
    {
        std::cout << "Error: " << error.what() << std::endl;
    }
    
    createDayObjects();
    constructMonth();
}

Calendar::~Calendar()
{
    if(m_addNoteField)
        delete m_addNoteField;
    if(m_editNoteField)
        delete m_editNoteField;
    if(m_targetTextField)
        delete m_targetTextField;
    if(m_target)
        delete m_target;
    if(m_targetEvent)
        delete m_targetEvent;
}

Calendar* Calendar::getInstance()
{
    if(!m_instance)
    {
        m_instance = new Calendar();
    }

    return m_instance;
}

void Calendar::run()
{
    while(m_window.isOpen())
    {
        while(m_window.pollEvent(m_event))
        {
            if(m_event.type == sf::Event::Closed)
            {
                try
                {
                    saveEvents();
                }
                catch(std::runtime_error &error)
                {
                    std::cout << "Error: " << error.what() << std::endl;
                }
                
                m_window.close();
            }
            
            handleInput(&m_event);
            
            if(m_target != NULL && m_targetTextField != NULL)
            {
                m_targetTextField->handleInput(&m_event);
            }
        }
        
        m_window.clear(sf::Color::White);
        
        m_window.draw(m_leftSelector);
        m_window.draw(m_monthLabel);
        m_window.draw(m_rightSelector);
        m_window.draw(m_daysLabel);
        
        for(int d = 0; d < m_month.size(); d++)
        {
            m_month[d]->draw(&m_window);
        }
        
        m_window.draw(m_divider);
        
        if(m_target != NULL)
        {
            m_window.draw(m_selectedDay);
            m_target->drawEvents(&m_window);
            
            if(m_target->getType() != "inactive")
            {
                m_window.draw(m_addNote);
                m_addNoteField->draw(&m_window);
                m_window.draw(m_addNoteButton);
            }
        }
        
        if(m_target != NULL && m_targetEvent != NULL)
        {
            m_window.draw(m_editNote);
            m_editNoteField->draw(&m_window);
            m_window.draw(m_deleteNoteButton);
            m_window.draw(m_secondEditNoteButton);
            m_window.draw(m_editNoteButton);
        }
        
        m_window.display();
    }
}

void Calendar::loadEvents()
{
    std::ifstream file;
        
    file.open("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Data/AllEvents.cll");
    
    if(!file.is_open())
    {
        throw std::runtime_error("can not open file!");
    }
    
    std::string title = "";
    int month = 0, day = 0, year = 0;
    
    while(file >> title)
    {
        file >> month;
        file >> day;
        file >> year;
        
        m_allEvents.push_back(new Event(addSpaces(title), month, day, year));
    }
    
    file.close();
    sortEvents(); //?????????????????
}

void Calendar::sortEvents()
{
    for(int i = 0; i < m_allEvents.size(); i++)
    {
        for (int j = 1 + i; j < m_allEvents.size(); j++)
        {
            if(m_allEvents[i]->getTitle() > m_allEvents[j]->getTitle())
            {
                std::swap(m_allEvents[i], m_allEvents[j]);
            }
        }
    }
}

void Calendar::saveEvents()
{
    std::ofstream file;

    file.open("/Users/pl1fert/Documents/прога универ/сем 3/Calendar/Calendar/Data/AllEvents.cll");
    
    if(!file.is_open())
    {
        throw std::runtime_error("can not open file!");
    }
    
    for(int i = 0; i < m_allEvents.size(); i++)
    {
        file << removeSpaces(m_allEvents[i]->getTitle()) << " ";
        file << m_allEvents[i]->getMonth() << " ";
        file << m_allEvents[i]->getDay() << " ";
        file << m_allEvents[i]->getYear() << " ";
        file << "\n";
    }
    
    file.close();
}

void Calendar::deleteEvent(Event* event)
{
    for(int e = 0; e < m_allEvents.size(); e++)
    {
        if(m_allEvents[e] == event)
        {
            m_allEvents.erase(m_allEvents.begin() + e);
        }
    }
}

int Calendar::getActiveYear()
{
    return m_activeYear;
}

int Calendar::getActiveMonth()
{
    return m_activeMonth;
}

void Calendar::constructMonth()
{
    m_monthLabel.setString(getMonthString(getActiveMonth()) + " " + std::to_string(getActiveYear()));
    
    //Tomohiko Sakamoto’s Algorithm
    int currentYear = getActiveYear();
    int currentMonth = getActiveMonth() + 1;
    
    int dayOfTheWeek;
    int day = 1;
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        
    if(currentMonth < 3)
        currentYear--;
    
    dayOfTheWeek = ((currentYear + currentYear / 4 - currentYear / 100 + currentYear / 400 + t[currentMonth-1] + day) % 7) - 1;
    
    if(dayOfTheWeek < 0)
        dayOfTheWeek = 6;
    
    int daysInLastMonth = 0;
    if(getActiveMonth() != 0)
    {
        daysInLastMonth = getDaysInMonth(getActiveMonth() - 1, getActiveYear());
    }
    else
    {
        daysInLastMonth = getDaysInMonth(11, getActiveYear() - 1);
    }
    
    int daysInThisMonth = getDaysInMonth(getActiveMonth(), getActiveYear());
    
    int iterator = 0;

    if(dayOfTheWeek == 0)
    {
        dayOfTheWeek += 7;
    }
    
    iterator = setLastMonthDays(daysInLastMonth, (dayOfTheWeek-1), iterator);
    
    //set this month's days.
    for(int i = 1; i <= daysInThisMonth; i++)
    {
        m_month[iterator]->setNumber(i);
        
        if(i == m_currentDate->tm_mday && getActiveMonth() == m_currentDate->tm_mon && getActiveYear() == m_currentDate->tm_year + 1900)
        {
            m_month[iterator]->setType("current");
        }
        else
        {
            m_month[iterator]->setType("default");
        }

        iterator++;
    }
    
    setNextMonthDays(iterator);
    addEvents();
}

int Calendar::setLastMonthDays(int daysInLastMonth, int dayOfTheWeek, int iterator)
{
    for(int i = daysInLastMonth - (dayOfTheWeek); i <= daysInLastMonth; i++)
    {
        m_month[iterator]->setNumber(i);
        m_month[iterator]->setType("inactive");
        iterator++;
    }
    return iterator;
}

void Calendar::addEvents()
{
    //add events
    for(int e = 0; e < m_allEvents.size(); e++)
    {
        if(m_allEvents[e]->getMonth() == getActiveMonth() && m_allEvents[e]->getYear() == getActiveYear())
        {
            for(int day = 0; day < m_month.size(); day++)
            {
                if(m_month[day]->getNumber() == m_allEvents[e]->getDay())
                {
                    if(m_month[day]->getType() == "default" || m_month[day]->getType() == "current")
                    {
                        m_month[day]->addEvent(m_allEvents[e]);
                    }
                }
            }
        }
    }
}

void Calendar::setNextMonthDays(int iterator)
{
    //set next month's days.
    for(int i = 1; iterator < m_month.size(); i++)
    {
        m_month[iterator]->setNumber(i);
        m_month[iterator]->setType("inactive");
        iterator++;
    }
}

void Calendar::createDayObjects()
{
    for(int h = 0; h < 6; h++)
    {
        for(int w = 0; w < 7; w++)
        {
            m_month.push_back(new Day(15 + (w * 225), 100 + (h * 103), &m_font));
        }
    }
}

//void Calendar::firstConstructMonth()
//{
//    //create day objects.
//    for(int h = 0; h < 6; h++)
//    {
//        for(int w = 0; w < 7; w++)
//        {
//            m_month.push_back(new Day(15 + (w * 225), 100 + (h * 103), &m_font));
//        }
//    }
//
//    //Tomohiko Sakamoto’s Algorithm
//    int currentYear = m_currentDate->tm_year;
//    int currentMonth = m_currentDate->tm_mon + 1;
//
//    int dayOfTheWeek;
//    int day = 1;
//    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
//
//    if(currentMonth < 3)
//        currentYear--;
//
//    dayOfTheWeek = ((currentYear + currentYear / 4 - currentYear / 100 + currentYear / 400 + t[currentMonth - 1] + day) % 7);
//
//    int daysInLastMonth = 0;
//
//    if(m_currentDate->tm_mon != 0)
//    {
//        daysInLastMonth = getDaysInMonth(m_currentDate->tm_mon - 1, m_currentDate->tm_year + 1900);
//    }
//    else
//    {
//        daysInLastMonth = getDaysInMonth(11, (m_currentDate->tm_year - 1) + 1900);
//    }
//
//    int daysInThisMonth = getDaysInMonth(m_currentDate->tm_mon, m_currentDate->tm_year + 1900);
//
//    if(dayOfTheWeek == 0)
//    {
//        dayOfTheWeek += 7;
//    }
//
//    int iterator = 0;
//
//    iterator = setLastMonthDays(daysInLastMonth, dayOfTheWeek, iterator);
//
//    //set this month's days.
//    for(int i = 1; i <= daysInThisMonth; i++)
//    {
//        m_month[iterator]->setNumber(i);
//
//        if(i == m_currentDate->tm_mday)
//        {
//            m_month[iterator]->setType("current");
//        }
//        else
//        {
//            m_month[iterator]->setType("default");
//        }
//
//        iterator++;
//    }
//    setNextMonthDays(iterator);
//    addEvents();
//}

void Calendar::prepBeforeMonthChange()
{
    m_target = NULL;
    m_targetEvent = NULL;
    m_targetTextField = NULL;
    
    //clear all content
    for(int d = 0; d < m_month.size(); d++)
    {
        m_month[d]->clearContent();
    }
}

void Calendar::nextMonth()
{
    prepBeforeMonthChange();
        
    if(m_activeMonth == 11)
    {
        m_activeMonth = 0;
        m_activeYear++;
    }
    else
    {
        m_activeMonth++;
    }
    
    constructMonth();
}

void Calendar::prevMonth()
{
    prepBeforeMonthChange();
    
    if(m_activeMonth == 0)
    {
        m_activeMonth = 11;
        m_activeYear--;
    }
    else
    {
        m_activeMonth--;
    }
    
    constructMonth();
}

void Calendar::handleInput(sf::Event* event)
{
    if(event->type == sf::Event::MouseButtonPressed)
    {
        if(event->mouseButton.button == sf::Mouse::Left)
        {
            //Handle directional arrows
            if(spriteIncludesPoint(&m_leftSelector, event->mouseButton.x, event->mouseButton.y) == true)
            {
                prevMonth();
            }
            else if(spriteIncludesPoint(&m_rightSelector, event->mouseButton.x, event->mouseButton.y) == true)
            {
                nextMonth();
            }
            
            //Handle the text input field
            if(rectIncludesPoint(m_addNoteField->getRect(), event->mouseButton.x, event->mouseButton.y) == true)
            {
                m_targetTextField = m_addNoteField;
                m_targetTextField->setBorder("targeted");
                m_editNoteField->setBorder("normal");
            }
            
            //Handle the text edit field
            if(rectIncludesPoint(m_editNoteField->getRect(), event->mouseButton.x, event->mouseButton.y) == true)
            {
                m_targetTextField = m_editNoteField;
                m_targetTextField->setBorder("targeted");
                m_addNoteField->setBorder("normal");
            }
            
            //Handle day
            Day* day = getClickedDay(event->mouseButton.x, event->mouseButton.y);
            if(day != NULL)
            {
                m_target = day;
                
                m_targetEvent = NULL;
                m_targetTextField = NULL;
                m_addNoteField->setBorder("normal");
                m_target->updateEventTextColor(m_targetEvent);
                
                std::string targetsMonth = "";
                std::string targetsYear = "";
                
                if(m_target->getType() == "inactive")
                {
                    if(m_target->getNumber() > 15)
                    {
                        if(getActiveMonth() == 0)
                        {
                            targetsMonth = getMonthString(11);
                            targetsYear = std::to_string(getActiveYear() - 1);
                        }
                        else
                        {
                            targetsMonth = getMonthString(getActiveMonth() - 1);
                            targetsYear = std::to_string(getActiveYear());
                        }
                    }
                    else if(m_target->getNumber() < 15)
                    {
                        if(getActiveMonth() == 11)
                        {
                            targetsMonth = getMonthString(0);
                            targetsYear = std::to_string(getActiveYear() + 1);
                        }
                        else
                        {
                            targetsMonth = getMonthString(getActiveMonth() + 1);
                            targetsYear = std::to_string(getActiveYear());
                        }
                    }
                }
                else
                {
                    targetsMonth = getMonthString(getActiveMonth());
                    targetsYear = std::to_string(getActiveYear());
                }
                
                std::string number = std::to_string(m_target->getNumber());
                std::string buf = setMonthString(targetsMonth, number, targetsYear);
                
                updateDayColors();
                
                m_selectedDay.setString(buf);
                centerText(&m_selectedDay, 0, 1600, 740);
            }
            
            //Handle event
            if(m_target != NULL)
            {
                Event* clickedEvent = m_target->getClickedEvent(event->mouseButton.x, event->mouseButton.y);
                if(clickedEvent != NULL)
                {
                    m_targetTextField = NULL;
                    m_addNoteField->setBorder("normal");
                    m_editNoteField->setBorder("normal");
                    m_targetEvent = clickedEvent;
                    m_target->updateEventTextColor(clickedEvent);
                    m_deleteNoteButton.setPosition(980 + clickedEvent->getEventText()->getGlobalBounds().width + 4, clickedEvent->getEventText()->getPosition().y - 10);
                    m_editNoteButton.setPosition(1150 + clickedEvent->getEventText()->getGlobalBounds().width + 4, clickedEvent->getEventText()->getPosition().y - 10);
                }
            }
            
            //Handle add note button
            if(m_target != NULL)
            {
                if(spriteIncludesPoint(&m_addNoteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    if(m_addNoteField->getValue() != "")
                    {
                        Event* newEvent = new Event(m_addNoteField->getValue(), getActiveMonth(), m_target->getNumber(), getActiveYear());
                        m_allEvents.push_back(newEvent);
                        m_target->addEvent(newEvent);
                        m_addNoteField->setValue("");
                    }
                }
            }
            
            //Handle event delete button
            if(m_targetEvent != NULL)
            {
                if(spriteIncludesPoint(&m_deleteNoteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    if(m_target->deleteEvent(m_targetEvent) == true)
                    { //if deleting the event from the day was successful
                        deleteEvent(m_targetEvent); //delete the event completely.
                        m_targetEvent = NULL;
                    }
                }
            }
            
            //Handle event edit button
            if(m_targetEvent != NULL)
            {
                if(spriteIncludesPoint(&m_editNoteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    m_editNoteField->setValue(m_targetEvent->getTitle());
                }
            }
            
            //Handle event second edit button
            if(m_targetEvent != NULL)
            {
                if(spriteIncludesPoint(&m_secondEditNoteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    if(m_editNoteField->getValue() != "")
                    {
                        if(m_target->deleteEvent(m_targetEvent) == true)
                        {
                            deleteEvent(m_targetEvent);
                            m_targetEvent = NULL;
                        }
                        
                        Event* newEvent = new Event(m_editNoteField->getValue(), getActiveMonth(), m_target->getNumber(), getActiveYear());
                        m_allEvents.push_back(newEvent);
                        m_target->addEvent(newEvent);
                        m_editNoteField->setValue("");
                    }
                }
            }
        }
    }
    
    
    if(event->type == sf::Event::KeyPressed)
    {
        //Handle pressed delete keyboard button
        if(event->key.code == sf::Keyboard::Delete)
        {
            if(m_targetEvent != NULL)
            {
                if(m_target->deleteEvent(m_targetEvent) == true)
                {
                    deleteEvent(m_targetEvent);
                    m_targetEvent = NULL;
                }
            }
        }
        
        //Handle pressed return keyboard button
        if(event->key.code == sf::Keyboard::Return)
        {
            if(m_target != NULL)
            {
                if(m_addNoteField->getValue() != "")
                {
                    Event* newEvent = new Event(m_addNoteField->getValue(), getActiveMonth(), m_target->getNumber(), getActiveYear());
                    m_allEvents.push_back(newEvent);
                    m_target->addEvent(newEvent);
                    m_addNoteField->setValue("");
                }
                
                if(m_editNoteField->getValue() != "")
                {
                    if(m_target->deleteEvent(m_targetEvent) == true)
                    {
                        deleteEvent(m_targetEvent);
                        m_targetEvent = NULL;
                    }
                    
                    Event* newEvent = new Event(m_editNoteField->getValue(), getActiveMonth(), m_target->getNumber(), getActiveYear());
                    m_allEvents.push_back(newEvent);
                    m_target->addEvent(newEvent);
                    m_editNoteField->setValue("");
                }
            }
        }
        
        //Handle pressed left keyboard button
        if(event->key.code == sf::Keyboard::Left)
        {
            prevMonth();
            updateDayColors();
        }
        
        //Handle pressed right keyboard button
        if(event->key.code == sf::Keyboard::Right)
        {
            nextMonth();
            updateDayColors();
        }
        
        //Handle pressed escape keyboard button
        if(event->key.code == sf::Keyboard::Escape)
        {
            m_target = NULL;
            m_targetEvent = NULL;
            m_targetTextField = NULL;
            updateDayColors();
        }
    }
}
                                                 
std::string Calendar::setMonthString(std::string month, std::string day, std::string year)
{
    std::string buf = month + " " + day + " " + year;
    
    return buf;
}

sf::Font* Calendar::getFont()
{
    return &m_font;
}

std::string Calendar::getMonthString(int month)
{
    std::string returnString = "";

    switch(month)
    {
        case 0:
            returnString = "January";
            break;
        case 1:
            returnString = "February";
            break;
        case 2:
            returnString = "March";
            break;
        case 3:
            returnString = "April";
            break;
        case 4:
            returnString = "May";
            break;
        case 5:
            returnString = "June";
            break;
        case 6:
            returnString = "July";
            break;
        case 7:
            returnString = "August";
            break;
        case 8:
            returnString = "September";
            break;
        case 9:
            returnString = "October";
            break;
        case 10:
            returnString = "November";
            break;
        case 11:
            returnString = "December";
            break;
    }
    
    return returnString;
}

int Calendar::getDaysInMonth(int month, int year)
{
    int result = 0;

    switch(month)
    {
        case 0:
            result = 31;
            break;
        case 1:
            if((year % 4 == false && year % 100 != false) || year % 400 == false)
            {
                result = 29;
            }
            else
            {
                result = 28;
            }
            break;
        case 2:
            result = 31;
            break;
        case 3:
            result = 30;
            break;
        case 4:
            result = 31;
            break;
        case 5:
            result = 30;
            break;
        case 6:
            result = 31;
            break;
        case 7:
            result = 31;
            break;
        case 8:
            result = 30;
            break;
        case 9:
            result = 31;
            break;
        case 10:
            result = 30;
            break;
        case 11:
            result = 31;
            break;
    }
    
    return result;
}

Day* Calendar::getClickedDay(int clickX, int clickY)
{
    Day* result = NULL;

    for(int d = 0; d < m_month.size(); d++)
    {
        if(clickX >= m_month[d]->getX() && clickX < m_month[d]->getX() + 221
           && clickY >= m_month[d]->getY() && clickY < m_month[d]->getY() + 100)
        {
            result = m_month[d];
        }
    }
    
    return result;
}

std::string Calendar::addSpaces(std::string str)
{
    for(int c = 0; c < str.length(); c++)
    {
        if(str[c] == '_')
        {
            str[c] = ' ';
        }
    }
    
    return str;
}

std::string Calendar::removeSpaces(std::string str)
{
    for(int c = 0; c < str.length(); c++)
    {
        if(str[c] == ' ')
        {
            str[c] = '_';
        }
    }
    
    return str;
}

void Calendar::centerText(sf::Text* text, int x1, int x2, int y)
{
    int width = text->getGlobalBounds().width;
    int x = x1 + ((x2 - x1) - width) / 2;
    text->setPosition(x, y);
}

void Calendar::updateDayColors()
{
    //make all unselected
    for(int i = 0; i < m_month.size(); i++)
    {
        m_month[i]->setSelected(false);
    }
    
    //select target
    if(m_target != NULL)
    {
        m_target->setSelected(true);
    }
}

bool Calendar::spriteIncludesPoint(sf::Sprite* sprite, int x, int y)
{
    if(x >= sprite->getPosition().x && x < sprite->getPosition().x + sprite->getGlobalBounds().width
    && y >= sprite->getPosition().y && y < sprite->getPosition().y + sprite->getGlobalBounds().height)
    {
        return true;
    }
    
    return false;
}

bool Calendar::rectIncludesPoint(sf::RectangleShape* rect, int x, int y)
{
    if(x >= rect->getPosition().x && x < rect->getPosition().x + rect->getGlobalBounds().width
    && y >= rect->getPosition().y && y < rect->getPosition().y + rect->getGlobalBounds().height)
    {
        return true;
    }
    
    return false;
}
