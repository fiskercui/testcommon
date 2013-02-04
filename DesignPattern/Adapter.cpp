#include <stdio.h>
#include <unistd.h>


typedef int Coord;

class Point
{
public:
    Point():x(0), y(0){

    }
    Point(const Coord& cx, const Coord& cy): x(cx), y(cy){

    }
private:
    Coord x;
    Coord y;
};


class Manipulator
{

};

class Shape
{
public:
    Shape(){

    }
    virtual void BoundingBox(Point& bottemLeft, Point& topRight) const{

    }
    virtual Manipulator* CreateManipulator() const{

    }

};

class TextView {
public:
    TextView(){

    }
    void GetOrigin(Coord&x, Coord& y) const{

    }
    void GetExtent(Coord& width, Coord& height) const{

    }
    virtual bool IsEmpty() const{

    }
};


class TextManipulator: public Manipulator
{
public:
    TextManipulator(){

    }
    TextManipulator(const Shape*){

    }
    virtual ~TextManipulator(){

    }
};

class TextShape: public Shape, private TextView
{
public:
    TextShape(){

    }

    virtual void BoundingBox( Point& bottemLeft, Point& topRight) const;
    virtual bool IsEmpty() const;

    virtual Manipulator* CreateManipulator() const;
};


void TextShape::BoundingBox(Point& bottemLeft, Point& topRight) const
{
    Coord bottem , left , width , height;
    GetOrigin(bottem, left);
    GetExtent(width, height);
    bottemLeft = Point(bottem , left);
    topRight = Point(bottem+height, left+width);
}
bool TextShape::IsEmpty() const {
    return TextView::IsEmpty();
}

Manipulator* TextShape::CreateManipulator() const{
    return new TextManipulator(this);
};


//use composite
class TextShape2: public Shape{
public:
    TextShape2(TextView* p ): m_pText(p){

    }

    virtual void BoundingBox(Point& bottemLeft, Point& topRight) const;
    virtual bool IsEmpty() const;
    virtual Manipulator* CreateManipulator() const;

private:
    TextView* m_pText;
};

void TextShape2::BoundingBox(Point& bottemLeft, Point& topRight) const
{
    Coord bottem , left , width , height;
    m_pText->GetOrigin(bottem, left);
    m_pText->GetExtent(width, height);
    bottemLeft = Point(bottem , left);
    topRight = Point(bottem+height, left+width);
}

bool TextShape2::IsEmpty() const
{
    return m_pText->IsEmpty();
}

Manipulator* TextShape2::CreateManipulator() const
{
    return new TextManipulator(this);
}


int main(int argc, char *argv[])
{
    Shape* shape = new TextShape();


    return 0;
}
