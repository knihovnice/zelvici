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
#include "Sprite.h"

/*
 ~CSprite(), destructor
 Freeing the memory (deallocation)
*/
CSprite::~CSprite()
{
    if(m_timer != NULL)
        delete m_timer;
    if(m_timerEach != NULL)
        delete m_timerEach;
}

/*
 CSprite(), constructor #0
 Setting the default properties; ready to be set later using set properties
*/
CSprite::CSprite()
{
    m_visible = 1;

    m_width = 0;
    m_height = 0;
    m_frameWidth = 0;
    m_frameHeight = 0;
    m_framesInX = 0;
    m_framesInY = 0;

    m_rect.x = 0;
    m_rect.y = 0;
    m_rect.w = 0;
    m_rect.h = 0;

    m_frameX = 1;
    m_frameY = 1;

    m_timer = new CTimer();
    m_timerEach = new CTimer();
    m_play = true;
    m_loop = true;
}

/*
 CSprite(), constructor #1
 Setting properties via configuration file
*/
CSprite::CSprite(const char *fileName)
{
    m_visible = 1;
    this->LoadSpriteFromFile(fileName);
}

/*
 CSprite(), constructor #2
 Setting properties and memory allocation for timer
*/
CSprite::CSprite(Uint16 W, Uint16 H, Uint16 FW, Uint16 FH, Uint16 speed)
{
    m_visible = 1;
    m_width = W;
    m_height = H;
    m_frameWidth = FW;
    m_frameHeight = FH;
    m_framesInX = ((Uint8)(W/FW));
    m_framesInY = ((Uint8)(H/FH));
    m_frameX = m_frameY = 1;

    // Decision whether to animate this sprite
    m_animable = (m_framesInX > 1 || m_framesInY > 1);

    // Memory allocation for timer
    m_timer = new CTimer((int)speed); // Setting the interval

    m_timerEach = new CTimer();
    m_play = m_loop = true;
}

/*
 CSprite::SetProperties()
 Setting the sprite properties
*/
void CSprite::SetProperties(Uint16 W, Uint16 H, Uint16 FW, Uint16 FH, Uint16 speed)
{
    m_width = W;
    m_height = H;
    m_frameWidth = FW;
    m_frameHeight = FH;
    m_framesInX = ((Uint8)(W/FW));
    m_framesInY = ((Uint8)(H/FH));
    m_frameX = m_frameY = 1;

    // Decision whether to animate this sprite
    m_animable = (m_framesInX > 1 || m_framesInY > 1);

    // Setting the interval (i.e. speed) of the animation timer
    m_timer->SetInterval((int)speed);
}
/*
 LoadSprite()
 Calls private procedure that loads sprite properties from the file
*/
void CSprite::LoadSprite(const char *fileName)
{
    this->LoadSpriteFromFile(fileName);
}
/*
 Animates sprite every X (m_Timer_Each) ms at the speed y (m_Timer)
*/
void CSprite::Update()
{
    if(m_timerEach->Tick() && m_play)
        m_playNow = true; // It's time to play animation

    // Let's animate
    if(m_timer->Tick() && m_playNow && m_play)
    {
        m_frameX = m_frameX > (m_framesInX - 1) ? 1 : m_frameX + 1;

        // If the frame is equal to 1, PlayNow is set to False and we are having a pause ;)
        if(!(m_playNow = !(m_frameX == 1)))
        {
            m_play = m_loop;       // No loop, no animation
            m_timerEach->Reset(); // Timer reset ;)
        }
    }

    // Rectangle cut
    m_rect.x = (m_frameX - 1) * m_frameWidth;
    m_rect.y = (m_frameY - 1) * m_frameHeight;
    m_rect.w = m_frameWidth;
    m_rect.h = m_frameHeight * m_visible;
}

/*
 Play();
 Allows the animation playing
*/
void CSprite::Play()
{
    m_play = true;
}

/*
 Stop();
 Stops the animation
*/
void CSprite::Stop()
{
    m_play = false;
    m_playNow = false;
    m_timerEach->Reset();
}

/*
 Pause();
 Pauses the animation
*/
void CSprite::Pause()
{
    m_play = false;
}

/*
 PlayEach();
 Sets the interval of the animation, i.e. how often it should be played
*/
void CSprite::PlayEeach(Uint16 miliseconds)
{
    m_timerEach->SetInterval((int)miliseconds);
}

/*
 LoadSpriteFromFile();
 Loads sprite properties from file
*/
void CSprite::LoadSpriteFromFile(const char *fileName)
{
    // TODO (dave#1#): Implement loading sprite properties from file... one day :)
}

void CSprite::Draw(SDL_Surface *primary, int x, int y)
{
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface(m_surface, &m_rect, primary, &position);
}

void CSprite::Draw(SDL_Surface *primary, SDL_Rect *position)
{
    SDL_BlitSurface(m_surface, &m_rect, primary, position);
}
