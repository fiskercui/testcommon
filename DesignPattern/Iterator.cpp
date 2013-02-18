#include <stdio.h>
#include <unistd.h>


#define DEFAULT_LIST_CAPACITY 32


template <class Item>
class List
{
public:
    List(long size = DEFAULT_LIST_CAPACITY);
    long Count() const ;
    Item& Get(long index) const ;
};


template <typename Item>
class Iterator
{
public:

    virtual void First() =0;
    virtual void Next() = 0;
    virtual bool End() = 0;
    virtual Item CurItem() const = 0;

protected:
    Iterator(){    }
};
template <typename Item>
class ListIterator: public Iterator<Item>
{
public:
    ListIterator(const List<Item>* aList);

    virtual void First() ;
    virtual void Next() ;
    virtual bool End() ;
    virtual Item CurItem() const;

private:
    List<Item>* m_aList;
    long m_current;
};

template <typename Item>
ListIterator<Item>::ListIterator(const List<Item>* aList): m_aList(aList), m_current(0)
{
}

template <typename Item>
void ListIterator<Item>::First(){
    m_current = 0;
}

template <typename Item>
void ListIterator<Item>::Next()
{
    m_current++;
}

template <typename Item>
bool ListIterator<Item>::End()
{
    return m_current >= m_aList->Count();
}

template <typename Item>
Item ListIterator<Item>::CurItem() const
{
    if(End())
        throw "Out";
    return m_aList->Get(m_current);
}



//internal iterator
template <typename Item>
class ListTraveller
{
public:
    ListTraveller(List<Item>* aList);
    virtual ~ListTraveller();

    bool Traverse();

protected:
    virtual bool ProcessItem(const Item& ) = 0;
private:
    ListIterator<Item> m_iterator;
};

template <typename Item>
ListTraveller<Item>::ListTraveller(List<Item>* aList)
{

}
