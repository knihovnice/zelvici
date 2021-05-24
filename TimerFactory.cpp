#include "TimerFactory.h"

CTimerFactory::CTimerFactory()
{

}

CTimerFactory::~CTimerFactory()
{
    Release();
}

CTimerFactory *CTimerFactory::GetInstance()
{
    static CTimerFactory instance;
    return &instance;
}

CTimer *CTimerFactory::Get(int id)
{
    // look up the timer
    map<int, CTimer*>::iterator result = m_timers.find(id);
    if(result == m_timers.end())
    {
        // not there, create new one
        CTimer *timer = new CTimer();
        timer->SetEnabled(false);

        m_timers.insert(pair<int, CTimer*>(id, timer));

        return timer;
    }

    return result->second;
}

void CTimerFactory::Release()
{
    printf("CTimerFactory: Releasing timers..");
    map<int, CTimer*>::iterator it;
    for(it = m_timers.begin(); it != m_timers.end(); ++it)
    {
        CTimer *timer = it->second;
        timer->SetEnabled(false);
        delete timer;
    }

    m_timers.clear();
    printf("OK.\n");
}
