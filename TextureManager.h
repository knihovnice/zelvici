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
#ifndef TEXMAN_H
#define TEXMAN_H

#include "SDLHeaders.h"
#include "Common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <cstring>

class CTextureManager
{
private:
    typedef struct Texture
    {
        SDL_Surface *Surface; // SDL surface
        char Name[32]; // Asset name
        char Group[32]; // Group name
    } Texture;

    std::vector<Texture> m_textures; // Assets
private:
    CTextureManager();
public:
    static CTextureManager *GetInstance();
    SDL_Surface *LoadSurface(const char *name, const char *group, const char *fileName, bool transparent, short R, short G, short B);
    SDL_Surface *LoadSurface(const char *name, const char *group, const char *fileName);
    void ReleaseSurface(const char *name, bool byGroup);
    void ReleaseAll();
    ~CTextureManager();
};

#endif
