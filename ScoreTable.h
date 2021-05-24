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
#ifndef SCORETABLE_H
#define SCORETABLE_H

#include "SDLHeaders.h"
#include "Draw.h"
#include "Font.h"
#include "SpriteText.h"

class CDraw;

class CScoreTable
{
private:
    typedef struct STableItem
    {
        char Name[32];
        int Score;
    } STableItem;

    char m_fileName[1024];
    char m_newName[32];
    char m_nameBuffer[33];
    int m_newScore;
    short m_pointer;
    CDraw *m_draw;
    STableItem *m_items;
    short m_itemCount;
    CSpriteText m_text;

public:
    CScoreTable();
    ~CScoreTable();

    inline void SetDraw(CDraw *draw)
    {
        m_draw = draw;
    }
    inline void SetScore(int value)
    {
        m_newScore = value;
    }
    bool Load();
    bool Save();
    bool CanIWriteScore();
    void Insert(const char*,int);
    void PutChar(Sint16);
    void Draw(SDL_Surface *);
};

#endif /* SCORETABLE_H */
