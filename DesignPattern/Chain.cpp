#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


typedef int TOPIC;
const TOPIC NO_HELP_TOPIC = -1;

class HelpHandle
{
public:
    HelpHandle(HelpHandle* =0, TOPIC = NO_HELP_TOPIC);
    virtual ~HelpHandle(){

    }

    virtual void SetHandle(HelpHandle* handle, TOPIC top);

    virtual bool HasHelp();
    virtual void HandleHelp();
private:
    HelpHandle* m_successsor;
    TOPIC m_topic;
};


HelpHandle::HelpHandle(HelpHandle* handle, TOPIC top): m_successsor(handle), m_topic(top){

}

void HelpHandle::SetHandle(HelpHandle* handle, TOPIC top){
    m_successsor = handle;
    m_topic = top;
}

bool HelpHandle::HasHelp(){
    return m_topic != NO_HELP_TOPIC;
}

void HelpHandle::HandleHelp(){
    if(m_successsor != NULL){
        m_successsor->HandleHelp();
    }
}


class Widget: public HelpHandle
{
public:
    virtual ~Widget(){

    }
protected:
    Widget(Widget* parent, TOPIC t = NO_HELP_TOPIC);

private:
    Widget* m_parent;
};

Widget::Widget(Widget* parent, TOPIC t): HelpHandle(parent, t)
{
    m_parent =  parent;
}

class Button : public Widget
{
public:
    Button(Widget* d, TOPIC t= NO_HELP_TOPIC): Widget(d,t){
    }

    void HandleHelp(){
        std::cout << "Button HandleHelp" << std::endl;
        if(HasHelp())
        {

        }
        else
        {
            HelpHandle::HandleHelp();
        }
    }
};



class Dialog:public Widget
{
public:
    Dialog(HelpHandle* d, TOPIC t = NO_HELP_TOPIC): Widget(0){
        SetHandle(d, t);
    }
    void HanleHelp(){
        std::cout << "Dialog HandleHelp" << std::endl;
        if(HasHelp())
        {

        }
        else
        {
            HelpHandle::HandleHelp();
        }
    }
};


class Application: public HelpHandle
{
public:
    Application(TOPIC t):HelpHandle(0,t){

    }

    virtual void HandleHelp(){

    }

};


int main(int argc, char *argv[])
{
    const TOPIC PRINT_TOPIC = 1;
    const TOPIC PAPER_ORIENTATION_TOPIC = 2;
    const TOPIC APPLICATION_TOPIC = 3;

    Application* pplication =  new Application(APPLICATION_TOPIC);
    Dialog* pDialog = new Dialog(pplication, PRINT_TOPIC);
    Button* pButton = new Button(pDialog, PAPER_ORIENTATION_TOPIC);

    pButton->HandleHelp();
    return 0;
}

