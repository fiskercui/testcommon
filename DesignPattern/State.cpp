#include <stdio.h>
#include <unistd.h>

class TCPOctetStream;
class TCPState;

class TCPConnetion
{
public:
    TCPConnetion();
    virtual ~TCPConnetion();

    void ActiveOpen();
    void PassiveOpen();
    void Close();
    void Send();
    void Acknowledge();
    void Synchronize();
    void ProcessOctet(TCPOctetStream* );

private:
    friend class TCPState;
    void ChangeState(TCPState*);

private:
    TCPState* m_state;
};


class TCPState
{
public:
    virtual void Transmit(TCPConnetion*,TCPOctetStream*){ }
    virtual void ActiveOpen(TCPConnetion* ){    }
    virtual void PassiveOpen(TCPConnetion* ){   }

    virtual void Close(TCPConnetion*){    }
    virtual void Synchronize(TCPConnetion* ){    }
    virtual void Acknowledge(TCPConnetion* ){    }

    virtual void Send(TCPConnetion*){    }

protected:
    void ChangeState(TCPConnetion*, TCPState*);
};

class TCPEstablished : public TCPState
{
public:
    virtual ~TCPEstablished() { }
    static TCPState* Instance()
    {
        if(m_state == NULL)
        {
            m_state = new TCPEstablished;
        }
        return m_state;
    }

    virtual void Transmit(TCPConnetion* , TCPOctetStream*);
    virtual void Close(TCPConnetion* );

private:
    static TCPEstablished* m_state;
};
TCPEstablished* TCPEstablished::m_state = NULL;

class TCPListen: public TCPState
{
public:
    virtual ~TCPListen(){ }
    static TCPState* Instance(){
        if(m_state == NULL)
        {
            m_state = new TCPListen;
        }
        return m_state;
    }

    virtual void Send(TCPConnetion* );
private:
    static TCPListen* m_state;
};

TCPListen* TCPListen::m_state = NULL;

class TCPClosed:public TCPState
{
public:
    virtual ~TCPClosed() {    }
    static TCPState* Instance(){
        if(m_state == NULL){
            m_state = new TCPClosed;
        }
        return m_state;
    }

    virtual void ActiveOpen(TCPConnetion*);
    virtual void PassiveOpen(TCPConnetion* );

private:
    static TCPClosed* m_state;
};

TCPClosed* TCPClosed::m_state = NULL;

void TCPClosed::ActiveOpen(TCPConnetion* t)
{
    ChangeState(t, TCPEstablished::Instance());
}

void TCPClosed::PassiveOpen(TCPConnetion* t)
{
    ChangeState(t, TCPListen::Instance());
}

void TCPEstablished::Close(TCPConnetion* t)
{
    ChangeState(t, TCPListen::Instance());
}

void TCPEstablished::Transmit(TCPConnetion* t, TCPOctetStream* o)
{
    t->ProcessOctet(o);
}

void TCPListen::Send(TCPConnetion* t)
{
    ChangeState(t, TCPEstablished::Instance());
}

TCPConnetion::TCPConnetion()
{
    m_state = TCPClosed::Instance();
//    m_state->ChangeState(TCPClosed::Instance());
}

void TCPConnetion::ChangeState(TCPState*s)
{
    m_state = s;
}

void TCPConnetion::ActiveOpen()
{
    m_state->ActiveOpen(this);
}

void TCPConnetion::PassiveOpen()
{
    m_state->PassiveOpen(this);
}

void TCPConnetion::Close()
{
    m_state->Close(this);
}

void TCPConnetion::Send()
{
    m_state->Send(this);
}

void TCPConnetion::Acknowledge()
{
    m_state->Acknowledge(this);
}

void TCPConnetion::ProcessOctet(TCPOctetStream*)
{
    printf("ProcessOctet\n");
}

void TCPConnetion::Synchronize()
{
    m_state->Synchronize(this);
}


void TCPState::ChangeState(TCPConnetion* t, TCPState* s)
{
    if(s == TCPEstablished::Instance())
    {
        printf("TCPEStablished\n");
    }
    else if(s == TCPListen::Instance())
    {
        printf("TCPListen\n");
    }
    else if(s == TCPClosed::Instance()){
        printf("TCPClose\n");
    }
    t->ChangeState(s);
}

int main(int argc, char *argv[])
{
    TCPConnetion tcpcon;
    tcpcon.PassiveOpen();
    tcpcon.Send();
    tcpcon.Close();
    return 0;
}
