#include <stdio.h>



class Maze
{
public:
    Maze* Clone(){
        return new Maze(*this);
    }
    virtual ~Maze(){

    }
};
class Wall
{
public:
    Wall* Clone(){
        return new Wall(*this);
    }
    virtual ~Wall(){

    }
};
class Room
{
public:
    Room(const int n){

    }
    Room(const Room& r)
    {

    }
    Room* Clone(){
        return new Room(*this);
    }
    virtual ~Room(){

    }
};

class Door
{
public:
    Door(Room* r1, Room* r2){

    }
    Door(const Door& door){//use deep copy?
        m_r1 = door.m_r1;
        m_r2 = door.m_r2;
    }
    Door* Clone(){
        return new Door(*this);
    }

    virtual void Initialize(Room* r1,Room*  r2){
        m_r1 = r1;
        m_r2 = r2;
    }
    virtual ~Door(){

    }
private:
    Room* m_r1;
    Room* m_r2;
};

class MazeFactory
{
public:
    MazeFactory(){

    }
    MazeFactory(Maze* maze,Wall* wall, Room* room , Door* door){

    }

    virtual Maze* MakeMaze() const =0;
    virtual Room* MakeRoom() const =0;
    virtual Wall* MakeWall() const = 0;
    virtual Door* MakeDoor(Room*, Room*) const = 0;

    virtual ~MazeFactory(){

    }
};

class MazePrototypeFactory: public MazeFactory
{
public:
    MazePrototypeFactory(Maze* maze,Wall* wall, Room* room, Door* door);
    virtual ~MazePrototypeFactory(){

    }
    virtual Maze* MakeMaze() const ;
    virtual Room* MakeRoom() const ;
    virtual Wall* MakeWall() const ;
    virtual Door* MakeDoor(Room*, Room*) const;


protected:
    Maze* m_pMaze;
    Room* m_pRoom;
    Wall* m_pWall;
    Door* m_pDoor;
};


MazePrototypeFactory::MazePrototypeFactory(Maze* maze,Wall* wall, Room* room, Door* door){
    m_pMaze = maze;
    m_pWall = wall;
    m_pRoom = room;
    m_pDoor = door;
}

Maze* MazePrototypeFactory::MakeMaze() const{
    return m_pMaze->Clone();
}

Room* MazePrototypeFactory::MakeRoom() const {
    return m_pRoom->Clone();
}


Wall* MazePrototypeFactory::MakeWall() const{
    return m_pWall->Clone();
}

Door* MazePrototypeFactory::MakeDoor(Room* r1, Room* r2) const{
    Door* door = m_pDoor->Clone();
    door->Initialize(r1, r2);
    return door;
}


class MazeGame
{
public:
    MazeGame(){

    }

    Maze* CreateMaze(MazeFactory& factory){
//        Maze* maze = factory.MakeMaze();
       Maze* maze = factory.MakeMaze();
//        return Maze;
       Room* r1 = factory.MakeRoom();
       Room* r2 = factory.MakeRoom();
       Door* door = factory.MakeDoor(r1,r2);
       return maze;
    }
    virtual ~MazeGame(){

    }
};

int main(int argc, char *argv[])
{
    MazeGame game;
    MazePrototypeFactory simpleMazeFactory(
        new Maze, new Wall, new Room(0), new Door(new Room(1),new Room(2))
        );
    Maze* pMaze = game.CreateMaze(simpleMazeFactory);
    return 0;
}
