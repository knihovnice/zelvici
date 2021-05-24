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
#ifndef PART_H
#define PART_H

#include "Objects.h"
#include "SDLHeaders.h"
#include "Timer.h"
#include "TextureManager.h"
#include "Camera.h"

#include <cstdlib>
#include <cmath>

const float PI = 3.14159265358979f;

class CParticleSystem
{
private:

    // Defines vector for moving particle
    typedef struct SVector
    {
        float X;
        float Y;
    } SVector;

    typedef struct SParticle
    {
        float X;
        float Y;
        float Speed; // Particle speed
        Sint16 Graphics; // Particle graphics index
        bool IsDead; // If true, particle isn't moving anymore
        SVector	V; // Movement vector
    } SParticle;

    typedef struct SEffect
    {
        Sint16 DeadCount; // Number of dead particles
        Sint16 Count; // Number of all particles
        SParticle *P; // Array of particles
        bool Special;
    } SEffect;

    CObj *m_objects; // Objects in the map
    short *m_objectCount; // Number of objects in the map
    short *m_objectIndices; // Indices of obejects

    Sint16 m_effectCount; // Number of particles
    Uint8 m_divider; // Particle ammount divider
    SEffect *m_effect; // Array of particles
    CTimer *m_timer; // Timer for particle calculation
    SDL_Surface	**m_particle; // Particles graphics
    SDL_Rect m_rect;
    SDL_Rect m_pos;

    CTextureManager *m_textures;
public:
    CParticleSystem(); // Ctor
    ~CParticleSystem(); // Dtor
    inline void SetObject(CObj *objects)
    {
        m_objects = objects;
    }
    inline void SetObjectCount(short *objectCount)
    {
        m_objectCount = objectCount;
    }
    inline void SetObjectIndex(short *objectIndices)
    {
        m_objectIndices = objectIndices;
    }

    inline void SetParticleDivider(Uint8 value)
    {
        m_divider = value;
    }
    inline Sint16 GetEffectCount()
    {
        return m_effectCount;
    }
    void Initialize();
    void FreeMemory();
    void Create(float x,float y, Sint16 count, Sint16 type, float speed);
    void Draw(SDL_Surface *);
private:
    void TestCollision(SParticle *);
};

#endif
