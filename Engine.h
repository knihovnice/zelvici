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
#ifndef ENGINE_H
#define ENGINE_H

#include "SDLHeaders.h"
#include "Config.h"
#include "Zelvik.h"
#include "Font.h"
#include "TextureManager.h"
#include "Screen.h"
#include "Particles.h"
#include "Map.h"
#include "Draw.h"
#include "Camera.h"
#include "Menu.h"
#include "Sound.h"
#include "SlideShow.h"
#include "Localizer.h"
#include "Timers.h"
#include "Sprite.h"
#include "ButterFly.h"
#include "FontFactory.h"

#include <cstdio>
#include <cstdlib>

class CMap;
class CDraw;
class CMenu;
class CButterFly;
class CSprite;

class CEngine
{
private:
    SDL_Surface *m_surface;       // Primary surface

    // Items
    SDL_Surface **m_surfaceObjects; // Surface cache for objects
    SDL_Surface **m_surfaceEnemies; // Surface cache for enemies
    SDL_Surface **m_surfaceItems; // Surface cache for items

    // Backgrounds
    SDL_Surface  *m_surfaceBack;
    SDL_Surface  *m_surfaceBack2;
    SDL_Surface  *m_surfaceFont;

    // Sprites
    CButterFly *m_bFly1;
    CButterFly *m_bFly2;
    CButterFly *m_bFly3;
    CSprite    *m_head;
    CSprite    *m_heart;

    CTimer *m_endTimer;           // After the certain interval game ends
    CTimer *m_fpsTimer;           // FPS counts (1 second interval)
    CTimer *m_invulnerable;       // Timer for invulnerable counting
    CTimer *m_playerAnimation;
    CTimer *m_update;
    CTimer *m_gravity;
    CTimer *m_trampoline;
    CTimer *m_animation;
    CTimer *m_enemy;

    CZelvik *m_player;            // Player
    CTextureManager *m_textureManager;    // Texture manager

    const CFont *m_font;                // Font
    CSound *m_sound;              // Sound system
    CParticleSystem *m_particles; // Particle system
    CMap *m_map;                  // Map
    CDraw *m_draw;                // Draw system
    CMenu *m_menu;                // Menu
    CSlideShow *m_slide;          // Slide show

    bool m_run;                   // Indication of running program
    bool m_runMenu;               // Indication of running menu
    bool m_spritesInitialized;

    int m_ticks;                  // Ticks (when the loop begins)
    Uint8 *m_keys;                // Array of keys
    Uint16 m_fps;                 // Frames per second
    SDL_Rect m_rect, m_pos;
    char m_mapList[32];           // Map list

    bool m_debugScore;
public:
    CEngine();
    ~CEngine();

    inline SDL_Surface *GetSurfaceObject(int index)
    {
        return m_surfaceObjects[index];
    }
    inline SDL_Surface *GetSurfaceEnemy(int index)
    {
        return m_surfaceEnemies[index];
    }
    inline SDL_Surface *GetSurfaceItem(int index)
    {
        return m_surfaceItems[index];
    }
    inline SDL_Surface *GetSurfaceBackground()
    {
        return m_surfaceBack;
    }
    inline SDL_Surface *GetSurfaceBackground2()
    {
        return m_surfaceBack2;
    }

    inline CSprite *GetHeadSprite()
    {
        return m_head;
    }
    inline CSprite *GetHeartSprite()
    {
        return m_heart;
    }

    inline Uint16 GetFps()
    {
        return m_fps;
    }

    CButterFly *GetButterfly(int index);
    void GameOver();
    void NewGame(const char*,short, short);
    void LoadGame();
    void ExitGame();
    void InitGame(const char*);
    bool Initialize(bool);
    void LoadConfig();
    bool LoadGraphics(Uint16 theme);
    bool LoadSounds();
    void Start();
    void UpdateKeys();
    void Update();
};

#endif
