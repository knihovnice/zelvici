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
#include "Timer.h"

void CTimer::Reset()
{
#ifdef WIN32
    m_now = GetTickCount() + m_interval;
#else
    m_now = SDL_GetTicks() + m_interval;
#endif
}

bool CTimer::Tick()
{
#ifdef WIN32
    if((signed int)GetTickCount() >= m_now && m_enabled)
#else
    if((Sint32)SDL_GetTicks() >= m_now && m_enabled)
#endif
    {
#ifdef WIN32
        m_now = GetTickCount() + m_interval;
#else
        m_now = SDL_GetTicks() + m_interval;
#endif
        return true;
    }
    return false;
}

CTimer::CTimer()
{
    m_now = 0;
    m_interval = -1;
    m_enabled = true;
}
CTimer::CTimer(int interval)
{
    m_now = 0;
    m_interval = interval;
    m_enabled = true;
}

CTimer::CTimer(int interval, bool enabled)
{
    m_interval = interval;
    m_enabled = enabled;
    m_now = 0;
}
