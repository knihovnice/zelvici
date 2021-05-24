#ifndef TIMER_FACTORY_H
#define TIMER_FACTORY_H

#include "Timer.h"
#include <map>

using namespace std;

class CTimerFactory
{
private:
    map<int, CTimer*> m_timers;

private:
    CTimerFactory();
public:
    ~CTimerFactory();

    void Release();
    CTimer *Get(int id);
    static CTimerFactory *GetInstance();
};


#endif
