/***************************************************************************
 *   �elv�ci Gold, Copyright (c) 2006 Dave Schwarz                         *
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
#ifndef FONT_H
#define FONT_H

#include "SDLHeaders.h"
#include "TextureManager.h"
#include "FontFactory.h"

#include <map>
class CFontFactory;

class CFont
{
private:
    int m_width;
    int m_height;
    SDL_Surface *m_font;
    std::map<char, int> m_charMap;
public:
    CFont(CFontFactory *);
    ~CFont();

    inline int GetWidth()
    {
        return m_width;
    }
    inline int GetHeight()
    {
        return m_height;
    }
    inline SDL_Surface *GetSurface()
    {
        return m_font;
    }
    int GetPosition(const char letter);
};

#endif
