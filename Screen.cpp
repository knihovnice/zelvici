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
#include "Screen.h"
#include <cstdlib>
#include <stdio.h>


CScreen::CScreen()
{
}

CScreen *CScreen::GetInstance()
{
    static CScreen *screen = new CScreen();
    return screen;
}

bool CScreen::IsOnScreen(float x, float y, float w, float h)
{
    return (x + w) > 0
           && x < m_width
           && (y + h) > 0
           && y < m_height;
}

SDL_Surface *CScreen::Initialize(int w, int h, int bpp, int refresh, bool windowed)
{
    m_width = w;
    m_height = h;
    m_bpp = bpp;
    m_refresh = refresh;

    m_widthHalf = w / 2;
    m_heightHalf = h / 2;

    Uint32 flags = windowed? (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL) :
                   (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL | SDL_FULLSCREEN);
    return SDL_SetVideoMode(w, h, bpp, flags);
}
