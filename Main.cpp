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

#include "Engine.h"

#ifdef __WIN32__
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#endif

int main(int argc, char *argv[])
{
    printf("..::[Engine Start, (c)2006-2012 David Schwarz, http://knihovnik.net]::..\n");

    CEngine *engine = new CEngine();

    bool windowed = false;
    if(argc > 1)
    {
        windowed = StringsEquals(argv[1], "-w");
    }

    engine->Initialize(!windowed);
    engine->Start();

    delete engine;

    printf("..::[Engine Stop]::..\n");

    return 0;
}
