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
#include "Menu.h"

//
// Constructor
//
CMenu::CMenu()
{
    this->m_scores = NULL;
    this->m_timer = new CTimer();
    this->m_animTimer = new CTimer();
    this->m_anim = new CTimer();

    this->m_animTimer->SetInterval(100);
    this->m_timer->SetInterval(10);
    this->m_timer->SetEnabled(true);
    this->m_anim->SetInterval(100);

    this->m_bird.X = 0;
    this->m_bird.Y = 0;
    this->m_bird.fX = 1;
    this->m_bird.fY = 1;

    this->m_cursor.fX = 1;
    this->m_cursor.fY = 1;

    this->m_angle = 0.0f;
    this->m_gameStarted = false;
    this->m_textures = CTextureManager::GetInstance();
}

//
// Destructor
//
CMenu::~CMenu()
{
    // Memory release
    this->m_textures->ReleaseSurface("menu", true); // Group menu release

    int count = this->m_menuItems.size();
    for(int i = 0; i < count; ++i)
        if(m_menuItems[i]->Text)
        {
            delete m_menuItems[i]->Text; // Deallocate it
        }

    this->m_menuItems.clear();

    // Deallocate timers
    delete m_timer;
    delete m_anim;
    delete m_animTimer;

    // If score table is present..
    if(m_scores) delete m_scores;

    printf("CMenu: Memory freed.\n");
}

void CMenu::Initialize()
{
    printf("CMenu: Initializing menu.\n");

    //
    // Load graphics
    //

    this->m_surfBack = m_textures->LoadSurface("back", "menu", "./screen/menu.bmp",false,0,0,0); // Background
    this->m_surfBird = m_textures->LoadSurface("bird", "menu", "./screen/bird.bmp",true,255,255,255); // Bird
    this->m_surfKurzor = m_textures->LoadSurface("kurzor", "menu", "./screen/kurzor.bmp",true,255,255,255); // Leaf

    //
    // Create Menu
    //

    // Main menu
    this->CreateMenuItem(MENU_NEWGAME, Language->GetText(1), 335, 220, false, true);
    this->CreateMenuItem(MENU_CONTINUE, Language->GetText(2), 325, 248, false, true);
    this->CreateMenuItem(MENU_SETTINGS, Language->GetText(31), 325, 276, false, true);
    this->CreateMenuItem(MENU_SCORE, Language->GetText(3), 300, 304, false, true);
    this->CreateMenuItem(MENU_CREDITS, Language->GetText(4), 350, 332, false, true);
    this->CreateMenuItem(MENU_EXIT, Language->GetText(5), 330, 360, false, true);

    // New game menu
    this->CreateMenuItem(MENU_EASY, Language->GetText(6), 340, 230, false, false);
    this->CreateMenuItem(MENU_MEDIUM, Language->GetText(7), 345, 260, false, false);
    this->CreateMenuItem(MENU_HARD, Language->GetText(8), 353, 290, false, false);
    this->CreateMenuItem(MENU_BACK, Language->GetText(9), 348, 320, false, false);

    // End game menu
    this->CreateMenuItem(MENU_NO, Language->GetText(10), 420, 320, false, false);
    this->CreateMenuItem(MENU_YES, Language->GetText(11), 330, 320, false, false);
    this->CreateMenuItem(11, Language->GetText(12), 250, 280, true, false);

    // Credits menu
    this->CreateMenuItem(12, Language->GetText(13), 340, 200, true, false);
    this->CreateMenuItem(13, Language->GetText(17), 300, 225, true, false);
    this->CreateMenuItem(14, Language->GetText(14), 345, 260, true, false);
    this->CreateMenuItem(15, Language->GetText(18), 290, 285, true, false);
    this->CreateMenuItem(16, Language->GetText(15), 350, 320, true, false);
    this->CreateMenuItem(17, Language->GetText(19), 305, 345, true, false);
    this->CreateMenuItem(18, Language->GetText(17), 300, 370, true, false);
    this->CreateMenuItem(19, Language->GetText(16), 340, 400, true, false);
    this->CreateMenuItem(20, Language->GetText(20), 320, 425, true, false);
    this->CreateMenuItem(MENU_CREDITS_BACK, Language->GetText(9), 355, 460, false, false);

    // Score table menu
    this->CreateMenuItem(MENU_SCORE_BACK, Language->GetText(9), 355, 460, false, false);
    this->CreateMenuItem(24, Language->GetText(21), 200, 100, true, false);

    // Settings
    SMenu *s1 = this->CreateMenuItem(40, Language->GetText(32), 335, 220, false, false);
    SMenu *s2 = this->CreateMenuItem(41, Language->GetText(33), 325, 248, false, false);
    SMenu *s3 = this->CreateMenuItem(42, Language->GetText(34), 325, 276, false, false);
    s1->Checkable = s2->Checkable = true;
    this->CreateMenuItem(MENU_SETTINGS_BACK, Language->GetText(9), 350, 314, false, false);

    m_common = new CSpriteText();
    YES = Language->GetText(11);
    NO = Language->GetText(10);

    // Load music..
    this->m_sound->LoadMusic("./snd/soundtrack/menu.mod", 1);
    this->m_sound->PlayMusic(1); // ..and play it now.

    printf("CMenu: Menu created.\n"); // ;)
}

