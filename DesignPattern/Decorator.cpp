#include <stdio.h>
#include <iostream>
#include <unistd.h>


class VisualComponent{
public:
    virtual ~VisualComponent(){

    }

    virtual void Draw()
    {
        std::cout << "VisualComponent Draw" << std::endl;
    }
    virtual void Resize(){
        std::cout << "VisualComponent Draw" << std::endl;
    }
};


class Decorator: public VisualComponent
{
public:
    Decorator(VisualComponent* pVc): m_component(pVc){

    }

    virtual void Draw(){
        m_component->Draw();
    }
    virtual void Resize(){
        m_component->Resize();
    }
private:
    VisualComponent* m_component;
};

class BorderDecorator : public Decorator
{
public:
    BorderDecorator(VisualComponent* vc, int borderWidth): Decorator(vc), width(borderWidth){
    }

    virtual void Draw(){
        Decorator::Draw();
        DrawBorder(10);
    }
private:
    void DrawBorder(int){
        std::cout << "Draw Border" << std::endl;
    }
private:
    int width;
};


class Window{
public:
    void SetComponent(VisualComponent* vc){
        m_component = vc;
    }
private:
    VisualComponent* m_component;
};

class TextView: public VisualComponent
{
public:
    virtual void Draw(){
        std::cout << "TextView Draw" << std::endl;
    }
    virtual void Resize(){
        std::cout << "TextView Resize" << std::endl;
    }
};
int main(int argc, char *argv[])
{

    Window* window = new Window;

    TextView* pTx = new TextView;
    window->SetComponent(new BorderDecorator(pTx, 10));

    return 0;
}
