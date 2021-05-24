#include "Object.h"

void CObj::GetRect(SDL_Rect *rect)
{
    if(m_theme == 5 && m_asset == ASSET_TRAMPOLINE) // Is the trampoline in the fifth theme
    {
        m_timer.SetInterval(100); // Interval
        if(m_timer.Tick())
        {
            m_frameX = 5;
            m_frameW = 123;
            m_currentFrameX = m_currentFrameX > 3 ? 1 : m_currentFrameX + 1;
        }
    }

    rect->x = (m_currentFrameX - 1) * m_frameW;
    rect->y = (m_currentFrameY - 1) * m_frameH;
    rect->w = m_frameW;
    rect->h = m_frameH;
}
