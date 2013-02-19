#include <stdio.h>
#include <unistd.h>
#include <vector>

class Point;
class Graphic
{
public:
    Graphic(){
    }
    virtual ~Graphic(){
    }
    virtual void Draw() =0;
    virtual void Move(Point& pt) =0;
};


typedef int Coord;
class Point
{
public:
    Point&  operator-()
    {
        m_x = -m_x;
        m_y = -m_y;
        return *this;
    }
    Coord m_x;
    Coord m_y;
};


class ConstraintSolverMemento;

class MoveCommand
{
public:
    MoveCommand(Graphic* taget, const Point& point);

    void Excute();
    void Unexcute();
private:
    ConstraintSolverMemento* m_state;
    Point m_delta;
    Graphic* m_target;
};



class ConstraintSolver
{
public:
    static ConstraintSolver* Instance();

    void Solve();
    void AddConstraint(Graphic* start, Graphic* end);
    void RemoveConstraint(Graphic* start, Graphic* end);

    ConstraintSolverMemento* CreateMemento();
    void SetMemento(ConstraintSolverMemento* memento);

private:
    static ConstraintSolver* m_instance;
};

ConstraintSolver* ConstraintSolver::m_instance = NULL;

ConstraintSolver* ConstraintSolver::Instance()
{
    if(m_instance == NULL)
    {
        m_instance = new ConstraintSolver;
    }
    return m_instance;
}

void ConstraintSolver::Solve()
{

}

void ConstraintSolver::AddConstraint(Graphic* start, Graphic* end)
{

}

void ConstraintSolver::RemoveConstraint(Graphic* start, Graphic* end)
{

}

ConstraintSolverMemento* ConstraintSolver::CreateMemento()
{
    return NULL;
}

void SetMemento(ConstraintSolverMemento* memento)
{

}




class ConstraintSolverMemento
{
public:
    virtual ~ConstraintSolverMemento() {
    }
private:
    friend class ConstraintSolver;
    ConstraintSolverMemento();
};


void MoveCommand::Excute()
{
    ConstraintSolver* solver = ConstraintSolver::Instance();
    m_state = solver->CreateMemento();
    m_target->Move(m_delta);
    solver->Solve();
}

void MoveCommand::Unexcute()
{
    ConstraintSolver* solver = ConstraintSolver::Instance();
    m_target->Move(-m_delta);
    solver->SetMemento(m_state);
    solver->Solve();
}


//memento is not so important
//i don't know the real details
int main(int argc, char *argv[])
{

    return 0;
}