int CMenu::Draw(SDL_Surface *surface)
{
    SDL_Rect rect;
    SDL_Rect pos;

    // Draw the menu background
    SDL_BlitSurface(m_surfBack, NULL, surface, NULL);

    if(GetScoreTable())         // If we get non-NULL pointer
        GetScoreTable()->Draw(surface); // Draw the Score Table

    int count = m_menuItems.size();
    for(register int i = 0; i < count; ++i)      // Draw all menu items
    {
        SMenu *item = m_menuItems[i];
        if(!item->Visible) continue; // If they are visible

        item->Text->Draw(surface, item->X, item->Y);
        if(!item->Checkable) continue;
        m_common->SetText(item->Checked? YES : NO);
        m_common->Draw(surface, item->X + item->Text->GetWidth(), item->Y);
    }

    // Draw the leaf (i.e. mouse cursor)
    pos.x = m_cursor.X;
    pos.y = m_cursor.Y;

    rect.x = 48 * (m_cursor.fX - 1);
    rect.y = 48 * (m_cursor.fY - 1);
    rect.w = 48;
    rect.h = 48;

    SDL_BlitSurface(m_surfKurzor, &rect, surface, &pos);

    // Draw the bird
    pos.x = m_bird.X;
    pos.y = m_bird.Y;

    rect.x = 95 * (m_bird.fX - 1);
    rect.y = 95 * (m_bird.fY - 1);
    rect.w = rect.h = 95;

    SDL_BlitSurface(m_surfBird, &rect, surface, &pos);

    // On screen
    SDL_Flip(surface);

    return this->Check(); // Call check method and return an event
}

int CMenu::Check()
{
    // Well, I'm not proud on these statics, but hang it! :)
    static float p_X,p_Y;
    static Uint8 PrevButton;
    float c = 0;

    // Get informations about coordinates and active button
    Uint8 NowButton = SDL_GetMouseState(&m_mice.X, &m_mice.Y);

    // Set active button only in the case it differs from previous active
    m_mice.ActiveButton = PrevButton == NowButton ?  0 : NowButton;
    PrevButton = NowButton; // :P

    m_cursor.X = m_mice.X; // Mouse coordinates
    m_cursor.Y = m_mice.Y;

    if(m_timer->Tick())
    {
        // Calculate current frame of bird
        if(m_bird.X > p_X && m_bird.Y > p_Y) m_bird.fY = 2;
        if(m_bird.X < p_X && m_bird.Y < p_Y) m_bird.fY = 6;
        if(m_bird.X < p_X && m_bird.Y > p_Y) m_bird.fY = 4;
        if(m_bird.X > p_X && m_bird.Y < p_Y) m_bird.fY = 8;
        if(m_bird.X > p_X && absf(m_bird.Y-p_Y) < 2.0f) m_bird.fY = 1;
        if(m_bird.X < p_X && absf(m_bird.Y-p_Y) < 2.0f) m_bird.fY = 5;
        if(m_bird.Y > p_Y && absf(m_bird.X-p_X) < 2.0f) m_bird.fY = 3;
        if(m_bird.Y < p_Y && absf(m_bird.X-p_X) < 2.0f) m_bird.fY = 7;

        // The bird is moving when its move difference in x and y axis are higher than 2 pixels
        if(absf(m_bird.Y-p_Y) < 2.0f && absf(m_bird.X-p_X) < 2.0f )
            m_bird.fY = 6; // It's not moving, lets set default frame

        // Calculate the speed of bird animation thanks to pythagoras theorem
        c = 150 - sqrt(absf((pow(p_Y - m_bird.Y, 2)) - pow(p_X - m_bird.X, 2))) * 10;
        m_animTimer->SetInterval(((int)c)); // Set speed of animation

        if(m_animTimer->Tick()) // Bird animation?
        {
            m_bird.fX = m_bird.fX == 2 ? 1 : 2; // Frame change
        }

        if(m_anim->Tick()) // Cursors animation
        {
            m_cursor.fX = m_cursor.fX > 3 ? 1 : m_cursor.fX + 1;
        }

        p_X = m_bird.X; // Save X and Y just before calculation
        p_Y = m_bird.Y;

        m_bird.X += ((m_mice.X - m_bird.X + 47) / 30.0f); // New position of bird
        m_bird.Y += ((m_mice.Y - m_bird.Y + 47) / 30.0f);

        // Animation of menu items; if the angle is more than 2*pi,
        // the angle is only what is more than 2*pi (angle - (2*pi))
        m_angle = m_angle > (2 * PI) ? m_angle - (2 * PI) + 0.2f : m_angle + 0.2f;
    }

    int count = this->m_menuItems.size();
    for(int i = 0; i < count; ++i) // For each menu item
    {
        SMenu *item = m_menuItems[i];
        if(item->Visible && !item->Static &&
                m_mice.X > item->X && m_mice.Y > item->Y &&
                m_mice.X < (item->X + item->Text->GetWidth()) &&
                m_mice.Y < (item->Y + item->Text->GetHeight()))
        {
            if(!item->Colide) // If this one it's not colliding
            {
                m_angle = 0.0f;
                m_sound->Play("over",0); // Play sound
            }

            if(m_mice.ActiveButton == 1) // If it's clicked
            {
                m_sound->Play("click",0); // Play click sound
                ++item->Clicked; // Increment clicked
                if(item->Checkable)
                    item->Checked = !item->Checked;
                return this->MenuClick(item->ID); // And return value from MenuClick
            }

            item->Colide = true; // Set collision = true
            item->X += cos(m_angle) / 10.0f; // X postition calculation
            item->Y += sin(m_angle) / 10.0f; // Y position calculation
        }
        else
        {
            item->Colide = false; // Collision = false
            item->X = item->OrigX; // Original X
            item->Y = item->OrigY; // ..and original Y :)
        }
    }

    return 0;
}

