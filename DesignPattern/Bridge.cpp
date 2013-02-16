#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

typedef int Coord;
class Point
{
public:
    Point(): m_x(0),m_y(0){

    }
    Point(const Coord& x, const Coord& y): m_x(x), m_y(y){

    }
    Point(const Point& pt){
        m_x = pt.m_x;
        m_y = pt.m_y;
    }
    const Coord X() const {
        return m_x;
    }
    const Coord Y() const {
        return m_y;
    }
private:
    Coord m_x;
    Coord m_y;
};


class WindowImp;
class View;

class Window
{
public:
    Window(){

    }
    virtual ~Window(){}

    virtual void DrawContents(){    }

    virtual void Open(){}
    virtual void Close(){}
    virtual void Iconify(){}
    virtual void Deiconify(){}

    virtual void SetOrgin(const Point& pt){}
    virtual void SetExtent(const Point& extent){}

    virtual void Raise(){ }
    virtual void Lower(){  }


    virtual void DrawLine(const Point& , const Point&){}
    virtual void DrawRect(const Point& , const Point&);
    virtual void DrawPolygon(const Point[],int n){}
    virtual void DrawText(const char*, const Point&){}
    void SetWindowImp(WindowImp* pImp){
        m_imp = pImp;
    }
protected:
    WindowImp* GetWindowImp() {
        return m_imp;
    }
    View* GetView(){
        return m_contents;
    }
private:
    WindowImp* m_imp;
    View* m_contents;
};


class WindowImp
{
public:
//    virtual void ImpTop() = 0;
//    virtual void ImpBottem() = 0;
//    virtual void ImpSetExtent(const Point&) = 0;
//    virtual void ImpSetOrgin(const Point&) = 0;

    virtual void DeviceRect(const Coord, const Coord, const Coord, const Coord) = 0;
    virtual void DeviceText(const char* ,const Coord, const Coord) = 0;
    virtual void DeviceBitmap(const char* ,const Coord, const Coord) = 0;
protected:
    WindowImp(){
    }
    virtual ~WindowImp(){
    }
};


class ApplicationWindow: public Window
{
public:
    virtual void DrawContents(){
        std::cout << "ApplicationWindow DrawContents" << std::endl;
        Window::DrawRect(Point(0,0), Point(1,1));
    }
public:
    ApplicationWindow(){

    }
    virtual ~ApplicationWindow(){

    }
};


class IconWindow: public Window
{
public:
    IconWindow(){
    }
    virtual ~IconWindow(){
    }
    virtual void DrawContents(){
        std::cout << "IconWindow DrawContents" << std::endl;
        WindowImp* imp = GetWindowImp();
        if(imp != NULL){
            imp->DeviceBitmap(m_name, 0,0);
        }
    }
private:
    const char* m_name;
};

void Window::DrawRect(const Point& pt1, const Point& pt2)
{
    std::cout << "Window DrawRect" << std::endl;
    WindowImp* imp =GetWindowImp();
    if(imp != NULL){
        //   imp->DeviceRect(0,0,0,0);
        imp->DeviceRect(pt1.X(), pt1.Y(), pt2.X(), pt2.Y());
    }
}


class XWindowImp : public WindowImp
{
public:
    XWindowImp(){
    }
    virtual ~XWindowImp(){
    }

    virtual void DeviceRect(Coord, Coord,Coord, Coord){
        std::cout << "XWindowImp DeviceRect" << std::endl;
    }
    virtual void DeviceText(const char* ,const Coord, const Coord){
        std::cout << "XWindowImp DeviceText" << std::endl;
    }
    virtual void DeviceBitmap(const char* ,const Coord, const Coord){
        std::cout << "XWindowImp DeviceBitmap" << std::endl;
    }

private:

};


class PmWindowImp: public WindowImp
{
public:
    PmWindowImp(){
    }
    virtual ~PmWindowImp(){
    }
    virtual void DeviceRect(Coord, Coord,Coord, Coord){
        std::cout << "PmWindowImp DeviceRect" << std::endl;
    }
    virtual void DeviceText(const char* ,const Coord, const Coord){
        std::cout << "PmWindowImp DeviceText" << std::endl;
    }
    virtual void DeviceBitmap(const char* ,const Coord, const Coord){
        std::cout << "PmWindowImp DeviceBitmap" << std::endl;
    }

};
int main(int argc, char *argv[])
{
    Window* pAppWindow = new ApplicationWindow;
    WindowImp* pXImp = new XWindowImp;
    pAppWindow->SetWindowImp(pXImp);
    pAppWindow->DrawContents();

    WindowImp* pPmImp = new PmWindowImp;
    pAppWindow->SetWindowImp(pPmImp);
    pAppWindow->DrawContents();

    return 0;
}

