#include "../apue.h"

#include <stdio.h>
#include <map>

class Wall{

};

class Room;
class Door{
public:
    Door(Room* r1, Room* r2){

    }
};

enum eDirection{
    eNorth,
    eSouth,
    eWest,
    eEast,
    eDirNum,
};


class Room{
public:
    Room(const int n){
        m_iRoom = n;
    }
    virtual ~Room(){

    }
    const int  GetRoomNo(){
        return m_iRoom;
    }

    void SetSide(eDirection dir, Wall wall)  {
        if(dir >= eDirNum)
            return ;
        m_wall[dir] = wall;
    }

    void SetSide(eDirection dir, Door* door){
        if(dir >= eDirNum)
            return;
        m_door[dir] = door;
    }
private:
    int m_iRoom;
    Wall m_wall[eDirNum];
    Door* m_door[eDirNum];
};

class Maze
{
public:
    Maze(){
        printf("Create maze");
        m_mpNoRoom.clear();
    }
    virtual ~Maze(){
        for(MpNoRoom::iterator it = m_mpNoRoom.begin(); it != m_mpNoRoom.end(); it++){
//            Room*& pRoom = (it->second);
            if(it->second != NULL)
            {
                delete it->second;
                it->second = NULL;
            }
        }
    }

    virtual bool AddRoom(const int n){
        Room * pRoom = GetRoom(n);
        if(pRoom != NULL)
            return false;
        else
        {
            Room* room = new Room(n);
            if(room == NULL)
            {
                printf("create room error\n");
            }
            m_mpNoRoom[n] = room;
        }

    }

    virtual bool DelRoom(const int n){
        Room* pRoom = GetRoom(n);
        if(pRoom == NULL)
            return true;
        delete pRoom;
        m_mpNoRoom.erase(n);
    }

    virtual bool HasRoom(const int n){
        return GetRoom(n) != NULL;
    }

    virtual Room* GetRoom(const int n){
//        return false;
        //    m_noRoom.find(n) !=
        MpNoRoom::iterator it = m_mpNoRoom.find(n);
        if(it != m_mpNoRoom.end()){
            return &*(it->second);
        }
        else
            return NULL;
    }


private:
    typedef std::map<int ,Room*> MpNoRoom;
    MpNoRoom m_mpNoRoom;
};

class MazeBuilder
{
public:
    MazeBuilder(){
    }
    virtual ~MazeBuilder(){
    }

    virtual void BuildMaze(){
    }

    virtual void BuildRoom(int room)=0;

    virtual void BuildDoor(int roomfrom, int roomto)=0;

    virtual Maze* GetMaze()=0;
};


class MazeGame
{
public:
    MazeGame(){

    }
    virtual ~MazeGame(){

    }
    Maze* CreateMaze(MazeBuilder& builder);

    Maze* CreateComplexMaze(MazeBuilder& builder);
};



Maze* MazeGame::CreateMaze(MazeBuilder& builder){
    builder.BuildMaze();
    builder.BuildRoom(1);
    builder.BuildRoom(2);
    builder.BuildDoor(1,2);
    return builder.GetMaze();
}

Maze* MazeGame::CreateComplexMaze(MazeBuilder& builder){

    for(int i=0; i< 1000; i++){
        builder.BuildRoom(i);
    }
    return builder.GetMaze();
}

class StandardMazeBuilder : public MazeBuilder
{
public:
    StandardMazeBuilder(): m_pMaze(NULL){

    }
    virtual ~StandardMazeBuilder() {
        if(m_pMaze != NULL)
        {
            delete m_pMaze;
            m_pMaze = NULL;
        }
    }

    virtual void BuildMaze();
    virtual void BuildRoom(int);
    virtual void BuildDoor(int,int);

    virtual Maze* GetMaze(){
        return m_pMaze;
    }
private:
    eDirection CommonWall(Room*, Room*);
    Maze* m_pMaze;
};

eDirection StandardMazeBuilder::CommonWall(Room* r1, Room*r2){
    return eDirNum;
}

void StandardMazeBuilder::BuildMaze(){
    if(m_pMaze != NULL)
    {
        delete m_pMaze;
        m_pMaze = NULL;
    }
    m_pMaze = new Maze;
}

void StandardMazeBuilder::BuildRoom(int n){
    if(m_pMaze == NULL)
        return ;

    if(!m_pMaze->HasRoom(n)){
        m_pMaze->AddRoom(n);
        Room* room = m_pMaze->GetRoom(n);
        room->SetSide(eNorth, Wall());
        room->SetSide(eSouth, Wall());
        room->SetSide(eWest, Wall());
        room->SetSide(eEast, Wall());
    }
}

void StandardMazeBuilder::BuildDoor(int n1, int n2){
    if(m_pMaze == NULL)
        return;
    Room* r1 = m_pMaze->GetRoom(n1);
    Room* r2 = m_pMaze->GetRoom(n2);

    Door* d = new Door(r1, r2);
    r1->SetSide(CommonWall(r1,r2), d);
    r2->SetSide(CommonWall(r2,r1), d);
}

void dump(int signo);

int main(int argc, char *argv[])
{
    signal(SIGABRT, &dump);
    Maze* maze;
    MazeGame game;
    StandardMazeBuilder builder;

    game.CreateMaze(builder);
    maze = builder.GetMaze();
    return 0;
}




void
dump(int signo)
{
    char  buf[1024];
    char  cmd[1024];
    FILE *fh;
    snprintf(buf, sizeof(buf), "/proc/%d/cmdline",getpid());
    if(!(fh = fopen(buf, "r")))
        exit(0);
    if(!fgets(buf, sizeof(buf), fh))
        exit(0);
    fclose(fh);
    if(buf[strlen(buf)-1]  == '\n')
        buf[strlen(buf)-1]  = '\0';
    snprintf(cmd, sizeof(cmd), "gdb %s %d",buf, getpid());
    system(cmd);
    exit(0);
}

