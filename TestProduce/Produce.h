#ifndef PRODUCE_H
#define PRODUCE_H

#include <vector>
//#include <queue>

class cTimer
{
public:
    void OnTimer(){

    }
    void StartTimer(){

    }
    void CancelTimer(){

    }
};


static const int TimePassed = 100;

class cNpcObj
{
public:
    cObj(){

    }
    virtual ~cObj(){

    }

public:
    int m_typeId;
    int m_num;
    int m_mTime;
//    int m_total;//
};


class FinishObjListener()
{
public:

};

class MultiProduceFactory:public cTimer
{
public:
    ProduceFactory(){

    }

    virtual ~ProduceFactory(){

    }

    virtual void EnterGame()
    {
        //

    }

    void StartTimer()
    {
        m_startTimer ;

    }

    void OnTimer()
    {
        if(m_vecObj.empty()){
            CancelTimer();
            return;
        }
        if(m_totalunit == max)
        {
            CancelTimer();
            return;
        }
        else
        {
            //judge the number of the first element
//            ifm_vecObj[0].m_num == 1

            cObj obj = m_vecObj[0];
            DelObj(obj);
            //67 panduan
            //get the netxt timer
            //new sechd
        }
    }


    void AddObj(cNpcObj obj)
    {
        if(m_totalunit == max)
            return;
        for(int i =0; i< m_vecObj.size(); i++)
        {
            if(m_vecObj[i].m_typeId == obj.m_typeId)
            {
                m_vecObj[i].m_num++;
            }
            else
            {
                m_vecObj[i].push(obj);
            }
        }

    }


    void DelObj(cNpcObj obj)
    {
        std::vector<cObj>::iterator it;
        for(it = m_vecObj.begin(); it!=m_vecObj.end(); it++)
        {
            if(it->m_typeId == obj.m_typeId)
            {
                if(it->m_num <=1)
                {
                    m_vecObj.erase(it);
                }
                else
                {
                    it->m_num--;
                }

            }

        }
    }




private:
    std::vector<cNpcObj> m_vecObj;
    int m_startTime;
    int m_totalunit;
};

//resouse


class SingleProduceFactory: public cTimer
{
public:

private:

};

#endif
