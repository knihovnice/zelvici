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
#include "Draw.h"
#include <string.h>

CDraw::CDraw()
{
    m_font = CFontFactory::GetInstance()->GetFont();
}

CDraw::~CDraw()
{
    if(m_enemiesKnocked) delete m_enemiesKnocked;
    if(m_vegetablesPicked) delete m_vegetablesPicked;
    if(m_pressEnter) delete m_pressEnter;
    if(m_score) delete m_score;
    if(m_commonText) delete m_commonText;
}

void CDraw::Initialize()
{
    m_particles->SetObjectCount(&m_drawObjectCount);
    m_particles->SetObjectIndex(m_drawObject);

    ENEMIES_KNOCKED = Language->GetText(23);
    VEGETABLES_PICKED = Language->GetText(24);
    PRESS_ENTER = Language->GetText(25);
    SCORE = Language->GetText(28);
    ENEMY_BONUS = Language->GetText(29);
    ITEMS_BONUS = Language->GetText(30);

    CScreen *screen = CScreen::GetInstance();
    m_enemiesKnocked = new CSpriteText(ENEMIES_KNOCKED, 20, 200);
    m_vegetablesPicked = new CSpriteText(VEGETABLES_PICKED, 20, 225);
    m_pressEnter = new CSpriteText(PRESS_ENTER, 180, screen->GetHeight() - 250);
    m_score = new CSpriteText(SCORE, screen->GetWidth() - 200, screen->GetHeight() - 50);
    m_commonText = new CSpriteText();
}

void CDraw::ResetDrawCounts()
{
    m_drawEnemyCount = 0;
    m_drawItemCount = 0;
    m_drawObjectCount = 0;
}

void CDraw::ReinitalizeDrawObjects()
{
    if(m_drawEnemy) delete m_drawEnemy;
    if(m_drawItem) delete m_drawItem;
    if(m_drawObject) delete m_drawObject;

    m_drawEnemy = new short[255];
    m_drawItem = new short[255];
    m_drawObject = new short[255];

    ResetDrawCounts();
}

// Draw the game
void CDraw::DrawGame(SDL_Surface *surface)
{
    // Draw order matters!
    m_map->Draw(surface); // Draws the map - background, items, enemies..
    m_player->Draw(surface); // Draws player
    m_particles->Draw(surface); // Draws particle effects
    DrawMisc(surface); // Draw screen stuff (score, etc.)

    SDL_Flip(surface);
}

void CDraw::DrawMisc(SDL_Surface *surface)
{
    CScreen *screen = CScreen::GetInstance();

    if(!m_map->IsEndLevel())
    {
        CButterFly *butterfly1 = m_engine->GetButterfly(1);
        CButterFly *butterfly2 = m_engine->GetButterfly(2);
        CButterFly *butterfly3 = m_engine->GetButterfly(3);

        butterfly1->Draw(surface);
        butterfly2->Draw(surface);
        butterfly3->Draw(surface);
    }
    else
    {
        m_enemiesKnocked->SetText(ENEMIES_KNOCKED, m_map->GetEnemiesKnocked(), m_map->GetEnemiesTotal());
        m_enemiesKnocked->Draw(surface);

        m_vegetablesPicked->SetText(VEGETABLES_PICKED, m_map->GetItemsPicked(), m_map->GetItemCount());
        m_vegetablesPicked->Draw(surface);

        if(m_map->GetEnemiesTotal() == m_map->GetEnemiesKnocked())
        {
            m_commonText->SetText(ENEMY_BONUS, 2 * m_map->GetEnemiesTotal());
            m_commonText->Draw(surface, 20, 140);
        }

        if(m_map->GetItemsPicked() == m_map->GetItemCount())
        {
            m_commonText->SetText(ITEMS_BONUS, 2 * m_map->GetItemCount());
            m_commonText->Draw(surface, 20, 160);
        }

        m_pressEnter->Draw(surface);
    }

    // Draw number of hearts
    m_commonText->SetText("%dx", m_player->GetHeart());
    m_commonText->Draw(surface, 20, screen->GetHeight() - 100);

    // Draw players head sprite
    CSprite *head = m_engine->GetHeadSprite();
    head->Update();
    head->Draw(surface, 30, screen->GetHeight() - 100);

    // Draw hearts
    CSprite *heart = m_engine->GetHeartSprite();
    heart->Update();
    heart->GetRect()->w = 35 * m_player->GetHealth();
    heart->Draw(surface, 100, screen->GetHeight() - 50);

    // Draw score
    m_score->SetText(SCORE, m_player->GetScore());
    m_score->Draw(surface);

    if(m_fps)
    {
        m_commonText->SetText("FPS:%d", m_engine->GetFps());
        m_commonText->Draw(surface, 10, 10);
    }
}
