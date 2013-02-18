#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <string.h>

class Command
{
public:
    virtual ~Command(){

    }

    virtual void Excute() = 0;
protected:
    Command(){    }
};

class Document
{
public:
    Document(): m_user(NULL){

    }
    Document(const char* name){
        m_user = strdup(name);
    }
    void Open(){
        printf("Open\n");
    }

    void Close(){
        printf("close\n");
    }

    void Cut(){
        printf("cut\n");
    }
    void Paste(){
        printf("paste\n");
    }

private:
    char* m_user;
};

class Application
{
public:
    void Add(Document*){
        printf("Add Document\n");
    }
private:
};
class OpenCommand: public Command
{
public:
    OpenCommand(Application*);

    virtual void Excute();

protected:
    virtual const char* AskUser();

private:
    Application* m_app;
    char* response;
};


OpenCommand::OpenCommand(Application* app)
{
    m_app = app;
}


void OpenCommand::Excute()
{
    const char* name = AskUser();
    if(name != NULL)
    {
        Document* pDoc = new Document(name);
        m_app->Add(pDoc);
        pDoc->Open();
    }
}

const char* OpenCommand::AskUser()
{
    return "123";
}



class PasteCommand:public Command
{
public:
    PasteCommand(Document *);
    virtual ~PasteCommand();

    virtual void Excute();
private:
    Document* m_doc;
};

PasteCommand::PasteCommand(Document* doc): m_doc(doc){

}

PasteCommand::~PasteCommand(){
    m_doc = NULL;
}


void PasteCommand::Excute()
{
    if(m_doc != NULL)
        m_doc->Paste();
}

template<class Receiver>
class SimpleCommand : public Command
{
public:
    typedef void (Receiver::*Action)();
    SimpleCommand(Receiver* r, Action a): m_rece(r), m_act(a){
    }
    virtual ~SimpleCommand() {

    }

    virtual void Excute();
private:
    Action m_act;
    Receiver* m_rece;
};

template <class Receiver>
void SimpleCommand<Receiver>::Excute()
{
    (m_rece->*m_act)();
}

class MyClass
{
public:
    MyClass(){

    }
    virtual ~MyClass(){

    }

    void Action(){
        printf("action\n");
    }
};

class MacroCommand:public Command
{
public:
    MacroCommand(){

    }
    virtual ~MacroCommand(){

    }

    virtual void Add(Command*);
    virtual void Remove(Command*);

    virtual void Excute();

protected:
    bool HasCommand(Command*);

private:
    std::vector<Command* > m_vecCommand;
};


bool MacroCommand::HasCommand(Command* cmd)
{
    std::vector<Command* >::iterator it = std::find (m_vecCommand.begin(), m_vecCommand.end(), cmd);
    if(it != m_vecCommand.end())
        return true;
    else
        return false;
}

void MacroCommand::Add(Command* cmd)
{
    if(HasCommand(cmd))
        Remove(cmd);
    m_vecCommand.push_back(cmd);
}

void MacroCommand::Remove(Command* cmd)
{
    std::vector<Command* >::iterator it = std::find (m_vecCommand.begin(), m_vecCommand.end(), cmd);
    if(it != m_vecCommand.end())
        m_vecCommand.erase(it);
}

void MacroCommand::Excute()
{
    std::vector<Command* >::iterator it ;
    for(it = m_vecCommand.begin(); it!= m_vecCommand.end(); it++)
    {
        (*it)->Excute();
    }
}




int main(int argc, char *argv[])
{
    Application* pApp = new Application;
    Command* pCmd = new OpenCommand(pApp);
    pCmd->Excute();


    Document* pDoc = new Document;
    Command* pCommand = new PasteCommand(pDoc);
    pCommand->Excute();

    MyClass* receiver = new MyClass;
    Command* aCommand = new SimpleCommand<MyClass>(receiver, &MyClass::Action);
    aCommand->Excute();
    return 0;
}
