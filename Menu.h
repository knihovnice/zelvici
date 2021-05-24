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
#ifndef MENU_H
#define MENU_H

#include "SDLHeaders.h"
#include "Draw.h"
#include "TextureManager.h"
#include "Font.h"
#include "Timer.h"
#include "Common.h"
#include "ScoreTable.h"
#include "Localizer.h"
#include "Environment.h"
#include "SpriteText.h"

#include <cmath>
#include <cstdarg>
#include <vector>

// Menu identifiers
const int MENU_NEWGAME = 1;
const int MENU_LOADGAME = 2;
const int MENU_SETTINGS = 24;
const int MENU_SETTINGS_BACK = 25;
const int MENU_SCORE = 3;
const int MENU_CREDITS = 4;
const int MENU_EXIT = 5;

const int MENU_EASY = 6;
const int MENU_MEDIUM = 7;
const int MENU_HARD = 8;

const int MENU_BACK = 9;
const int MENU_YES = 11;
const int MENU_NO = 10;
const int MENU_CONTINUE = 35;
const int MENU_LOAD = 36;
const int MENU_CREDITS_BACK = 22;
const int MENU_SCORE_BACK = 23;

typedef struct SMenu
{
    int ID;
    float X;
    float Y;
    float OrigX;
    float OrigY;
    CSpriteText *Text;
    bool Visible;
    bool Colide;
    bool Static;
    bool Checkable;
    bool Checked;
    Uint16 Clicked;
} SMenu;

class CDraw;
class CScoreTable;

using namespace std;

class CMenu
{
private:

    typedef struct SMouse
    {
        int X;
        int Y;
        Uint8 ActiveButton;
    } SMouse;

    typedef struct SFloatXY
    {
        float X;
        float Y;
        float C;
        short fX;
        short fY;
    } SFloatXY;

    int m_count;
    float m_angle;
    bool m_gameStarted;
    CDraw *m_draw;
    CTextureManager *m_textures;
    CSound *m_sound;
    CTimer *m_timer;
    CTimer *m_animTimer;
    CTimer *m_anim;
    CScoreTable *m_scores;
    CSpriteText *m_common;

    const char *YES;
    const char *NO;

    SFloatXY m_bird;         // Structure of bird's coordinates
    SFloatXY m_cursor;

    SMouse m_mice;           // Mouse structure
    vector<SMenu*> m_menuItems;
    SDL_Surface *m_surfBack; // Background
    SDL_Surface *m_surfBird; // Bird :-)
    SDL_Surface *m_surfKurzor;

public:
    CMenu();
    ~CMenu();

    void InitializeScore();
    void Initialize();

    inline void SetGameStarted(bool value)
    {
        m_gameStarted = value;
    }
    inline CScoreTable *GetScoreTable()
    {
        return m_scores;
    }
    inline void SetDraw(CDraw *draw)
    {
        m_draw = draw;
    }
    inline void SetSound(CSound *sound)
    {
        m_sound = sound;
    }
    int Draw(SDL_Surface *surface);
    int MenuClick(int);
private:
    int Check();
    void MenuHideAll();
    void MenuShow(int idStart, int idEnd);
    SMenu *CreateMenuItem(int id, const char *title, float x, float y, bool isStatic, bool isVisible);
};

#endif
