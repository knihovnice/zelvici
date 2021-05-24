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
#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include "SDLHeaders.h"
#include "Timer.h"
#include "Common.h"
#include "Localizer.h"

#include <vector>

using namespace std;

class CSlideShow
{
private:

    typedef struct SPicture
    {
        int Duration;
        SDL_Surface *Surface;
        SDL_Rect TextLocation;
        char Text[256];
        CTimer *Wait;
    } SPicture;

    vector<SPicture> m_pictures;
    unsigned short m_index;
    SDL_Event m_event;
public:
    CSlideShow();
    ~CSlideShow();

    inline bool IsEnd()
    {
        return m_index >= m_pictures.size();
    }
    bool Load(const char *fileName);
    void Draw(SDL_Surface *primary);
    void Destroy();
};

#endif /* !SLIDESHOW_H */
