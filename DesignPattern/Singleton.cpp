#include <stdio.h>
#include <stdlib.h>
#include <map>
class Singleton
{
public:
    static Singleton* Instance();
protected:
    Singleton(){

    }

private:
    static Singleton* m_sSingleton;
};


Singleton* Singleton::m_sSingleton = NULL;


Singleton* Singleton::Instance()
{
    if(m_sSingleton == NULL){
        m_sSingleton = new Singleton;
    }
    return m_sSingleton;
}

class RegSingleton
{
public:
    static void Register(const char* name, RegSingleton*);
    static RegSingleton* Instance();

    typedef std::map<const char*, RegSingleton* > MpNameSingle;
protected:
    static RegSingleton* LookUp(const char* name);
private:
    static RegSingleton* m_RegSingleton;
    static MpNameSingle  m_mpNameSingle;
};

RegSingleton* RegSingleton::m_RegSingleton = NULL;
RegSingleton::MpNameSingle RegSingleton::m_mpNameSingle;

void RegSingleton::Register(const char* name, RegSingleton* reg){
    m_mpNameSingle[name] = reg;
}

RegSingleton* RegSingleton::LookUp(const char* name){
    MpNameSingle::iterator it = m_mpNameSingle.find(name);
    if(it == m_mpNameSingle.end())
        return NULL;
    return (it->second);
}



RegSingleton* RegSingleton::Instance(){

    if(m_RegSingleton == NULL){
        const char* singletonName = getenv("SINGLETON");
        m_RegSingleton = LookUp(singletonName);
    }
    return m_RegSingleton;
}

class MySingleton: public RegSingleton
{
public:
    MySingleton(){
        RegSingleton::Register("MySingleton", this);
    }
    void Test(){
        printf("test\n");
    }
};

static MySingleton theSingleton;

int main(int argc, char *argv[])
{
    theSingleton.Test();
    return 0;
}
