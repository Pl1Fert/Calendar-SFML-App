#include "Calendar.hpp"

Calendar* Calendar::m_instance;

Calendar::Calendar()
{
    m_window.create(sf::VideoMode(1600, 1200, 32), "Calendar", sf::Style::Close);//was 800x600
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true);
    
    m_font.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Data/arial.ttf");
    
    m_currentTime = time(0);
    m_currentDate = localtime(&m_currentTime);
    
    m_target = NULL;
    //targetEvent_ = NULL;
    
    m_activeMonth = m_currentDate->tm_mon;
    m_activeYear = m_currentDate->tm_year + 1900;
    
    m_monthLabel.setString(getMonthString(m_currentDate->tm_mon) + " " + convertIntToString(m_currentDate->tm_year + 1900));
    m_monthLabel.setFont(m_font);
    m_monthLabel.setFillColor(sf::Color::Black);
    m_monthLabel.setCharacterSize(30);
    m_monthLabel.setStyle(sf::Text::Bold);
    centerText(&m_monthLabel, 0, 1600, 10);
    
    m_daysLabel.setString("     Monday      Tuesday      Wednesday       Thursday         Friday          Saturday           Sunday     ");
    m_daysLabel.setPosition(0, 50);
    m_daysLabel.setFont(m_font);
    m_daysLabel.setFillColor(sf::Color::Black);
    m_daysLabel.setCharacterSize(24);
    m_daysLabel.setStyle(sf::Text::Bold);
    
    m_leftSelectorImage.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Images/SelectorLeft.png");
    m_rightSelectorImage.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Images/SelectorRight.png");
    m_leftSelector.setTexture(m_leftSelectorImage);
    m_leftSelector.setPosition(600, 10);
    m_rightSelector.setTexture(m_rightSelectorImage);
    m_rightSelector.setPosition(980, 10);
    
    m_dividerImage.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Images/Divider.png");
    m_divider.setTexture(m_dividerImage);
    m_divider.setPosition(320, 448);
    m_selectedDay.setString("");
    m_selectedDay.setFont(m_font);
    m_selectedDay.setFillColor(sf::Color::Black);
    m_selectedDay.setCharacterSize(16);
    m_selectedDay.setStyle(sf::Text::Bold);
    
    m_deleteButtonImage.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Images/DeleteButton2.png");
    m_deleteButton.setTexture(m_deleteButtonImage);
    
    m_addNoteButtonImage.loadFromFile("/Users/user/Documents/прога универ/сем 3/Calendar/Calendar/Images/AddNote.png");
    m_addNoteButton.setTexture(m_addNoteButtonImage);
    m_addNoteButton.setPosition(300, 510);
    
    m_targetTextField = NULL;
    m_addNote.setString("Add a note: ");
    m_addNote.setFont(m_font);
    m_addNote.setPosition(11, 442);
    m_addNote.setFillColor(sf::Color::Black);
    m_addNote.setCharacterSize(18);
    m_addNote.setStyle(sf::Text::Bold);
    m_titleField = new TextField("text", "Note: ", 10, 475);
    m_typeBox = new Checkbox("Type: ", 10, 515);
    m_typeBox->CreateOption("Unique ", sf::Color::Black, "Unique notes are for a specific day of a specific year. \nThey do not carry over to future years.", 80, 515);
    m_typeBox->CreateOption("Persistent ", sf::Color::Blue, "Persistent notes will appear on the same day of every \nyear. Useful for birthdays and such.", 180, 515);
    
    loadUniqueEvents();
    constructMonth();
}

