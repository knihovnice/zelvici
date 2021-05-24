#include "Enemy.h"

CEnm::~CEnm()
{
    delete m_gravity;
}

CEnm::CEnm()
{
    m_start = false;
    m_canHurt = true;
    m_flag = ENM_FLAG_NORMAL;
    m_direction = -1;
    m_currentLength = 0;
    m_dead = false;      // He isn't dead (yet)
    m_erase = false;     // He isn't meant to be deallocated
    m_colideNow = false; // He doesn't collide with player at the moment
    m_animate = true;    // He is animated

    m_gravity = new CGravity();
    m_hurt = false;
}

void CEnm::SetTheme(short value)
{
    m_theme = value;
    if(value == 6)
    {
        m_currentFrameX = 3;
        m_currentFrameY = 3;
        m_animate = false;
    }
}

void CEnm::GetRect(SDL_Rect *rect)
{
    rect->x = (m_currentFrameX - 1) * m_frameW;
    rect->y = (m_currentFrameY - 1) * m_frameH;
    rect->w = m_frameW;
    rect->h = m_frameH;
}

void CEnm::Walk(CZelvik *player)
{
    CCamera *camera = CCamera::GetInstance();
    CScreen *screen = CScreen::GetInstance();

    if(!m_dead && !m_erase)
    {
        if(m_theme == 6) // Bosák
        {
            if(m_start)
            {
                float X = m_x;
                float Y = m_y;

                if(m_hurt)
                {
                    m_x += (((player->GetX())- m_x + (500 * m_direction)) / 20.0f);
                    m_hurt = (absf(X - m_x) > 1.0f);
                }
                else
                {
                    m_x += (((player->GetX()) - m_x - 50) / 200.0f);
                    m_y += (((player->GetY()) - m_y - 150) / 100.0f);
                }
                m_currentFrameY = (m_x < X)? 1 : 2;
            }
            else
            {
                m_start = absf(player->GetX() - m_x) < 100.0f;
                m_animate = m_start;
                m_currentFrameX = 3;
                m_currentFrameY = 3;
            }
        }
        else
        {
            bool k = (m_currentLength * m_direction >= m_length);
            m_x += k? 0 : (m_speed * m_direction);
            m_direction = k? 0 - m_direction : m_direction;
            m_currentLength += k? m_length * (m_direction * -1) : (1 * m_direction); // olol 1 * something :)) I'll leave it here

            m_currentFrameY = k && m_length > 0? (m_currentFrameY == 1 ? (m_frameY == 1 ? 1 : 2) : 1) : m_currentFrameY;
        }
    }
    else
    {
        m_gravity->Update();
        m_y += m_gravity->Get();
        m_erase = (m_y + camera->GetY()) > screen->GetHeight();
    }
}

void CEnm::Update()
{
    CCamera *camera = CCamera::GetInstance();
    CScreen *screen = CScreen::GetInstance();

    if(!m_dead && m_animate)
    {
        m_currentFrameX += (m_currentFrameX >= m_frameX)? -m_currentFrameX + 1 : 1;

        // End of animation
        if(m_currentFrameX == m_frameX && m_flag == ENM_FLAG_NODESTRUCT && m_theme != 4)
        {
            m_animate = false;
            m_currentFrameX = 1;
        }

        if(m_asset == 4) // e5.bmp enemy
        {
            Uint8 frame = 1;

            switch(m_theme) // Depending on the theme
            {
            case 1:
                frame = 18;
                break;
            case 2:
                frame = 14;
                break;
            case 3:
                frame = 14;
                break;
            case 4:
                frame = m_currentFrameX;
                break;
            case 5:
                frame = 12;
                break;
            }

            if(m_canHurt = (m_currentFrameX == frame) && // Can he actually hurt you?
                           screen->IsOnScreen(m_x + camera->GetX(), m_y + camera->GetY(), m_frameW, m_frameH))
            {
                // Mole's sound (beaver's too)
                if(m_theme != 1 && m_theme != 4)
                    m_sound->Play("krtek", 0);
            }

            if(m_theme == 4) // Woodpecker
            {
                if(m_currentFrameX == 1 &&
                        screen->IsOnScreen(m_x + camera->GetX(), m_y + camera->GetY(),m_frameW, m_frameH))
                    m_sound->Play("date", 0);  // Play woodpecker's sound

                m_currentFrameY = m_colideNow ? 2 : 1; // Change animation frame in case of collision
            }
        }
    }
}
