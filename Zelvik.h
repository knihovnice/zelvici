/***************************************************************************
 *   Želvíci Gold, Copyright (c) 2006 Dave Schwarz                         *
 *   http://vkapse.aspweb.cz, dave_cz@jabber.cz                            *
 *                                                                         *
 *   Želvíci Touch, (c) 2012 David Schwarz                                 *
 *   http://knihovnik.net                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ZELVIK_H
#define ZELVIK_H

#include "Base.h"
#include "Timer.h"
#include "Sound.h"
#include "Gravity.h"
#include "Common.h"

enum EState
{
    Falling = 1,
    Jumping = 2,
    Staying = 3,
    Crouching = 4
};
enum EDirection
{
    Left = -1,
    Right = 1,
    Neutral = 0
};

class CZelvik : public CBase
{
private:
    CGravity *m_gravity; // Gravity object
    float m_defaultWalk; // default walk speed
    float m_defaultJumpPower; // default jump power

    int m_score; // Score
    float m_jump;
    float m_jumpPower; // Jump power
    float m_walk;    // Walk speed
    short m_health; // Health, default 5
    short m_heart;  // Number of hearts

    bool m_dead; // Is player dead?
    bool m_gameOver; // Just game over indicator
    bool m_invulnerable;  // Player is invulnerable
    bool m_rotating; // Player is rotating
    bool m_pumped;

    EState     m_state; // State
    EDirection m_direction; // Walk direction

    CSound    *m_sound; // Sound object
    CTimer    *m_timer;
public:
    CZelvik(); // Ctor
    ~CZelvik(); // Dtor

    inline CGravity *GetGravity()
    {
        return m_gravity;
    }
    inline void SetDefaultWalk(float value)
    {
        m_defaultWalk = value;
    }
    inline void SetDefaultJumpPower(float value)
    {
        m_defaultJumpPower = value;
    }

    inline bool Pumped()
    {
        return m_pumped;
    }
    inline void GameOver(bool value)
    {
        m_gameOver = value;
    }
    inline bool GameOver()
    {
        return m_gameOver;
    }
    inline void SetSound(CSound *sound)
    {
        m_sound = sound;
    }
    inline void SetScore(int value)
    {
        m_score = value;
    }
    inline int GetScore()
    {
        return m_score;
    }
    inline void SetDead(bool value)
    {
        m_dead = value;
    }
    inline bool IsDead()
    {
        return m_dead;
    }

    inline void SetJumpPower(float value)
    {
        m_jumpPower = (value == 0)? m_defaultJumpPower : value;
    }
    inline float GetJumpPower()
    {
        return m_jumpPower;
    }

    inline EDirection GetDirection()
    {
        return m_direction;
    }

    inline void SetState(EState state)
    {
        m_state = state;
    }
    inline EState GetState()
    {
        return m_state;
    }

    inline short GetColFromR()
    {
        return m_direction == Right? 14 : 45;
    }
    inline short GetColFromL()
    {
        return m_direction == Right? 45 : 14;
    }

    inline void SetInvulnerable(bool value)
    {
        m_invulnerable = value;
    }
    inline bool GetInvulnerable()
    {
        return m_invulnerable;
    }

    void ModifyHeart(short);
    void ModifyHealth(short);
    inline void ModifyScore(int value)
    {
        m_score += value;
    }
    inline short GetHealth()
    {
        return m_health;
    }
    inline short GetHeart()
    {
        return m_heart;
    }
    inline void SetHealth(short value)
    {
        m_health = value;
    }
    inline void SetHeart(short value)
    {
        m_heart= value;
    }

    void Reset();
    void Turn();
    void PumpJump();
    void TurnJump();
    void TurnLeft();
    void TurnRight();
    bool TurnDown();
    void StopWalk();

    void ResetGravity();
    void UpdateGravity();
    void Update();
};

#endif
