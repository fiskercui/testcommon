#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>

class Subject;
class Observer
{
public:
    virtual ~Observer(){

    }

    virtual void Update(Subject* sub) = 0;

protected:
    Observer(){

    }
};


class Subject
{
public:
    virtual ~Subject(){

    }

    virtual void Attach(Observer*);
    virtual void Detach(Observer*);

    virtual void Notify();
protected:
    Subject(){

    }

private:
    std::vector<Observer * > m_vecObserver;
};

void Subject::Attach(Observer* p)
{
    std::vector<Observer*>::iterator it;
    it = find(m_vecObserver.begin(), m_vecObserver.end(),p);
    if(it == m_vecObserver.end())
    {
        m_vecObserver.push_back(p);
    }
    else
    {
        *it = p;
    }
}

void Subject::Detach(Observer * p)
{
    std::vector<Observer*>::iterator it;
    it = find(m_vecObserver.begin(), m_vecObserver.end(),p);
    if(it == m_vecObserver.end())
    {
    }
    else
    {
        m_vecObserver.erase(it);
    }
}

void Subject::Notify()
{
    std::vector<Observer*>::iterator it;
    for(it = m_vecObserver.begin(); it != m_vecObserver.end(); it++)
    {
        if(*it != NULL)
        {
            (*it)->Update(this);
        }
    }
}


class ClockTimer: public Subject
{
public:
    ClockTimer(){

    }

    void Tick();

};

void ClockTimer::Tick()
{
    Notify();
}



class Widget
{

};

class DigitalClock: public Widget, public Observer
{
public:
    DigitalClock(ClockTimer* ct): m_subject(ct){
        m_subject->Attach(this);
    }
    virtual ~DigitalClock() {
        m_subject->Detach(this);
    }
    virtual void Update(Subject*){
        printf("DigitalClock  Update\n");
    }

    virtual void Draw()
    {

    }
private:
    ClockTimer* m_subject;
};

class AnalogClock: public Widget, public Observer
{
public:
    AnalogClock(ClockTimer* ct) : m_subject(ct){
        m_subject->Attach(this);
    }

    virtual ~AnalogClock(){
        m_subject->Detach(this);
    }

    virtual void Update(Subject*){
        printf("AnalogClock update\n");
    }
    virtual void Draw(){

    }
private:
    ClockTimer* m_subject;

};


int main(int argc, char *argv[])
{
    ClockTimer* timer = new ClockTimer;
    AnalogClock* pAc = new AnalogClock(timer);
    DigitalClock* pDc = new DigitalClock(timer);

    timer->Tick();
    return 0;
}

