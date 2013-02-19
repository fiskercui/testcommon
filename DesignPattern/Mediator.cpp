#include <stdio.h>
#include <unistd.h>

class Widget;

class DialogDirector
{
public:
    virtual ~DialogDirector(){

    }

    virtual void ShowDialog() {

    }
    virtual void WidgetChanged(Widget* ) =0;

protected:
    DialogDirector(){
        printf("DialogDirector\n");
    }
    virtual void CreateWidgets() = 0;
};

class Widget
{
public:
    Widget(DialogDirector* p): m_director(p){
    }

    virtual void Changed();
    virtual void HandleMouse();

private:
    DialogDirector* m_director;
};


void Widget::Changed()
{
    m_director->WidgetChanged(this);
}

void Widget::HandleMouse()
{
    printf("Widget Handle Mouse\n");
}


class ListBox: public Widget
{
public:
    ListBox(DialogDirector* pDD): Widget(pDD){

    }

    virtual void SetText(const char* text);
    virtual void HandleMouse();
private:
//    DialogDirector* m_director;
};


void ListBox::SetText(const char* text)
{
    printf("ListBox SetText\n");
}

void ListBox::HandleMouse()
{
    Changed();
}


class EntryField : public Widget
{
public:
    EntryField(DialogDirector* pDD): Widget(pDD){

    }

    virtual ~EntryField(){
    }

    virtual void SetText(const char* text){
        printf("EnteryFiled SetText\n");
    }

    virtual const char* GetText(){
        return "";
    }

    virtual void HandleMouse(){
        printf("EntryField HandleMouse\n");
    }
};

class Button:public Widget
{
public:
    Button(DialogDirector* dd): Widget(dd){

    }
    virtual~ Button() {
    }

    virtual void SetEnable(bool b)
    {
        printf("SetEnabled: %d\n", (int)b);
    }

    virtual void HandleMouse()
    {
        printf("Handle Mouse\n");
    }

private:
//    DialogDirector* m_director;
};

// mediator
// FrontDialogDirector is same as cBridge int the sanguo mmo server
class FontDialogDirector: public DialogDirector
{
public:
    FontDialogDirector(){
    }
    virtual ~FontDialogDirector(){
    }


    virtual void WidgetChanged(Widget* );

protected:
    virtual void CreateWidgets();

private:
    Button* m_OkBtn;
    Button* m_CancelBtn;
    EntryField* m_Entry;
    ListBox* m_ListBox;
};


void FontDialogDirector::CreateWidgets( )
{
    m_OkBtn = new Button(this);
    m_CancelBtn = new Button(this);
    m_ListBox = new ListBox(this);
    m_Entry = new EntryField(this);
}

void FontDialogDirector::WidgetChanged(Widget*  pdg)
{
    if(pdg == m_OkBtn)
    {

    }
    else if(pdg == m_CancelBtn)
    {

    }
    else if(pdg == m_ListBox)
    {

    }
}


int main(int argc, char *argv[])
{
    FontDialogDirector dig;

    return 0;
}
