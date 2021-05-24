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
#ifndef LOCALIZER_H_INCLUDED
#define LOCALIZER_H_INCLUDED

#include "Common.h"

#include <map>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

//
// Class for different language support
//
class CLocalizer
{
private:
    char m_lang[3]; // Contains language postfix (eg. cs, eng, ..)
    map<int, string> m_texts; // Contains all text loaded from file
public:
    // Constructor
    CLocalizer(const char *lang);
    // Destructor
    ~CLocalizer();
    const char *GetText(int id);
    inline const char *GetLang()
    {
        return m_lang;
    }
};

extern CLocalizer *Language;

#endif // LOCALIZER_H_INCLUDED
