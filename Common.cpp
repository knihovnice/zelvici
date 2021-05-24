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

#include "Common.h"
#include <cstdlib>
#include <cstring>
#include <string.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif


int ftoi(float p_Val)
{
    return ((int)p_Val);
}
float itof(int p_Val)
{
    return ((float)p_Val);
}

float absf(float p_Val)
{
    return p_Val < 0.0f ? -p_Val : p_Val;
}
int absi(int p_Val)
{
    return p_Val < 0 ? -p_Val : p_Val;
}
Sint8 absi(Sint8 p_Val)
{
    return p_Val < 0 ? -p_Val : p_Val;
}
Sint16 absi(Sint16 p_Val)
{
    return p_Val < 0 ? -p_Val : p_Val;
}

int atoi(const char *value)
{
    int result = 0;
    sscanf(value, "%d", &result);
    return result;
}

bool StringsEquals(const char *cmp1, const char *cmp2)
{
    return strcmp(cmp1, cmp2) == 0;
}
