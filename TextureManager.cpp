/***************************************************************************
 *   �elv�ci Gold, Copyright (c) 2006 Dave Schwarz                         *
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
#include "TextureManager.h"
#include <stdio.h>
#include <cstdlib>

/*
 * Destructor
*/
CTextureManager::~CTextureManager()
{
    Sint16 count = m_textures.size();
    Uint16 surfaceCount = 0;

    for(Uint16 i = 0; i < count; ++i)
    {
        if(m_textures[i].Surface != NULL)
        {
            SDL_FreeSurface(m_textures[i].Surface);
            ++surfaceCount;
        }
    }

    m_textures.clear();
    printf("CTextureManager: Memory freed (%d surfaces).\n", surfaceCount);
}

/*
 Konstruktor
*/
CTextureManager::CTextureManager()
{
}

CTextureManager *CTextureManager::GetInstance()
{
    static CTextureManager *instance = new CTextureManager();
    return instance;
}

SDL_Surface *CTextureManager::LoadSurface(const char *name, const char *group, const char *fileName)
{
    return this->LoadSurface(name, group, fileName, true, 255, 255, 255);
}

/*
 LoadSurface();
 Nahraje obr�zek do pam�ti a ulo�� do "asociovan�ho" pole d�len�ho do skupin.
*/
SDL_Surface *CTextureManager::LoadSurface(const char *name, const char *group, const char *fileName,
        bool transparent, short R, short G, short B)
{
    // Test, zda ji� tento povrch neexistuje (ve stejn� skupin�)
    Sint16 textureCount = m_textures.size();

    // Return surface if already exists
    for(Uint16 i = 0; i < textureCount; ++i)
    {
        if( StringsEquals(name, m_textures[i].Name) &&
                StringsEquals(group, m_textures[i].Group))
            return m_textures[i].Surface;
    }

    printf("CTextureManager: Loading \"%s\" alias %s->%s..", fileName, group, name);

    Texture texture;
    texture.Surface = SDL_LoadBMP(fileName);

    if(texture.Surface == NULL)
    {
        printf("ERROR!");
        return NULL;
    }
    else
    {
        strcpy(texture.Name, name); // Ulo��me jm�no
        strcpy(texture.Group, group); // Ulo��me skupinu

        // Nastav�me transparenci
        if(transparent)
        {
            int key = SDL_MapRGB(texture.Surface->format, R, G, B);
            SDL_SetColorKey(texture.Surface, SDL_SRCCOLORKEY, key);

            texture.Surface->flags = SDL_HWACCEL | SDL_SRCCOLORKEY;
        }
        else
            texture.Surface->flags = SDL_HWACCEL;

        m_textures.push_back(texture);
        printf("OK.\n");

        return texture.Surface; // Vrac�me ukazatel
    }
}
/*
 ReleaseAll();
 Uvoln� pam�..
*/
void CTextureManager::ReleaseAll()
{
    Sint32 count = m_textures.size();

    for(Uint16 i = 0; i < count; ++i)
    {
        if(m_textures[i].Surface != NULL)
            SDL_FreeSurface(m_textures[i].Surface);
    }

    m_textures.clear();
    printf("CTextureManager: Surfaces freed.\n");
}
/*
 ReleaseSurface();
 Uvoln�n� pam�ti konkr�tn� grafiky (rozli�ujeme podle jm�na �i skupiny)
*/
void CTextureManager::ReleaseSurface(const char *name, bool byGroup)
{
    Uint16 count = 0; // Po�et uvoln�n�ch povrch�
    Sint16 size = m_textures.size(); // Po�et pointer�..
    std::vector<Texture>::iterator p; // Iter�tor

    for(Uint16 i = 0; i < size; ++i) // Prolezeme grafiku
    {
        if( (!byGroup && StringsEquals(name,m_textures[i].Name)) || // Podle jm�na �i..
                (byGroup && StringsEquals(name,m_textures[i].Group))) // Podle skupiny..
        {
            // printf("CTextureManager::ReleaseSurface: Uvol�uji %s->%s..",m_textures[i].Group,m_textures[i].Name);
            SDL_FreeSurface(m_textures[i].Surface); // Uvoln�n� pam�ti
            m_textures[i].Surface = NULL; // Pojistka ;b
            p = m_textures.begin() + i;
            m_textures.erase(p); // Odstran�me jej z pole..
            size = m_textures.size(); // Aktualizujeme velikost
            // printf("Yeah!\n");
            if(!byGroup) // Pokud uvol�ujeme podle jm�na, kon��me cyklus
                break;
            else
            {
                --i; // Vrac�me se zp�t o jede cyklus..
                ++count; // Pokud podle skupiny, pokra�ujeme v cyklu a inkrementujeme po�et uvoln�n�ch
            }
        }
    }

    if(byGroup)
        printf("%d assets freed from memory.\n", count); // V�pis po�tu uvoln�n�ch povrch�

    return;
}

