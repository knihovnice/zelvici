#include "Item.h"

CItm::CItm()
{
    m_angle = 0.0F;
    m_bimBum = new CTimer(10, true);
    m_bimBum->Reset();
}

CItm::~CItm()
{
    delete m_bimBum;
}

void CItm::GetRect(SDL_Rect *rect)
{
    if(m_bimBum->Tick())
    {
        m_y += sin(m_angle);
        m_angle = m_angle > (2 * MATH_PI) ? m_angle - (2 * MATH_PI) + 0.1f : m_angle + 0.1f;
    }

    rect->x = (m_currentFrameX - 1) * m_frameW;
    rect->y = (m_currentFrameY - 1) * m_frameH;
    rect->w = m_frameW;
    rect->h = m_frameH;
}
