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
#ifndef BASE_H
#define BASE_H
#include "SDLHeaders.h"
#include "Camera.h"

class CBase
{
protected:
    short m_asset;

    // Coordinates
    float m_x;
    float m_y;

    bool  m_animated;  // Is it animated?

    short m_frameX;
    short m_frameY;     // Number of frames in X,Y axis
    short m_frameW;
    short m_frameH;     // Frame width and height
    short m_w;
    short m_h;             // Overall frame width and height
    short m_currentFrameX;
    short m_currentFrameY; // Current frame in X,Y axis

    SDL_Rect m_pos;
    SDL_Rect m_rect;             // SDL rectangle
    SDL_Surface *m_surface;   // SDL surface
public:
    CBase()
    {
        m_surface = NULL;
    }
    virtual ~CBase() { }

    inline short GetAsset()
    {
        return m_asset;
    };
    inline void SetAsset(short asset)
    {
        m_asset = asset;
    };

    inline virtual SDL_Surface *GetSurface()
    {
        return m_surface;
    }
    inline void SetSurface(SDL_Surface *surface)
    {
        m_surface = surface;
    }
    inline float GetX ()
    {
        return m_x;
    }
    inline float GetY ()
    {
        return m_y;
    }
    inline void  SetX (float value)
    {
        m_x = value;
    }
    inline void  SetY (float value)
    {
        m_y = value;
    }

    inline short GetFrameW()
    {
        return m_frameW;
    }
    inline short GetFrameH()
    {
        return m_frameH;
    }
    inline void  SetFrameW(int value)
    {
        m_frameW = value;
        m_frameX = m_w / m_frameW;
    }
    inline void  SetFrameH(int value)
    {
        m_frameH = value;
        m_frameY = m_w / m_frameH;
    }

    inline short GetCurrentFrameX()
    {
        return m_currentFrameX;
    }
    inline short GetCurrentFrameY()
    {
        return m_currentFrameY;
    }
    inline void  SetCurrentFrameX(short value)
    {
        m_currentFrameX = value;
    }
    inline void  SetCurrentFrameY(short value)
    {
        m_currentFrameY = value;
    }

    inline short GetWidth()
    {
        return m_w;
    }
    inline short GetHeight()
    {
        return m_h;
    }
    inline void  SetWidth(int value)
    {
        m_w = value;
    }
    inline void  SetHeight(int value)
    {
        m_h = value;
    }

    inline void  IncrementX (float value)
    {
        m_x += value;
    }
    inline void  IncrementY (float value)
    {
        m_y += value;
    }

    inline void  SetAnimated(bool value)
    {
        m_animated = value;
    }
    inline bool  GetAnimated()
    {
        return m_animated;
    }

    virtual void Draw(SDL_Surface *target);
    virtual void Update();
};

#endif
