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
#include "Sound.h"
#include "Common.h"
#include <stdio.h>

void CSound::FlushSound()
{
    int count = m_sound.size();
    for(int i = 0; i < count; ++i)
    {
        if(m_sound[i].Chunk)
            Mix_FreeChunk(m_sound[i].Chunk);
    }

    m_sound.clear();
}

CSound::CSound()
{
    m_musicEnabled = true;
    m_soundEnabled = true;
}

CSound::~CSound()
{
    FlushSound();
}

void CSound::LoadMusic(const char *fileName, short slot)
{
    printf("CSound: Loading music\"%s\" (%d)..", fileName, slot);

    if(slot > -1 && slot < 2 && m_musicEnabled)
    {
        if((m_music[slot] = Mix_LoadMUS(fileName)))
            printf("OK.\n");
        else
            printf("ERROR! %s\n", Mix_GetError());
    }
    else
        printf(m_musicEnabled? "Invalid slot!\n" : "Disabled.\n");
}

void CSound::StopMusic(short slot)
{
    if(slot > -1 && slot < 2 && m_musicEnabled)
        Mix_FreeMusic(m_music[slot]);
}

void CSound::PlayMusic(short slot)
{
    if(slot > -1 && slot < 2 && m_musicEnabled)
        Mix_PlayMusic(m_music[slot], -1);
}

void CSound::LoadSound(const char *fileName, const char *alias)
{
    SSoundAsset asset;
    printf("CSound: Loading sound \"%s\" alias \"%s\"..", fileName, alias);

    if(m_soundEnabled)
    {
        asset.Chunk = Mix_LoadWAV(fileName);

        if(asset.Chunk != NULL)
        {
            strcpy(asset.Alias, alias);
            m_sound.push_back(asset);

            printf("OK.\n");
        }
        else
            printf("ERROR!\n");
    }
    else
        printf("Disabled.\n");
}

void CSound::Release(const char *alias)
{
    int count = m_sound.size();

    for(int i = 0; i < count; ++i)
    {
        if(!StringsEquals(alias, m_sound[i].Alias)) continue;

        printf("CSound: Releasing \"%s\"..", alias);
        Mix_FreeChunk(m_sound[i].Chunk);
        std::vector<SSoundAsset>::iterator p = m_sound.begin() + i;
        m_sound.erase(p);

        printf("OK.\n");

        break;
    }
}

void CSound::Play(const char *alias, Sint16 loop)
{
    int count = m_sound.size();
    for(int i = 0; i < count; ++i)
    {
        if(StringsEquals(alias, m_sound[i].Alias))
        {
            Mix_PlayChannel(-1, m_sound[i].Chunk, loop);
            break;
        }
    }

}

void CSound::SetMusicVolume(short value)
{
}

short CSound::GetMusicVolume()
{
    return 0;
}

void CSound::SetSoundVolume(short value)
{
}

short CSound::GetSoundVolume()
{
    return 0;
}
