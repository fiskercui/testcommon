#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <map>

class VariableExp;
class Context
{
public:
    bool Lookup(const char* ) const ;
    void Assign(VariableExp*, bool);
private:
    std::map<const char*, bool> mpNameRet;
};

class BooleanExp
{
public:
    BooleanExp(){ }
    virtual ~BooleanExp() {}

    virtual bool Evaluate(Context& ) = 0;
    virtual BooleanExp* Replace(const char*, BooleanExp&) = 0;
    virtual BooleanExp* Copy() const = 0;
};

class Constant:public BooleanExp
{
public:
    Constant(bool b): m_b(b){ }

    virtual ~Constant() {    }

    virtual bool Evaluate(Context& ){
        return m_b;
    }
    virtual BooleanExp* Replace(const char*, BooleanExp&){
        return NULL;
    }
    virtual BooleanExp* Copy() const{
        return NULL;
    }

private:
    bool m_b;
};


class VariableExp: public BooleanExp
{
public:
    VariableExp(const char*);
    virtual ~VariableExp() {
    }

    virtual bool Evaluate(Context& );
    virtual BooleanExp* Replace(const char*, BooleanExp&);
    virtual BooleanExp* Copy() const;

    const char* GetName(){
        return m_name;
    }

private:
    char* m_name ;
};

//bool Context::Lookup(const char* ) const ;

VariableExp::VariableExp(const char* szName)
{
    m_name = strdup(szName);
}

bool VariableExp::Evaluate(Context& context)
{
    return context.Lookup(m_name);
}

BooleanExp* VariableExp::Replace(const char* name , BooleanExp& exp)
{
//    return aCommand;
    if(strcmp(name, m_name) == 0 )
    {
        return exp.Copy();
    }
    else{
        return new VariableExp(name);
    }
}


BooleanExp* VariableExp::Copy() const
{
    return new VariableExp(m_name);
}


bool Context::Lookup(const char* name)const
{
    if(name == NULL)
        return false;
    std::map<const char*, bool>::const_iterator it = mpNameRet.find(name);
    if(mpNameRet.find(name) == mpNameRet.end()){
        printf("not found \n");
        return false;
    }
    return it->second;
}

void Context::Assign(VariableExp* exp, bool b)
{
    if(exp->GetName() == NULL)
        return ;
    mpNameRet[exp->GetName()] = b;
}

class AndExp : public BooleanExp
{
public:
    AndExp(BooleanExp* , BooleanExp* );
    virtual ~AndExp(){
    }

    virtual bool Evaluate(Context& );
    virtual BooleanExp* Replace(const char*, BooleanExp&);
    virtual BooleanExp* Copy() const;

private:
    BooleanExp* m_operand1;
    BooleanExp* m_operand2;
};

AndExp::AndExp(BooleanExp* exp1, BooleanExp* exp2): m_operand1(exp1), m_operand2(exp2)
{
}

bool AndExp::Evaluate(Context& text)
{
    return m_operand1->Evaluate(text)
        && m_operand2->Evaluate(text);
}

BooleanExp* AndExp::Replace(const char* name , BooleanExp& exp)
{
    return new AndExp(m_operand1->Replace(name, exp), m_operand2->Replace(name,exp));
}

BooleanExp* AndExp::Copy() const
{
    return new AndExp(m_operand1->Copy(), m_operand2->Copy());
}

class OrExp: public BooleanExp
{
public:
    OrExp(BooleanExp* , BooleanExp* );
    virtual ~OrExp(){
    }

    virtual bool Evaluate(Context& );
    virtual BooleanExp* Replace(const char*, BooleanExp&);
    virtual BooleanExp* Copy() const;

private:
    BooleanExp* m_operor1;
    BooleanExp* m_operor2;
};

OrExp::OrExp(BooleanExp* exp1, BooleanExp* exp2): m_operor1(exp1), m_operor2(exp2)
{

}
bool OrExp::Evaluate(Context& text)
{
    return m_operor1->Evaluate(text)
        ||  m_operor2->Evaluate(text);
}

BooleanExp* OrExp::Replace(const char* name , BooleanExp& exp)
{
    return new OrExp(m_operor1->Replace(name, exp), m_operor2->Replace(name,exp));
}

BooleanExp* OrExp::Copy() const
{
    return new OrExp(m_operor1->Copy(), m_operor2->Copy());
}


// (true and x ) or (y and (not x ))

//ontly cimpile design

int main(int argc, char *argv[])
{
    Context context;

    VariableExp* x = new VariableExp("X");
    VariableExp* y = new VariableExp("Y");

    BooleanExp* expression = new OrExp(new AndExp(new Constant(true), x), new AndExp(new Constant(true), y));

    context.Assign(x, false);
    context.Assign(y, false);

    bool result = expression->Evaluate(context);

    printf("express result is : %d\n",result);
    printf("real  result is : %d\n", (true && false)||(false && true));
    return 0;
}


