#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>

#include <map>

typedef void * HANDLE;

//The Logging Handler is response for receiving and process Logging records
//The Logging Acceptor creates and connects Logging Handles that process subsequet
// loging records from clients


enum eEventType
{
    eAcceptEvent = 01,
    eReadEvent = 02,
    eWriteEvent = 04,
    eTimeoutEvent = 010,
    eSignalEvent = 020,
    eCloseEvent =040
};

class EventHandle
{
public:
//    virtual int HandleAccpet() = 0;
//    virtual int HandleInput() = 0;
//    virtual int HandleOutput() = 0;
//    virtual int HandleTimeout() = 0;
//    virtual int HandleClose() = 0;

    virtual void HandleEvents(eEventType& et) = 0;

    virtual HANDLE GetHandle() const = 0;
};

class Time_Value
{

};

//TITLE
//Demulitplex and dispatch Event_Handles
//in response to client request
class InitiationDispatcher
{
public:
    static InitiationDispatcher* Instance();
    static InitiationDispatcher* m_sDispatcher;
public:
    int RegisterHandle(EventHandle *eh, eEventType et);
    int RemoveHandle(EventHandle* eh, eEventType et);
    int HandleEvents(Time_Value* timeout = 0);
private:
    InitiationDispatcher(){
    };
    typedef std::map<EventHandle*, eEventType> MpEvent;
    MpEvent m_mpEvent;
};

InitiationDispatcher*  InitiationDispatcher::m_sDispatcher = NULL;

InitiationDispatcher* InitiationDispatcher::Instance()
{
    if(m_sDispatcher == NULL)
        m_sDispatcher = new InitiationDispatcher;
    return m_sDispatcher;
}
int InitiationDispatcher::RegisterHandle(EventHandle* sh, eEventType et){
    m_mpEvent[sh] = et;
    return 0;
}

int InitiationDispatcher::RemoveHandle(EventHandle* eh, eEventType et){
    return 0;
}

int InitiationDispatcher:: HandleEvents(Time_Value* timeout){
    MpEvent::iterator it ;
    for(it = m_mpEvent.begin(); it!= m_mpEvent.end(); it++){
        EventHandle* pEvent = it->first;
        pEvent->HandleEvents(it->second);
    }
    return 0;
}


typedef sockaddr_in SockStream;


class SockAcceptor
{
public:
    virtual HANDLE GetHandle() const {
        return m_handle;
    }
    virtual void Accept(const SockStream& stream){

    }
private:
    HANDLE* m_handle;
};

class LoggingHandle: public EventHandle
{
public:
    LoggingHandle(const SockStream& stream): m_peerStream(stream){
        InitiationDispatcher::Instance()->RegisterHandle(this, eReadEvent);
    }
    virtual void  HandleEvents(eEventType& et) {
        printf("LoggingHandle Events\n");
        if( et == eReadEvent)
        {

        }
        else if(et == eWriteEvent)
        {

        }
    }

    virtual HANDLE GetHandle() const{
        return NULL;
    }

private:
    SockStream m_peerStream;
};

class LoggingAcceptor
{
public:
    LoggingAcceptor(const SockStream& addr){

    }
    virtual void HandleEvents(eEventType et)
    {
        assert(et == eAcceptEvent);
        SockStream newConnection;
        m_acceptor->Accept(newConnection);
        LoggingHandle* handle = new LoggingHandle(newConnection);
    }
    virtual HANDLE gethandle() const {
        return m_acceptor->GetHandle();
    }
private:
    SockAcceptor* m_acceptor;
};

const u_short uport = 10000;


int main(int argc, char *argv[])
{
    SockStream  addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(uport);

    LoggingAcceptor la(addr);
    for(;;)
        InitiationDispatcher::Instance()->HandleEvents();
    return 0;
}
