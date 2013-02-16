#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>

class Graphic
{
public:
    virtual ~Graphic(){    }

    virtual void Draw() = 0;

    virtual void Add(Graphic* ) {

    }

    virtual void Remove(Graphic* ){

    }
};


class Line:public Graphic
{
public:
    void Draw(){
        std::cout << "LINE" << std::endl;
    }
};


class Rectangle: public Graphic
{
public:
    void Draw(){
        std::cout << "Rectangle" << std::endl;
    }

};


class Text: public Graphic
{
public:
    void Draw(){
        std::cout << "Text" << std::endl;
    }
};


class Picture: public Graphic
{
public:
    void Draw(){
        std::cout << "Draw Picture" << std::endl;
        std::vector<Graphic*>::iterator it;
        for(it = m_vecGraph.begin(); it!= m_vecGraph.end(); it++){
            (*it)->Draw();
        }
    }

    void Add(Graphic* p){
        m_vecGraph.push_back(p);
    }
protected:
    std::vector<Graphic* > m_vecGraph;
};


int main()
{
    Picture* pPicture = new Picture;
    Line* Line1 = new Line;
    Rectangle* Rect1 = new Rectangle;
    pPicture->Add(Line1);
    pPicture->Add(Rect1);

    Picture* pFPicture = new Picture;
    pFPicture->Add(pPicture);
    Line* Line2 = new Line;
    Rectangle* Rect2 = new Rectangle;
    pFPicture->Add(Line2);
    pFPicture->Add(Rect2);

    pFPicture->Draw();

    return 0;
}