Calendar::~Calendar()
{
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
                save();
                m_window.close();
            }
            
            handleInput(&m_event);
            
            if(m_target != NULL && m_targetTextField != NULL)
            {
                m_targetTextField->HandleInput(&m_event);
            }
            
            if(m_target != NULL)
            {
                m_typeBox->HandleInput(&m_event);
                
            }
            
        }
        
        m_window.clear(sf::Color::White);
        
        m_window.draw(m_leftSelector);
        m_window.draw(m_monthLabel);
        m_window.draw(m_rightSelector);
        m_window.draw(m_daysLabel);
        
        for(int d = 0; d < m_month.size(); d++)
        {
            m_month[d]->Draw(&m_window);
        }
        
        m_window.draw(m_divider);
        
        if(m_target != NULL)
        {
            m_window.draw(m_selectedDay);
            
            if(m_target->GetType() != "inactive")
            {
                m_window.draw(m_addNote);
                m_titleField->Draw(&m_window);
                m_typeBox->Draw(&m_window);
                m_window.draw(m_addNoteButton);
            }
        }
        
        if(m_target != NULL)
        {
            m_target->DrawEvents(&m_window);
            
        }
        if(m_target != NULL && m_targetEvent != NULL)
        {
            m_window.draw(m_deleteButton);
            
        }
        
        m_window.display();
    }
}

string Calendar::convertIntToString(int n)
{
    string returnString = to_string(n);
    return returnString;
}

void Calendar::centerText(sf::Text* text, int x1, int x2, int y)
{
    int width = text->getGlobalBounds().width;
    int x = x1 + ((x2 - x1) - width) / 2;
    text->setPosition(x, y);
}

