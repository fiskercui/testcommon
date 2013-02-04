#include "../apue.h"
#include <stdio.h>

class Spell;
class Room;
class Maze
{
public:

    Maze(){

    }
    virtual ~Maze(){

    }
    void AddRoom(Room* r1){

    }
};

class Wall
{
public:
    Wall(){
        printf("Create Wall");
    }
    virtual ~Wall(){

    }
};

class Room
{
public:
    Room(const int n){
        printf("Create Room");
    }
    virtual ~Room(){

    }
};

class Door
{
public:
    Door(Room* r1, Room* r2){
        printf("Creat Door");
    }
    virtual ~Door(){

    }
};

class MazeFactory
{
public:
    MazeFactory(){

    }
    virtual ~MazeFactory(){

    }
    virtual Maze* MakeMaze() const{
        return new Maze;
    }
    virtual Wall* MakeWall() const {
        return new Wall;
    }
    virtual Room* MakeRoom(const int n) const{
        return new Room(n);
    }
    virtual Door* MakeDoor(Room* r1, Room* r2) const {
        return new Door(r1, r2);
    }

};



class Spell
{
public:
    Spell(){

    }
    virtual ~Spell(){

    }
};



//child room/
class EnchantedRoom:public Room
{
public:
    EnchantedRoom(const int n,const Spell*): Room::Room(n){
        printf("Enchanted Room");
    }
};

class BombedRoom:public Room
{
public:
    BombedRoom(const int n):Room(n){
        printf("Bombed Room");
    }
};


class DoorNeedingSpell: public Door
{
public:
    DoorNeedingSpell(Room* r1, Room* r2):Door(r1,r2){

    }
    virtual ~DoorNeedingSpell(){

    }
};

class BombedWall:public Wall{
public:
    BombedWall(){

    }
};

class EnchantedMazeFactory:public MazeFactory
{
public:
    EnchantedMazeFactory(){

    }
    virtual ~EnchantedMazeFactory(){

    }


    virtual Room* MakeRoom(int n) const {
        return new EnchantedRoom(n, CastSpell());
    }

    virtual Door* MakeDoor(Room* r1, Room* r2) const {
        return new DoorNeedingSpell(r1,r2);
    }
protected:
    Spell* CastSpell() const {
    }
};

class BombedMazeFactory
{
public:
    BombedMazeFactory(){

    }
    virtual ~BombedMazeFactory(){

    }

    virtual Wall* MakeWall() const {
        return new  BombedWall;
    }

    virtual Room* MakeRoom(int n) const {
        return new BombedRoom(n);
    }
};


class MazeGame
{
public:
    MazeGame(){

    }

    Maze* CreateMaze(MazeFactory& factory)
    {
        Maze* pMaze = factory.MakeMaze();
        Room* r1 = factory.MakeRoom(1);
        Room* r2 = factory.MakeRoom(2);
        Door* aDoor = factory.MakeDoor(r1,r2);
        pMaze->AddRoom(r1);
        pMaze->AddRoom(r2);
        retrun pMaze;
    }
    virtual ~MazeGame(){

    }
};

int main(int argc, char *argv[])
{
    //Maze is a maze object
    //Maze is maze object builder
    //
    Maze* pMaze;
    MazeGame game;
    EnchantedMazeFactory factory;

    pMaze = game.CreateMaze(factory);

    return 0;
}
