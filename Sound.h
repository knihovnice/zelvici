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
#ifndef SOUND_H
#define SOUND_H

#include "SDLHeaders.h"

#include <vector>
#include <cstring>


typedef struct SSoundAsset
{
    char Alias[32];
    Mix_Chunk *Chunk;

} SSoundAsset;

class CSound
{
private:
    // TODO: Replace vector with map<SSounAsset, char*>
    std::vector<SSoundAsset> m_sound;
    Mix_Music  *m_music[2];
    bool m_musicEnabled;
    bool m_soundEnabled;
public:
    CSound();
    ~CSound();

    inline void SetMusicEnabled(bool enabled)
    {
        m_musicEnabled = enabled;
    }
    inline void SetSoundEnabled(bool enabled)
    {
        m_soundEnabled = enabled;
    }
    void Release(const char *);
    void LoadSound(const char *fileName, const char *alias);
    void Play(const char *alias, Sint16 loop);
    void FlushSound();

    void LoadMusic(const char *fileName, short slot);
    void PlayMusic(short);
    void StopMusic(short);

    void SetMusicVolume(short);
    short GetMusicVolume();
    void SetSoundVolume(short);
    short GetSoundVolume();
};

#endif
