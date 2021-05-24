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
#include "ScoreTable.h"

CScoreTable::CScoreTable()
{
    m_pointer = 0;
    m_newScore = 0;
    m_draw = NULL;
    m_itemCount = 10;
    m_items = new STableItem[10];

    strcpy(m_fileName, CEnvironment::GetUserPath("zelviciScore.dat"));

    for(int i = 0; i < 32; ++i)
    {
        m_nameBuffer[i] = L'\0';
        m_newName[i] = L'\0';
    }
    m_nameBuffer[32] = L'\0';

    for(short i = 0; i < m_itemCount; ++i)
    {
        strcpy(m_items[i].Name, "-");
        m_items[i].Score = 0;
    }
}

CScoreTable::~CScoreTable()
{
    if(m_items) delete m_items;
}

bool CScoreTable::Load()
{
    FILE *stream = fopen(m_fileName, "rb"); // read binary
    if(stream)
    {
        printf("CScoreTable: Loading %s..\n", m_fileName);
        fread((void*) m_items, sizeof(STableItem), 10, stream); // Load the score data
        fclose(stream); // close file

        return true;
    }

    // Score table probably doesn't exists, so we try to create it :)
    Save();

    return false;
}

bool CScoreTable::Save()
{
    FILE *stream = fopen(m_fileName, "wb"); // write binary
    if(stream)
    {
        printf("CScoreTable: Saving %s..\n", m_fileName);
        fwrite((void*) m_items, sizeof(STableItem), 10, stream);
        fclose(stream);

        return true;
    }
    else
    {
        printf("CScoreTable: Cannot save score table file to %s.", m_fileName);
    }

    return false;
}

void CScoreTable::PutChar(Sint16 C)
{
    if( C == SDLK_RETURN || C == SDLK_KP_ENTER )
    {
        Insert(m_newName, m_newScore);
        strcpy(m_newName, "\0");
        m_newScore = 0;
    }
    else if( C == SDLK_BACKSPACE )
    {
        if((m_pointer - 1) > -1)
            m_newName[--m_pointer] = '\0';
    }
    else
    {
        if((m_pointer + 1) < 32)
            m_newName[m_pointer++] = (char)C;
    }

}

bool CScoreTable::CanIWriteScore()
{
    for(short i = 0; i < m_itemCount; ++i)
    {
        if(m_newScore >= m_items[i].Score)
            return true;
    }

    return false;
}

void CScoreTable::Insert(const char *name, int score)
{
    for(short i = 0; i < m_itemCount; ++i)
    {
        if(score >= m_items[i].Score) // New score is bigger than this one
        {
            if(i != (m_itemCount - 1)) // Move memory
                memmove((void*)(m_items + i), (const void*) (m_items + i - 1), 36 * (m_itemCount - i));
            strcpy(m_items[i].Name, name);
            m_items[i].Score = score;

            Save(); // Save the table

            break;
        }
    }
}

void CScoreTable::Draw(SDL_Surface *surface)
{
    if(m_newScore == 0)
    {
        char score[6];
        for(register short i = 0; i < m_itemCount; ++i)
        {
            m_text.SetText(m_items[i].Name);
            m_text.Draw(surface, 250, 150 + (i * 20));

            sprintf(score, "%d", m_items[i].Score);
            m_text.SetText(score);
            m_text.Draw(surface, 500, 150 + (i * 20));
        }
    }
    else
    {
        sprintf(m_nameBuffer, "%s|\0", m_newName);
        m_text.SetText(m_nameBuffer);
        m_text.Draw(surface, 300, 200);
    }
}

