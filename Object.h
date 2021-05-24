#ifndef OBJECT_H
#define OBJECT_H

#include "Base.h"
#include "Timer.h"

const short ASSET_TRAMPOLINE = 13;

class CObj : public CBase
{
private:
    short m_theme;

    float m_jump;
    bool m_coliding;
    CTimer m_timer;
public:
    inline void  SetTheme(short value)
    {
        m_theme = value;
    }
    void GetRect(SDL_Rect *rect);
    inline void SetColiding(bool value)
    {
        m_coliding = value;
    }
    inline bool GetColiding()
    {
        return m_coliding;
    }
    inline float GetJump()
    {
        return m_jump;
    }
    inline void SetJump(float value)
    {
        m_jump = value;
    }
};

#endif
