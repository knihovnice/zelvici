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
#ifndef SPRITE_H
#define SPRITE_H

#include "SDLHeaders.h"
#include "Timer.h"
#include <cstdio>

class CSprite
{
private:
    Uint16 m_width;
    Uint16 m_height;
    Uint16 m_frameHeight;
    Uint16 m_frameWidth;

    Uint8 m_framesInX;
    Uint8 m_framesInY;
    Uint8 m_frameX;
    Uint8 m_frameY;

    bool m_animable; // Is it possible to animate this sprite?
    bool m_play;     // Set to True when Play() is called.
    bool m_playNow;  // Set to True by Timer when time to play comes.
    bool m_loop;     // True when we want the animation to repeat.
    Uint8 m_visible;

    CTimer *m_timer;           // Animation speed.
    CTimer *m_timerEach;      // Animation play timer.
    SDL_Surface *m_surface; // Pointer to the graphics.
    SDL_Rect m_rect;
public:
    ~CSprite();
    CSprite();
    CSprite(Uint16 W,Uint16 H,Uint16 FW,Uint16 FH,Uint16 speed);
    CSprite(const char *fileName);

    inline SDL_Surface *GetSurface()
    {
        return m_surface;
    }
    inline void SetSurface(SDL_Surface *surface)
    {
        m_surface = surface;
    }
    inline void SetVisible(bool visible)
    {
        m_visible = visible ? 1 : 0;
    }
    inline bool GetVisible()
    {
        return m_visible==1;
    }
    inline void SetW(Uint16 W)
    {
        m_width = W;
    }
    inline void SetH(Uint16 H)
    {
        m_height = H;
    }
    inline Uint16 GetW()
    {
        return m_width;
    }
    inline Uint16 GetH()
    {
        return m_height;
    }
    inline SDL_Rect *GetRect()
    {
        return &m_rect;
    }

    void LoadSprite(const char *fileName);
    void SetProperties(Uint16 W,Uint16 H,Uint16 FW,Uint16 FH,Uint16 Spd);
    void Play();
    void Stop();
    void Pause();
    void PlayEeach(Uint16 miliseconds);

    void Update();
    void Update(const SDL_Rect *rect);
    void Draw(SDL_Surface *primary);
    void Draw(SDL_Surface *primary, SDL_Rect *position);
    void Draw(SDL_Surface *primary, int x, int y);
private:
    void LoadSpriteFromFile(const char *fileName);
};

#endif
