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
#ifndef DRAW_H
#define DRAW_H

#include "SDLHeaders.h"
#include "Camera.h"
#include "Map.h"
#include "Zelvik.h"
#include "Screen.h"
#include "Font.h"
#include "Particles.h"
#include "Engine.h"
#include "Menu.h"
#include "SpriteText.h"

#include <vector>

class CEngine;
class CMap;
using namespace std;

class CDraw
{
private:
    bool m_fps;
    SDL_Rect m_pos;
    SDL_Rect m_rect;

    CMap *m_map;
    CZelvik *m_player;
    CFont *m_font;
    CParticleSystem *m_particles;
    CEngine *m_engine;
    CMenu *m_menu;
    short m_drawObjectCount; // Number of objects to be drawn
    short m_drawEnemyCount;  // Number of enemies to be drawn
    short m_drawItemCount;   // Number of items to be drawn
    short *m_drawObject;     // Array of objects indexes
    short *m_drawEnemy;      // Array of enemies indexes
    short *m_drawItem;       // Array of items indexes

    const char *ENEMIES_KNOCKED;
    const char *VEGETABLES_PICKED;
    const char *PRESS_ENTER;
    const char *SCORE;
    const char *ENEMY_BONUS;
    const char *ITEMS_BONUS;

    CSpriteText *m_enemiesKnocked;
    CSpriteText *m_vegetablesPicked;
    CSpriteText *m_pressEnter;
    CSpriteText *m_score;
    CSpriteText *m_commonText;
public:
    CDraw();
    ~CDraw();

    inline short GetEnemyIndex(short i)
    {
        return m_drawEnemy[i];
    }
    inline short SetEnemyIndex(short i, short enemyIndex)
    {
        m_drawEnemy[i] = enemyIndex;
    }
    inline short GetItemIndex(short i)
    {
        return m_drawItem[i];
    }
    inline short SetItemIndex(short i, short itemIndex)
    {
        m_drawItem[i] = itemIndex;
    }
    inline short GetObjectIndex(short i)
    {
        return m_drawObject[i];
    }
    inline short SetObjectIndex(short i, short objectIndex)
    {
        m_drawObject[i] = objectIndex;
    }

    inline short GetEnemyCount()
    {
        return m_drawEnemyCount;
    }
    inline short GetObjectCount()
    {
        return m_drawObjectCount;
    }
    inline short GetItemCount()
    {
        return m_drawItemCount;
    }
    inline void IncrementEnemyCount()
    {
        ++m_drawEnemyCount;
    }
    inline void IncrementObjectCount()
    {
        ++m_drawObjectCount;
    }
    inline void IncrementItemCount()
    {
        ++m_drawItemCount;
    }
    inline void SetDrawObjectCount(short value)
    {
        m_drawObjectCount = value;
    }
    inline void SetDrawEnemyCount(short value)
    {
        m_drawEnemyCount = value;
    }
    inline void SetDrawItemCount(short value)
    {
        m_drawItemCount = value;
    }
    void ReinitalizeDrawObjects();
    void ResetDrawCounts();

    inline void SetFps(bool value)
    {
        m_fps = value;
    }
    inline bool GetFps()
    {
        return m_fps;
    }
    inline void SetMap(CMap *map)
    {
        m_map = map;
    }
    inline void SetPlayer(CZelvik *player)
    {
        m_player = player;
    }
    inline void SetParticles(CParticleSystem *particles)
    {
        m_particles = particles;
    }
    inline void SetEngine(CEngine *engine)
    {
        m_engine = engine;
    }
    inline void SetMenu(CMenu *menu)
    {
        m_menu = menu;
    }

    void Initialize();

    void DrawGame(SDL_Surface *surface);
    void DrawMisc(SDL_Surface *surface);
};

#endif
