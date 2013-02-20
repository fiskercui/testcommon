#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
//reference Dahua Sheji Moshi


class Action
{
public:
    virtual ~Action() {
    }
    virtual void ManConclusion() = 0;
    virtual void WomenConclusion() = 0;
protected:
    Action(){   }
};

class Success: public Action
{
public:
    virtual ~Success() {

    }
    virtual void ManConclusion(){
        printf("Success ManConclusion\n");
    }
    virtual void WomenConclusion(){
        printf("Success WomenConclusion\n");
    }
};

class Failing : public Action
{
public:
    virtual ~Failing(){

    }
    virtual void ManConclusion(){
        printf("Failing ManConclusion\n");
    }
    virtual void WomenConclusion(){
        printf("Failing WomenConclusion\n");
    }
};

class Marriage: public Action
{
public:
    virtual ~Marriage() {
    }

    virtual void ManConclusion(){
        printf("Marriage ManConclusion\n");
    }
    virtual void WomenConclusion(){
        printf("Marriage WomenConclusion\n");
    }
};

class Person{
public:
    virtual ~Person() {    }

    virtual void Accept(Action* act) = 0;
};

class Man:public Person{
public:
    virtual ~Man() {
    }
    virtual void Accept(Action* act){
        act->ManConclusion();
    }
};

class Woman: public Person{
public:
    virtual ~Woman() {
    }
    virtual void Accept(Action* act){
        act->WomenConclusion();
    }
};

class ObjectStructure
{
public:

    void Attach(Person* per);
    void Detach(Person* per);

    void Display(Action* pAct) ;
private:
    std::vector<Person* > m_vecObj;
};

void ObjectStructure::Attach(Person * p){
    std::vector<Person* >::iterator it;
    it = find (m_vecObj.begin(), m_vecObj.end(), p);
    if(it == m_vecObj.end()){
        m_vecObj.push_back(p);
    }
}

void ObjectStructure::Detach(Person *p){
    std::vector<Person* >::iterator it;
    it = find (m_vecObj.begin(), m_vecObj.end(), p);
    if(it != m_vecObj.end()){
        m_vecObj.erase(it);
    }
}

void ObjectStructure::Display(Action* pAct){
    std::vector<Person *>::iterator it ;
    for(it = m_vecObj.begin(); it!= m_vecObj.end(); it++)
    {
        (*it)->Accept(pAct);
    }
}

int main(int argc, char *argv[])
{
    ObjectStructure* pObj = new ObjectStructure;
    Person* pMan = new Man;
    Person* pWoman = new Woman;
    pObj->Attach(pMan);
    pObj->Attach(pWoman);

    Success* pSuccess = new Success;
    pObj->Display(pSuccess);


    return 0;
}