//
// Method initializes score table
//
void CMenu::InitializeScore()
{
    // Allocate memory for score table
    if(this->m_scores != NULL)
    {
        delete this->m_scores;
    }

    this->m_scores = new CScoreTable();
    this->m_scores->SetDraw(m_draw);  // Set pointer for draw manager
    this->m_scores->Load(); // Load score data
}

//
// Method reacts on menu items clicks
//
int CMenu::MenuClick(int ID)
{
    switch(ID)
    {
    case MENU_SETTINGS_BACK:
    {
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_SETTINGS:
    {
        MenuHideAll();
        MenuShow(25, 28);
        break;
    }
    case MENU_NEWGAME:
    {
        MenuHideAll();
        MenuShow(6, 9); // Show submenu (difficulty choosing)
        break;
    }
    case MENU_CONTINUE:
    {
        if(!(fopen(CEnvironment::GetUserPath("zelviciSave.dat"),"rb")) && !m_gameStarted)
        {
            // Cant load the game, 'cos it hasn't started yet
            return 0;
        }
        // Is game stared?
        int choice = this->m_gameStarted? MENU_CONTINUE : MENU_LOAD; // Continue game : Load saved game
        this->m_gameStarted = true;

        return choice;
    }
    case MENU_SCORE_BACK:
    {
        if(this->m_scores != NULL)
        {
            delete m_scores;
            m_scores = NULL;
        }
        MenuHideAll();
        MenuShow(0, 5); // Show main menu
        break;
    }
    case MENU_SCORE:
    {
        InitializeScore(); // Score table initialize
        MenuHideAll();
        MenuShow(23, 24);
        break;
    }
    case MENU_CREDITS_BACK:
    {
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_CREDITS:
    {
        MenuHideAll();
        MenuShow(13, 22);
        break;
    }
    case MENU_EXIT:
    {
        MenuHideAll();
        MenuShow(10, 12);

        return 0;
    }
    case MENU_EASY:
    {
        m_gameStarted = true;
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_MEDIUM:
    {
        m_gameStarted = true;
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_HARD:
    {
        m_gameStarted = true;
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_BACK:
    {
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_NO:
    {
        MenuHideAll();
        MenuShow(0, 5);
        break;
    }
    case MENU_YES:
        return MENU_EXIT;
    }
    return ID;
}

// This method will create new menuItem
SMenu *CMenu::CreateMenuItem(int id, const char *title, float x, float y, bool isStatic, bool isVisible)
{
    SMenu *item = new SMenu();
    item->Text = new CSpriteText(title == NULL? "\0" : title);
    item->ID = id;
    item->OrigX = item->X = x;
    item->OrigY = item->Y = y;
    item->Visible = isVisible;
    item->Static = isStatic;
    item->Colide = false;
    item->Clicked = 0;

    this->m_menuItems.push_back(item);

    return item;
}


void CMenu::MenuHideAll()
{
    int count = this->m_menuItems.size();
    for(int i = 0; i < count; ++i)
    {
        this->m_menuItems[i]->Visible = false;
    }
}

void CMenu::MenuShow(int start, int end)
{
    for(int i = start; i <= end; ++i)
    {
        this->m_menuItems[i]->Visible = true;
    }
}
