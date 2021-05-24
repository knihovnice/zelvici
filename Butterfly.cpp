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
#include "ButterFly.h"

CButterFly::CButterFly()
{
    // Setting the timer
    m_timer = new CTimer(20, true); // 20ms

    // Setting the random position
    m_x = (rand() % 800);
    m_y = (rand() % 600);

    // Setting the butterfly frames
    m_currentFrameX =
        m_currentFrameY = 1;
}

CButterFly::~CButterFly()
{
    //if(m_sprite) delete m_sprite;
    if(m_timer != NULL) delete m_timer;
}

void CButterFly::Update()
{
    if(m_timer->Tick()) // Every 20ms
    {
        CScreen *screen = CScreen::GetInstance();
        m_x += 4 * m_direction; // Move to the m_Direction
        m_currentFrameX = m_currentFrameX > (m_frameX - 1) ? 1 : m_currentFrameX + 1; // Animation
        CCamera *camera = CCamera::GetInstance();
        if((m_x + camera->GetX() > (2 * screen->GetWidth()) || m_y + camera->GetY() > screen->GetHeight())
                && m_direction == BUTTERFLY_RIGHT) // Disappeared to the right
        {
            m_y = (rand() % screen->GetHeight()) - camera->GetY(); // Creating random position in Y axis
            m_x = -(rand() % screen->GetWidth()) - m_frameW - camera->GetX(); // Placing back to the start
        }

        if((m_x + camera->GetX() + m_frameW < -screen->GetHeight() || m_y + m_frameH + camera->GetY() < 0) &&
                m_direction == BUTTERFLY_LEFT) // Disappeared to the left
        {
            m_y = (rand() % screen->GetHeight()) - camera->GetY(); // Creating random position in Y axis
            m_x = screen->GetWidth() - camera->GetX() + (rand() % screen->GetWidth()); // Back to the start :)
        }
    }

    m_rect.x = (m_currentFrameX - 1) * m_frameW;
    m_rect.y = (m_currentFrameY - 1) * m_frameH;
    m_rect.w = m_frameW;
    m_rect.h = m_frameH;

    CCamera *camera = CCamera::GetInstance();

    m_pos.x = (short) GetX() + camera->GetX();
    m_pos.y = (short) GetY() + camera->GetY();
}
