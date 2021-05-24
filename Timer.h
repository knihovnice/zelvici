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
#ifndef TIMER_H
#define TIMER_H

#ifdef WIN32
#include <windows.h>
#endif

#include "SDLHeaders.h"

class CTimer
{
private:
    int m_interval;
    int m_now;
    bool m_enabled;
public:
    CTimer();
    CTimer(int);
    CTimer(int,bool);

    inline void SetEnabled(bool value)
    {
        m_enabled = value;
    }
    inline bool IsEnabled()
    {
        return m_enabled;
    }
    inline void SetInterval(int value)
    {
        m_interval = value;
    }
    inline int  GetInterval()
    {
        return m_interval;
    }
    bool Tick();
    void Reset();
};

#endif
