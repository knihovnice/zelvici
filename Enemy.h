#ifndef ENEMY_H
#define ENEMY_H

#include "Base.h"
#include "Sound.h"
#include "Gravity.h"
#include "Zelvik.h"

const Uint16 ENM_FLAG_NORMAL = 100;
const Uint16 ENM_FLAG_NODESTRUCT = 300;
const Uint16 ENM_FLAG_HARMLESS = 200;

class CEnm : public CBase
{
private:

    short m_length;
    short m_currentLength;
    short m_destruct;
    short m_theme;

    float m_speed;
    short m_direction;
    bool m_dead;
    bool m_erase;
    bool m_canHurt;
    bool m_animate;
    bool m_colideNow;
    bool m_start;
    bool m_hurt;
    Uint16 m_flag;

    CGravity *m_gravity;
    CSound *m_sound;
public:
    CEnm();
    ~CEnm();
    void Walk(CZelvik *player);
    void Update();
    void GetRect(SDL_Rect *rect);
    inline void  SetSound(CSound *sound)
    {
        m_sound = sound;
    }

    inline void SetHurt(bool value)
    {
        m_hurt = value;
    }
    inline bool GetHurt()
    {
        return m_hurt;
    }
    void SetTheme(short value);

    inline void SetColiding(bool value)
    {
        m_colideNow = value;
    }
    inline bool GetColiding()
    {
        return m_colideNow;
    }
    inline void SetCanHurt(bool value)
    {
        m_canHurt = value;
    }
    inline bool GetCanHurt()
    {
        return m_canHurt;
    }
    inline void SetFlag(Uint16 value)
    {
        m_flag = value;
    }
    inline Uint16 GetFlag()
    {
        return m_flag;
    }
    inline void SetAnim(bool value)
    {
        m_animate = value;
    }
    inline bool GetAnim()
    {
        return m_animate;
    }

    inline void  SetDirection(short value)
    {
        m_direction = value;
    }
    inline short GetDirection()
    {
        return m_direction;
    }
    inline void  SetErase(bool value)
    {
        m_erase = value;
    }
    inline bool  GetErase()
    {
        return m_erase;
    }
    inline bool  GetDead()
    {
        return m_dead;
    }
    inline void  SetDead(bool value)
    {
        m_dead = value;
    }
    inline short GetLength()
    {
        return m_length;
    }
    inline void  SetLength(short value)
    {
        m_length = value;
        m_currentLength = value / 2;
    }
    inline short GetDestruct()
    {
        return m_destruct;
    }
    inline void  SetDestruct(short value)
    {
        m_destruct = value;
    }
    inline short DecrementDestruct()
    {
        return --m_destruct;
    }
    inline float GetSpeed()
    {
        return m_speed;
    }
    inline void  SetSpeed(float value)
    {
        m_speed = value;
    }
};

#endif
