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
#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include "Base.h"
#include "Timer.h"
#include "SDLHeaders.h"
#include "Sprite.h"
#include "Screen.h"
#include "Camera.h"
#include <cstdlib>

const Sint8 BUTTERFLY_LEFT = -1;
const Sint8 BUTTERFLY_RIGHT = 1;

class CButterFly : public CBase // Inherits CBase
{
private:
    CTimer *m_timer;      // Pointer to the timer
    Sint8 m_direction;    // Direction
public:
    CButterFly();
    ~CButterFly();

    inline void SetDirection(Sint8 direction)
    {
        m_direction = direction;
        m_x = (direction == 1)? -m_frameH : CScreen::GetInstance()->GetHeight();
    }
    inline Sint8 GetDirection()
    {
        return m_direction;
    }
    void Update();
};

#endif