void Calendar::updateDayColors()
{
    //First, make sure all days are unselected.
    for(int i = 0; i < m_month.size(); i++)
    {
        m_month[i]->SetSelected(false);
    }
    
    //Then, select the current target.
    if(m_target != NULL)
    {
        m_target->SetSelected(true);
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

string Calendar::addSpaces(string str)
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

string Calendar::removeSpaces(string str)
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

void Calendar::loadUniqueEvents()
{
    ifstream file;
    file.open("./Data/UniqueEvents.cll");
    
    string title = "";
    int month = 0, day = 0, year = 0;
    
    while(file >> title)
    {
        file >> month;
        file >> day;
        file >> year;
        
        m_uniqueEvents.push_back(new Event(addSpaces(title), month, day, year));
    }
    
    file.close();
}

void Calendar::deleteEvent(Event* event)
{
    string type = event->GetType();
    
    if(type == "persistent")
    {
        for(int p = 0; p < m_persistentEvents.size(); p++)
        {
            if(m_persistentEvents[p] == event)
            {
                m_persistentEvents.erase(m_persistentEvents.begin() + p);
            }
        }
    }
    else if(type == "unique")
    {
        for(int p = 0; p < m_uniqueEvents.size(); p++)
        {
            if(m_uniqueEvents[p] == event)
            {
                m_uniqueEvents.erase(m_uniqueEvents.begin() + p);
            }
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

void Calendar::drawMonth()
{
    m_monthLabel.setString(getMonthString(m_activeMonth) + " " + convertIntToString(m_activeYear));
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
    if(m_activeMonth != 0)
    {
        daysInLastMonth = getDaysInMonth(m_activeMonth - 1, m_activeYear);
    }
    else
    {
        daysInLastMonth = getDaysInMonth(11, m_activeYear - 1);
    }
    int daysInThisMonth = getDaysInMonth(m_activeMonth, m_activeYear);
    
    int iterator = 0;

    if(dayOfTheWeek == 0)
    {
        dayOfTheWeek += 7;
    }
    
    //set last month's days.
    for(int n = daysInLastMonth - (dayOfTheWeek-1); n <= daysInLastMonth; n++)
    {
        m_month[iterator]->SetNumber(n);
        m_month[iterator]->SetType("inactive");
        iterator++;
    }
    
    //set this month's days.
    for(int i = 1; i <= daysInThisMonth; i++)
    {
        m_month[iterator]->SetNumber(i);
        
        if(i == m_currentDate->tm_mday && m_activeMonth == m_currentDate->tm_mon && m_activeYear == m_currentDate->tm_year + 1900)
        {
            m_month[iterator]->SetType("current");
        }
        else
        {
            m_month[iterator]->SetType("default");
        }

        iterator++;
    }
    
    //set next month's days.
    for(int r = 1; iterator < m_month.size(); r++)
    {
        m_month[iterator]->SetNumber(r);
        m_month[iterator]->SetType("inactive");
        iterator++;
    }
            
    //add any unique events
    for(int ue = 0; ue < m_uniqueEvents.size(); ue++)
    {
        if(m_uniqueEvents[ue]->GetMonth() == m_activeMonth && m_uniqueEvents[ue]->GetYear() == m_activeYear)
        {
            for(int day = 0; day < m_month.size(); day++)
            {
                if(m_month[day]->GetNumber() == m_uniqueEvents[ue]->GetDay())
                {
                    if(m_month[day]->GetType() == "default" || m_month[day]->GetType() == "current")
                    {
                        //month_[day]->AddContent(uniqueEvents_[ue]->GetTitle());
                        m_month[day]->AddEvent(m_uniqueEvents[ue]);
                    }
                }
            }
        }
    }
}

void Calendar::constructMonth()
{
    //create day objects.
    for(int h = 0; h < 6; h++)
    {
        for(int w = 0; w < 7; w++)
        {
            m_month.push_back(new Day(11 + (w * 154), 70 + (h * 81), &m_font));//посчитать потом под 1600 на 1200
        }
    }
    
    //Tomohiko Sakamoto’s Algorithm
    int currentYear = m_currentDate->tm_year;
    int currentMonth = m_currentDate->tm_mon + 1;
    
    int dayOfTheWeek;
    int day = 1;
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        
    if(currentMonth < 3)
        currentYear--;
    
    dayOfTheWeek = ((currentYear + currentYear / 4 - currentYear / 100 + currentYear / 400 + t[currentMonth - 1] + day) % 7);
    
    int daysInLastMonth = 0;
    if(m_currentDate->tm_mon != 0)
    {
        daysInLastMonth = getDaysInMonth(m_currentDate->tm_mon - 1, m_currentDate->tm_year + 1900);
    }
    else
    {
        daysInLastMonth = getDaysInMonth(11, (m_currentDate->tm_year - 1) + 1900);
    }
    int daysInThisMonth = getDaysInMonth(m_currentDate->tm_mon, m_currentDate->tm_year + 1900);
    
    if(dayOfTheWeek == 0)
    {
        dayOfTheWeek += 7;
    }
    
    int iterator = 0;
    
    //set last month's days.
    for(int n = daysInLastMonth - (dayOfTheWeek); n <= daysInLastMonth; n++)
    {
        m_month[iterator]->SetNumber(n);
        m_month[iterator]->SetType("inactive");
        iterator++;
    }
    
    //set this month's days.
    for(int i = 1; i <= daysInThisMonth; i++)
    {
        m_month[iterator]->SetNumber(i);
    
        if(i == m_currentDate->tm_mday)
        {
            m_month[iterator]->SetType("current");
        }
        else
        {
            m_month[iterator]->SetType("default");
        }
        
        iterator++;
    }
    
    //set next month's days.
    for(int i = 1; iterator < m_month.size(); i++)
    {
        m_month[iterator]->SetNumber(i);
        m_month[iterator]->SetType("inactive");
        iterator++;
    }
        
    //add any unique events.
    for(int ue = 0; ue < m_uniqueEvents.size(); ue++)
    {
        if(m_uniqueEvents[ue]->GetMonth() == m_activeMonth && m_uniqueEvents[ue]->GetYear() == m_activeYear)
        {
            for(int day = 0; day < m_month.size(); day++)
            {
                if(m_month[day]->GetNumber() == m_uniqueEvents[ue]->GetDay())
                {
                    if(m_month[day]->GetType() == "default" || m_month[day]->GetType() == "current")
                    {
                        m_month[day]->AddEvent(m_uniqueEvents[ue]);
                    }
                }
            }
        }
    }
}

void Calendar::nextMonth()
{
    m_target = NULL;
    m_targetEvent = NULL;
    m_targetTextField = NULL;
    
    //clear all content
    for(int yad = 0; yad < m_month.size(); yad++)
    {
        m_month[yad]->ClearContent();
    }
        
    if(m_activeMonth == 11)
    {
        m_activeMonth = 0;
        m_activeYear++;
    }
    else
    {
        m_activeMonth++;
    }
    
    drawMonth();
}

void Calendar::prevMonth()
{
    m_target = NULL;
    m_targetEvent = NULL;
    m_targetTextField = NULL;

    //clear all content
    for(int yad = 0; yad < m_month.size(); yad++)
    {
        m_month[yad]->ClearContent();
    }
    
    //change current month.
    if(m_activeMonth == 0)
    {
        m_activeMonth = 11;
        m_activeYear--;
    }
    else
    {
        m_activeMonth--;
    }
    
    drawMonth();
}

void Calendar::handleInput(sf::Event* event)
{
    if(event->type == sf::Event::MouseButtonPressed)
    {
        if(event->mouseButton.button == sf::Mouse::Left)
        {
            //Handle directional arrows.
            if(spriteIncludesPoint(&m_leftSelector, event->mouseButton.x, event->mouseButton.y) == true)
            {
                prevMonth();
            }
            else if(spriteIncludesPoint(&m_rightSelector, event->mouseButton.x, event->mouseButton.y) == true)
            {
                nextMonth();
            }
            
            //Handle the text input field.
            if(rectIncludesPoint(m_titleField->GetRect(), event->mouseButton.x, event->mouseButton.y) == true)
            {
                m_targetTextField = m_titleField;
                m_targetTextField->SetBorder("targeted");
            }
            
            //Handle days and their events.
            Day* day = getClickedDay(event->mouseButton.x, event->mouseButton.y);
            if(day != NULL)
            {
                m_target = day;
                
                m_targetEvent = NULL;
                m_targetTextField = NULL;
                m_titleField->SetBorder("normal");
                m_target->UpdateEventTextColor(m_targetEvent);
                
                string targetsMonth = "";
                string targetsYear = "";
                
                if(m_target->GetType() == "inactive")
                {
                    if(m_target->GetNumber() > 15)
                    {
                        if(m_activeMonth == 0)
                        {
                            targetsMonth = getMonthString(11);
                            targetsYear = convertIntToString(m_activeYear - 1);
                        }
                        else
                        {
                            targetsMonth = getMonthString(m_activeMonth - 1);
                            targetsYear = convertIntToString(m_activeYear);
                        }
                    }
                    else if(m_target->GetNumber() < 15)
                    {
                        if(m_activeMonth == 11)
                        {
                            targetsMonth = getMonthString(0);
                            targetsYear = convertIntToString(m_activeYear + 1);
                        }
                        else
                        {
                            targetsMonth = getMonthString(m_activeMonth + 1);
                            targetsYear = convertIntToString(m_activeYear);
                        }
                    }
                }
                else
                {
                    targetsMonth = getMonthString(m_activeMonth);
                    targetsYear = convertIntToString(m_activeYear);
                }
                
                string proxy = targetsMonth;
                proxy += " ";
                proxy += convertIntToString(m_target->GetNumber());
                proxy += " ";
                proxy += targetsYear;
                
                updateDayColors();
                
                m_selectedDay.setString(proxy);
                centerText(&m_selectedDay, 0, 800, 440);
            }
            
            if(m_target != NULL)
            {
                Event* clickedEvent = m_target->GetClickedEvent(event->mouseButton.x, event->mouseButton.y);
                if(clickedEvent != NULL)
                {
                    m_targetTextField = NULL;
                    m_titleField->SetBorder("normal");
                    m_targetEvent = clickedEvent;
                    m_target->UpdateEventTextColor(clickedEvent);
                    m_deleteButton.setPosition(420 + clickedEvent->GetEventText()->getGlobalBounds().width + 4, clickedEvent->GetEventText()->getPosition().y);
                }
            }
            
            //Handle add note button.
            if(m_target != NULL)
            {
                if(spriteIncludesPoint(&m_addNoteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    if(m_titleField->GetValue() != "")
                    {
                        if(m_typeBox->GetValue() == "Unique ")
                        {
                            Event* newEvent = new Event(m_titleField->GetValue(), m_activeMonth, m_target->GetNumber(), m_activeYear);
                            m_uniqueEvents.push_back(newEvent);
                            m_target->AddEvent(newEvent);
                            m_titleField->SetValue("");
                        }
                        else if(m_typeBox->GetValue() == "Persistent ")
                        {
                            Event* newEvent = new Event(m_titleField->GetValue(), m_activeMonth, m_target->GetNumber());
                            m_persistentEvents.push_back(newEvent);
                            m_target->AddEvent(newEvent);
                            m_titleField->SetValue("");
                        }
                    }
                }
            }
            
            //Handle event delete button.
            if(m_targetEvent != NULL)
            {
                if(spriteIncludesPoint(&m_deleteButton, event->mouseButton.x, event->mouseButton.y) == true)
                {
                    if(m_target->DeleteEvent(m_targetEvent) == true)
                    { //if deleting the event from the day was successful (for visual reasons)
                        deleteEvent(m_targetEvent); //delete the event completely.
                        m_targetEvent = NULL;
                    }
                }
            }
            
        }
    }
    if(event->type == sf::Event::KeyPressed)
    {
        if(event->key.code == sf::Keyboard::Delete)
        {
            if(m_targetEvent != NULL)
            {
                if(m_target->DeleteEvent(m_targetEvent) == true)
                { //if deleting the event from the day was successful
                    deleteEvent(m_targetEvent); //delete the event completely.
                    m_targetEvent = NULL;
                }
            }
        }
        
        if(event->key.code == sf::Keyboard::Return)
        {
            if(m_target != NULL)
            {
                if(m_titleField->GetValue() != "")
                {
                    if(m_typeBox->GetValue() == "Unique ")
                    {
                        Event* newEvent = new Event(m_titleField->GetValue(), m_activeMonth, m_target->GetNumber(), m_activeYear);
                        m_uniqueEvents.push_back(newEvent);
                        m_target->AddEvent(newEvent);
                        m_titleField->SetValue("");
                    }
                    else if(m_typeBox->GetValue() == "Persistent ")
                    {
                        Event* newEvent = new Event(m_titleField->GetValue(), m_activeMonth, m_target->GetNumber());
                        m_persistentEvents.push_back(newEvent);
                        m_target->AddEvent(newEvent);
                        m_titleField->SetValue("");
                    }
                }
            }
        }
        
        if(event->key.code == sf::Keyboard::Left)
        {
            prevMonth();
            updateDayColors();
        }
        if(event->key.code == sf::Keyboard::Right)
        {
            nextMonth();
            updateDayColors();
        }
        if(event->key.code == sf::Keyboard::Escape)
        {
            m_target = NULL;
            m_targetEvent = NULL;
            m_targetTextField = NULL;
            updateDayColors();
        }
        
    }
}

void Calendar::save()
{    
    //Save unique events.
    ofstream uniqueEventFile;
    uniqueEventFile.open("./Data/uniqueEvents.cll");

    for(int u = 0; u < m_uniqueEvents.size(); u++)
    {
        uniqueEventFile << removeSpaces(m_uniqueEvents[u]->GetTitle()) << " ";
        uniqueEventFile << m_uniqueEvents[u]->GetMonth() << " ";
        uniqueEventFile << m_uniqueEvents[u]->GetDay() << " ";
        uniqueEventFile << m_uniqueEvents[u]->GetYear() << " ";
        uniqueEventFile << "\n";
    }
    
    uniqueEventFile.close();
}

sf::Font* Calendar::getFont()
{
    return &m_font;
}

string Calendar::getMonthString(int monthNum)
{
    string returnString = "";

    switch(monthNum)
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
        case 0: result = 31;
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
        if(clickX >= m_month[d]->GetX() && clickX < m_month[d]->GetX() + 110
        && clickY >= m_month[d]->GetY() && clickY < m_month[d]->GetY() + 60)
        {
            result = m_month[d];
        }
    }
    
    return result;
}













