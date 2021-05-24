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
#ifndef MAPA_H
#define MAPA_H

#include <vector>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "Camera.h"
#include "Screen.h"
#include "TextureManager.h"
#include "Engine.h"
#include "Zelvik.h"
#include "Sound.h"
#include "Draw.h"
#include "Particles.h"
#include "Environment.h"
#include "Timers.h"

using namespace std;

class CDraw;
class CEngine;

typedef struct SLocation
{
    int X;
    int Y;
} SLocation;

//
// Map class
//
class CMap
{
private:
    typedef struct SLevel // Structure of one game level
    {
        char  Name[8]; // Name of map
        short Theme;      // Map theme
        short AssetCount;     // Graphics top of stack
    } SLevel;

    typedef struct SObject // Objects for loading from mapfile
    {
        int X,Y;
        char fX,fY;
        short Jump;
        short Asset;
    } SObject;

    typedef struct SItem // Items
    {
        int X;
        int Y;
        char fX;
        char fY;
        short Asset;
    } SItem;

    typedef struct SEnemy // Enemies
    {
        int X;
        int Y;
        char fX;
        char fY;
        short Asset;
        short Len;
        short Speed;
        short Destr;
    } SEnemy;

    char m_musicFile[32];  // Music file name
    short m_currentMap;     // Index of current map
    short m_exceptEnemy;   // Enemy that won't be destroyed
    vector<SLevel> m_mapList; // List of maps

    CTextureManager *m_textures;  // Texture manager
    CEngine *m_engine;      // Game manager
    CSound *m_sound;    // Sound manager
    CDraw *m_draw;      // Draw manager
    CParticleSystem *m_particles; // Particle manager

    short m_theme; // Theme index
    bool m_endLevel;   // Is end of level?

    SLocation   m_start; // Start position
    SLocation   m_stop;  // Destination postition
    SLocation   m_null;  // Death position
    CObj *m_objects;   // Objects
    CEnm *m_enemies;   // Enemies
    CItm *m_items;   // Items

    CTimer *m_invulnerable;
    CTimer *m_trampoline;

    short m_lastTheme; // Previous theme index
    short m_objectCount;  // Number of objects
    short m_itemCount;  // Number of items
    short m_enemyCount;  // Number of enemies
    short m_nonkonckableEnemyCount; // Number of enemies that can't be knocned
    short m_leftBehind; // // Number of enemies that are erased because of difficulty level

    short m_enemiesKnocked;  // Number of killed enemies
    short m_itemsPicked;  // Number of collected items

    bool m_levelFinished;

    // Background coordinates
    SLocation m_bg1_a;
    SLocation m_bg1_b;
    SLocation m_bg2_a;
    SLocation m_bg2_b;

public:
    CMap();
    ~CMap();

    inline bool IsLevelFinished()
    {
        return m_levelFinished;
    }
    void SetLevelFinished(bool value, CZelvik *player);

    inline SLocation GetNull()
    {
        return m_null;
    }
    inline SLocation GetStart()
    {
        return m_start;
    }
    inline SLocation GetStop()
    {
        return m_stop;
    }
    inline short GetEnemiesKnocked()
    {
        return m_enemiesKnocked;
    }
    inline short GetEnemiesTotal()
    {
        return m_enemyCount - m_nonkonckableEnemyCount - m_leftBehind;
    }
    inline short GetEnemyCount()
    {
        return m_enemyCount;
    }
    inline short GetItemsPicked()
    {
        return m_itemsPicked;
    }
    inline short GetItemCount()
    {
        return m_itemCount;
    }
    inline short GetObjectCount()
    {
        return m_objectCount;
    }

    inline short GetTheme()
    {
        return m_theme;
    }
    inline CObj *GetObject(int index)
    {
        return &m_objects[index];
    }
    inline CEnm *GetEnemy(int index)
    {
        return &m_enemies[index];
    }
    inline CItm *GetItem(int index)
    {
        return &m_items[index];
    }
    inline bool IsEndLevel()
    {
        return m_endLevel;
    }
    inline bool SetEndLevel(bool value)
    {
        m_endLevel = value;
    }

    inline void SetCurrentMap(short value)
    {
        m_currentMap = value;
    }
    inline void SetExcept(short value)
    {
        m_exceptEnemy = value;
    }
    inline void SetEngine(CEngine *engine)
    {
        m_engine = engine;
    }
    inline void SetSound(CSound *sound)
    {
        m_sound = sound;
    }
    inline void SetDraw(CDraw *draw)
    {
        m_draw = draw;
    }
    inline void SetParticle(CParticleSystem *particles)
    {
        m_particles = particles;
    }

    void ResetStatistics();
    void RestartMap();
    void StartMap();
    void EndGame();
    void SaveGame(CZelvik *, const char *);
    void LoadGame(CZelvik *, char *);
    bool LoadMapList(const char *);
    bool LoadMap(const char *);
    bool SaveMap();

    bool ObjectColision(CZelvik *);
    void ItemColision(CZelvik *);
    void EnemyColision(CZelvik *);

    void Draw(SDL_Surface *surface);
    void Update();
};

#endif
