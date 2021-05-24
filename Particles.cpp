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
#include "Particles.h"

CParticleSystem::~CParticleSystem()
{
    FreeMemory();
    delete m_timer;
    printf("CParticleSystem: Memory freed.\n");
}

CParticleSystem::CParticleSystem()
{
    m_divider = 2;
    m_effectCount = 0;
    m_effect = NULL;
    m_textures = CTextureManager::GetInstance();

    m_timer = new CTimer();
    m_timer->SetInterval(10);
    m_timer->SetEnabled(true);
}

void CParticleSystem::FreeMemory()
{
    for(Sint16 i = 0; i < m_effectCount; ++i)
    {
        if(m_effect[i].P) delete[] m_effect[i].P;
    }

    if(m_effect)
        free(m_effect);
}

void CParticleSystem::Initialize()
{
    char fileName[64];
    char name[32];

    // Initialize array for particle surfaces
    m_particle = new SDL_Surface*[18];
    m_effectCount = 0;

    // Load particle graphics into memory
    for(Uint16 i = 0; i < 18; ++i)
    {
        sprintf(fileName, "./screen/particles/%d.bmp", i + 1);
        sprintf(name, "part%d", i);
        m_particle[i] = m_textures->LoadSurface(name, "effect", fileName, true, 255, 0, 255);
    }

    m_rect.x = 0;
    m_rect.y = 0;
    m_rect.w = 13;
    m_rect.h = 13;
}

void CParticleSystem::Create(float x,float y, Sint16 count, Sint16 type, float speed)
{
    CCamera *camera = CCamera::GetInstance();
    count = count / m_divider; // Divide the ammount

    // Allocate memory for new particles
    m_effect = (m_effect == NULL)?
               (SEffect*) malloc(sizeof(SEffect)):
               (SEffect*) realloc(m_effect, sizeof(SEffect) * (m_effectCount + 1));

    SEffect *effect = &m_effect[m_effectCount];
    effect->Special = (type == 666); // 666 is.. special :)
    effect->DeadCount = 0;
    effect->Count = count; // Number of particles
    effect->P = new SParticle[count]; // Allocate memory for particle items

    for(Uint16  i = 0; i < count; ++i)
    {
        // Set particle item properties
        effect->P[i].IsDead = false;
        effect->P[i].Speed = (rand() % 3) + 1; // Randomize speed
        effect->P[i].V.X = 10 - (rand() % 20); // Radomize initial speed
        effect->P[i].V.Y = - ((rand() % 20) + 10);
        effect->P[i].X = x - camera->GetX(); // X
        effect->P[i].Y = y - camera->GetY(); // Y
        effect->P[i].Graphics = (type == 666) ? rand() % 18 : type; // Set graphics according to type
    }

    ++m_effectCount;
}
void CParticleSystem::TestCollision(SParticle *particle)
{
    short count = *m_objectCount;
    for(register short i = 0; i < count; ++i)
    {
        short j = m_objectIndices[i];
        if(particle->Y > m_objects[j].GetY() &&
                particle->Y < (m_objects[j].GetY() + 12 + particle->V.Y) &&
                particle->X > m_objects[j].GetX() &&
                particle->X < (m_objects[j].GetX() + m_objects[j].GetFrameW()) &&
                m_objects[j].GetColiding())
        {
            particle->V.Y=-(particle->V.Y / 2.0f);
            break;
        }
    }
}


void CParticleSystem::Draw(SDL_Surface *surface)
{
    CCamera *camera = CCamera::GetInstance();
    bool update = m_timer->Tick();

    // For each particle
    for(register Sint16 j = 0; j < m_effectCount; ++j)
    {
        if(m_effect[j].P == NULL) continue;

        // For each particle item
        for(Uint16 i = 0; i < m_effect[j].Count; ++i)
        {
            if((m_effect[j].P[i].Y + camera->GetY() < 600) &&
                    ((m_effect[j].P[i].V.Y < -0.2f || m_effect[j].P[i].V.Y > 0.2f) ||
                     (m_effect[j].P[i].V.X < -0.2f || m_effect[j].P[i].V.X > 0.2f)))
            {
                if(update) // Update now?
                {
                    // Test collision with objects
                    TestCollision(&m_effect[j].P[i]);

                    // Calculate new coordinates
                    m_effect[j].P[i].X += m_effect[j].P[i].V.X;
                    m_effect[j].P[i].Y += (m_effect[j].P[i].V.Y / m_effect[j].P[i].Speed);
                    m_effect[j].P[i].V.X += ((-m_effect[j].P[i].V.X) / 50.0f);
                    ++m_effect[j].P[i].V.Y;
                }

                m_pos.x = (short)(m_effect[j].P[i].X + camera->GetX());
                m_pos.y = (short)(m_effect[j].P[i].Y + camera->GetY());

                SDL_BlitSurface(m_particle[m_effect[j].P[i].Graphics], &m_rect, surface, &m_pos);

                continue;
            }

            if(!m_effect[j].P[i].IsDead)
            {
                m_effect[j].P[i].IsDead = true;
                ++m_effect[j].DeadCount;
            }

            // Is whole particle effect dead?
            if(m_effect[j].DeadCount < m_effect[j].Count) continue;

            delete m_effect[j].P; // Free memory
            memcpy((void*)&m_effect[j],(void*)&m_effect[m_effectCount - 1], sizeof(SEffect)); // Reallocate array
            --m_effectCount;
            m_effect = (SEffect *) realloc(m_effect, sizeof(SEffect) * (m_effectCount));
            break;
        }
    }

}
