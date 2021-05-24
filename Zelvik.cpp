/***************************************************************************
 *   Želvíci Gold, Copyright (c) 2006 Dave Schwarz                         *
 *   http://vkapse.aspweb.cz, dave_cz@jabber.cz                            *
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
#include "Zelvik.h"
#include <stdio.h>

/*
 * Destructor
 */
CZelvik::~CZelvik()
{
    if(m_timer != NULL)
        delete m_timer;
    if(m_gravity != NULL)
        delete m_gravity;
}

/*
 * Constructor
 */
CZelvik::CZelvik()
{
    // Default position
    m_x = 10.0f;
    m_y = 10.0f;

    // Total width & height
    m_w = 600;
    m_h = 348;

    // Number of rames in vertical, horizontal direction
    m_frameX = 6;
    m_frameY = 4;

    // Frame width & height
    m_frameW = m_w / m_frameX;
    m_frameH = m_h / m_frameY;

    m_health = 5;
    m_heart = 0;
    m_score = 0;
    m_jump = 0.;
    m_pumped = false;
    this->Reset();

    m_timer = new CTimer();
    m_timer->SetInterval(100);
    m_timer->SetEnabled(true);

    m_gravity = new CGravity();
    m_gameOver = false;
}

void CZelvik::Reset()
{
    m_currentFrameX = 6;
    m_currentFrameY = 1;

    m_state = Falling;
    m_walk = 0.0f;
    m_animated = false;
    m_invulnerable = false;
    m_rotating = false;
    m_direction = Right;
}

void CZelvik::Update()
{
    CBase::Update();

    if(!m_rotating && m_timer->Tick())
    {
        if(m_animated && m_state == Staying)
        {
            m_currentFrameX += m_currentFrameX < m_frameX ? 1 : -m_currentFrameX + 1;
        }
    }

    m_rect.x = (m_currentFrameX - 1) * m_frameW;
    m_rect.y = (m_currentFrameY - 1) * m_frameH;
    m_rect.w = m_frameW;
    m_rect.h = m_frameH;
}

void CZelvik::ResetGravity()
{
    m_gravity->ResetGravity();
}

void CZelvik::UpdateGravity()
{
    m_y += m_gravity->Get();

    if(m_state == Jumping)
    {
        m_gravity->AntiGravity();
        if(m_gravity->Get() >= 0.0f)
            m_state = Falling;
    }
    else
    {
        if(m_state != Staying && m_state != Crouching)
            m_state = Falling;
        m_gravity->Update();
    }
}

void CZelvik::Turn()
{
    if(m_rotating)
    {
        m_rotating = !m_timer->Tick();
    }
    else
    {
        m_x += (int)m_walk; // Move player
        m_walk = m_walk > 0 ? m_walk - 0.2f : m_walk + 0.2f; // Balance

        m_timer->SetInterval((int)(200 - (absf(m_walk) * 20))); // Science biatch!

        m_currentFrameY = (m_direction == Right) ? 1 : 3; // Set frame in X axis

        if((int)m_walk == 0) // Stojíme..
        {
            if(m_state == Staying)
                m_currentFrameX = 6; // Frame 6
            m_animated = false; // No animation
        }
    }
}

void CZelvik::PumpJump()
{
    m_pumped = true;
    m_jump = m_jump < m_defaultJumpPower? m_defaultJumpPower : m_jump - 3.0f;
    if(m_jump == m_defaultJumpPower)
        TurnJump();
}

void CZelvik::TurnJump()
{
    m_pumped = false;
    if(m_state == Staying)
    {
        // Combine/randomize & play jumping sound
        char sound[8];
        sprintf(sound, "jump%d", (rand() % 3) + 1);
        m_sound->Play(sound, 0);

        m_currentFrameY = (m_direction == Right) ? 1 : 3; // Set correct frame in Y axis
        m_currentFrameX = 4; // Set frame in X axis
        m_animated = false; // No animation
        m_state = Jumping; // State: jumping
        m_gravity->ResetAntiGravity(); // Reset gravity
        m_gravity->SetAcceleration(m_defaultJumpPower); // Set jump power
        m_jump = 0.0f;
    }
}

void CZelvik::TurnLeft()
{
    if(m_state != Crouching && !m_rotating)
    {
        if(m_direction == Right)
        {
            m_currentFrameY = 2;
            m_currentFrameX = 1;
            m_timer->SetInterval(20);
            m_timer->Reset();
            m_rotating = true;
        }
        else
            m_currentFrameY = 3;

        m_direction = Left;

        m_animated = true;
        m_walk = (m_walk < -m_defaultWalk) ? -m_defaultWalk : m_walk - 0.5f;
    }
}

void CZelvik::TurnRight()
{
    if(m_state != Crouching && !m_rotating)
    {
        if(m_direction == Left)
        {
            m_currentFrameX = m_currentFrameY = 2;
            m_timer->SetInterval(20);
            m_timer->Reset();
            m_rotating = true;
        }
        else
            m_currentFrameY = 1;

        m_direction = Right;
        m_animated = true;
        m_walk = (m_walk > m_defaultWalk) ? m_defaultWalk : m_walk + 0.5f;
    }
}

bool CZelvik::TurnDown()
{
    bool staying = (m_state == Staying);

    if(staying)
    {
        m_animated = false;
        m_state = Crouching;
        m_currentFrameY = 4;
        m_currentFrameX = (m_direction == Left) ? 2 : 1;
    }

    return staying;
}

void CZelvik::StopWalk()
{
    if(m_state == Staying)
        m_currentFrameX = 6;
    m_animated = false;
}

void CZelvik::ModifyHealth(short value)
{
    m_health += value;

    // if hurt factor is less than zero, play "hurt" sound
    if(value < 0)
        m_sound->Play("ublizeni", 0);

    // if total health is more than 5, increment heart, play sound
    if(m_health > 5)
    {
        m_sound->Play("zivot", 0);
        ++m_heart;
        m_health = 1;
    }

    // if total health is less than 1, decrement heart
    if(m_health < 1)
    {
        m_sound->Play("ztrata", 0);
        --m_heart;
        m_health = 5; // Set full health
    }

    // Set game over if total of hearts is less than zero
    m_gameOver = (m_heart < 0);
}

void CZelvik::ModifyHeart(short value)
{
    m_heart += value;
}
